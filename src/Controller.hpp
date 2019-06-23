#pragma once

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/data/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

using std::shared_ptr;
using oatpp::String;
using namespace oatpp::web::server::api;
using namespace oatpp::web::protocol::http;

/**
 *  Root Controller
 */
class Controller: public ApiController {
public:

  explicit Controller(const shared_ptr<ObjectMapper>& objectMapper): ApiController(objectMapper) {}

  static shared_ptr<Controller> createShared(OATPP_COMPONENT(shared_ptr<ObjectMapper>, objectMapper)) {
    return std::make_shared<Controller>(objectMapper);
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
