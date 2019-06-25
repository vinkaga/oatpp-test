#pragma once

#include "StaticFilesManager.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/data/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

using namespace oatpp::web::server::api;
using namespace oatpp::web::protocol::http;

/**
 *  Root Controller
 */
class Controller: public ApiController {

private:
  std::shared_ptr<StaticFilesManager> staticFileManager;
  std::shared_ptr<outgoing::Response> workerFile(const oatpp::String &file) const;
  std::shared_ptr<outgoing::Response> workerRange(const oatpp::String &rangeStr, const oatpp::String &file) const;
  std::shared_ptr<outgoing::Response> getStatic(const std::shared_ptr<incoming::Request> &request) const;

public:
  explicit Controller(): ApiController(oatpp::parser::json::mapping::ObjectMapper::createShared()) {
    staticFileManager = std::make_shared<StaticFilesManager>("/tmp");
  }

/**
 *  Begin ENDPOINTs generation ('ApiController' codegen)
 */
#include OATPP_CODEGEN_BEGIN(ApiController)

  ENDPOINT("GET", "/", Root) {
    return createResponse(Status::CODE_200, "");
  };

  /**
    *  Static content
    */
  ENDPOINT("GET", "/*", Static, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
    return getStatic(request);
  };

/**
 *  Finish ENDPOINTs generation ('ApiController' codegen)
 */
#include OATPP_CODEGEN_END(ApiController)

};
