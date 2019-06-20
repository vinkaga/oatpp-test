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

class TestConfig {
private:
  v_int32 m_port;
public:

  TestConfig(v_int32 port)
    : m_port(port)
  {}

  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor)([] {
    return std::make_shared<oatpp::async::Executor>(1, 1, 1);
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::virtual_::Interface>, virtualInterface)([] {
    return oatpp::network::virtual_::Interface::createShared("virtualhost");
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([this] {
    if(m_port == 0) {
      OATPP_COMPONENT(std::shared_ptr<oatpp::network::virtual_::Interface>, interface);
      return std::static_pointer_cast<oatpp::network::ServerConnectionProvider>(
        oatpp::network::virtual_::server::ConnectionProvider::createShared(interface)
      );
    }
    return std::static_pointer_cast<oatpp::network::ServerConnectionProvider>(
      oatpp::network::server::SimpleTCPConnectionProvider::createShared(m_port)
    );
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
    return oatpp::web::server::HttpRouter::createShared();
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::server::ConnectionHandler>, serverConnectionHandler)([] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
    OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor);
    return oatpp::web::server::AsyncHttpConnectionHandler::createShared(router, executor);
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper)([] {
    return oatpp::parser::json::mapping::ObjectMapper::createShared();
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>, clientConnectionProvider)([this] {

    if(m_port == 0) {
      OATPP_COMPONENT(std::shared_ptr<oatpp::network::virtual_::Interface>, interface);
      return std::static_pointer_cast<oatpp::network::ClientConnectionProvider>(
        oatpp::network::virtual_::client::ConnectionProvider::createShared(interface)
      );
    }

    return std::static_pointer_cast<oatpp::network::ClientConnectionProvider>(
      oatpp::network::client::SimpleTCPConnectionProvider::createShared("127.0.0.1", m_port)
    );

  }());

};