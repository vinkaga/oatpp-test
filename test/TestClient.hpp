#pragma once

#include "oatpp/web/client/HttpRequestExecutor.hpp"
#include "oatpp/web/server/AsyncHttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/network/server/SimpleTCPConnectionProvider.hpp"
#include "oatpp/network/client/SimpleTCPConnectionProvider.hpp"
#include "oatpp/network/virtual_/client/ConnectionProvider.hpp"
#include "oatpp/network/virtual_/server/ConnectionProvider.hpp"
#include "oatpp/network/virtual_/Interface.hpp"
#include "oatpp/core/macro/component.hpp"

#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/core/macro/codegen.hpp"

/**
 * Test API client.
 * Use this client to call application APIs.
 */
class TestClient: public oatpp::web::client::ApiClient {

public:
  explicit TestClient():
      oatpp::web::client::ApiClient(
          oatpp::web::client::HttpRequestExecutor::createShared(
              oatpp::network::client::SimpleTCPConnectionProvider::createShared("127.0.0.1", 8000)),
          oatpp::parser::json::mapping::ObjectMapper::createShared()
      )  {}

  void stop() {
    getConnection();
  }

/* Begin Api Client code generation */
#include OATPP_CODEGEN_BEGIN(ApiClient)

  API_CALL("GET", "/", getRoot)
  API_CALL("GET", "/tmp.json", getStatic)

};

/* End Api Client code generation */
#include OATPP_CODEGEN_END(ApiClient)

