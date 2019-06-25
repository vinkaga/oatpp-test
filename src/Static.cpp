#include "Controller.hpp"

using oatpp::web::protocol::http::ContentRange;
using oatpp::web::protocol::http::Range;

std::shared_ptr<outgoing::Response> Controller::getStatic(const std::shared_ptr<incoming::Request> &request) const {
  auto filename = request->getPathTail();
  OATPP_ASSERT_HTTP(filename, Status::CODE_400, "Filename is empty");
  auto rangeHeader = request->getHeader(Header::RANGE);
  auto file = staticFileManager->getFile(filename);
  OATPP_ASSERT_HTTP(file.get() != nullptr, Status::CODE_404, "File not found");

  std::shared_ptr<OutgoingResponse> response;

  if (!rangeHeader) {
    response = workerFile(file);
  } else {
    response = workerRange(rangeHeader, file);
  }

  response->putHeader("Accept-Ranges", "bytes");
  response->putHeader(Header::CONNECTION, Header::Value::CONNECTION_KEEP_ALIVE);
  auto mimeType = staticFileManager->guessMimeType(filename);
  if (mimeType) {
    response->putHeader(Header::CONTENT_TYPE, mimeType);
  } else {
    OATPP_LOGD("Server", "Unknown Mime-Type. Header not set");
  }
  return response;
}

std::shared_ptr<outgoing::Response> Controller::workerFile(const String &file) const {
  return createResponse(Status::CODE_200, file);
}

std::shared_ptr<outgoing::Response> Controller::workerRange(const String &rangeStr, const String &file) const {
  auto range = Range::parse(rangeStr.getPtr());

  if (range.end == 0) {
    range.end = file->getSize() - 1;
  }

  OATPP_ASSERT_HTTP(range.isValid() &&
                    range.start < file->getSize() &&
                    range.end > range.start &&
                    range.end < file->getSize(), Status::CODE_416, "Range is invalid");

  auto chunk = String((const char*)&file->getData()[range.start], (v_int32)(range.end - range.start + 1), false);
  auto response = createResponse(Status::CODE_206, chunk);
  ContentRange contentRange(ContentRange::UNIT_BYTES, range.start, range.end, file->getSize(), true);
  OATPP_LOGD("Server", "range=%s", contentRange.toString()->c_str());
  response->putHeader(Header::CONTENT_RANGE, contentRange.toString());
  return response;
}
