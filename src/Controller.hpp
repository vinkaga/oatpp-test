#pragma once

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/web/protocol/http/outgoing/ChunkedBufferBody.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include <unordered_map>
#include <iostream>

using std::shared_ptr;
using oatpp::String;
using namespace oatpp::web::server::api;
using namespace oatpp::web::protocol::http;

/**
 *  Root Controller
 */
class Controller : public oatpp::web::server::api::ApiController {
public:
  typedef Controller __ControllerType;

public:
  explicit Controller(const shared_ptr<ObjectMapper>& objectMapper): ApiController(objectMapper) {}
  static shared_ptr<Controller> createShared(OATPP_COMPONENT(shared_ptr<ObjectMapper>, objectMapper)) {
    return shared_ptr<Controller>(new Controller(objectMapper));
  }

  /**
   *  Begin ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_BEGIN(ApiController)
  ENDPOINT_ASYNC("GET", "/", Root) {
    ENDPOINT_ASYNC_INIT(Root)
    Action act() override {
      return _return(controller->createResponse(Status::CODE_200, ""));
    }
  };

  /**
   *  Finish ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_END(ApiController)

};
