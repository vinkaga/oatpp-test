#pragma once

#include "Controller.hpp"
#include "oatpp/network/server/Server.hpp"
#include "oatpp/network/server/SimpleTCPConnectionProvider.hpp"

class Server {

private:
  std::shared_ptr<oatpp::network::server::Server> server;
  std::shared_ptr<Controller> controller;
  std::shared_ptr<oatpp::web::server::HttpRouter> router;
  std::shared_ptr<oatpp::network::ServerConnectionProvider> connectionProvider;
  std::shared_ptr<oatpp::network::server::ConnectionHandler> connectionHandler;

public:
  explicit Server() {
    controller = std::make_shared<Controller>();
    router = oatpp::web::server::HttpRouter::createShared();
    controller->addEndpointsToRouter(router);
    connectionProvider = oatpp::network::server::SimpleTCPConnectionProvider::createShared(8000);
    connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);
    server = oatpp::network::server::Server::createShared(connectionProvider, connectionHandler);
  }
  void run();
  void stop();
};

