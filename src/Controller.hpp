#pragma once

#include "StaticFilesManager.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/data/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

using std::shared_ptr;
using oatpp::String;
using namespace oatpp::web::server::api;
using namespace oatpp::web::protocol::http;

/**
 *  Root Controller
 */
class Controller: public ApiController {

private:
  shared_ptr<StaticFilesManager> staticFileManager;

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
 *  Finish ENDPOINTs generation ('ApiController' codegen)
 */
#include OATPP_CODEGEN_END(ApiController)

};
