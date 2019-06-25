#pragma once

#include "Controller.hpp"
#include "oatpp/network/server/Server.hpp"
#include "oatpp/network/server/SimpleTCPConnectionProvider.hpp"

using std::make_shared;

class Server {

private:
  shared_ptr<oatpp::network::server::Server> server;
  shared_ptr<Controller> controller;
  shared_ptr<oatpp::web::server::HttpRouter> router;
  shared_ptr<oatpp::network::ServerConnectionProvider> connectionProvider;
  shared_ptr<oatpp::network::server::ConnectionHandler> connectionHandler;

public:
  explicit Server() {
    controller = make_shared<Controller>();
    router = oatpp::web::server::HttpRouter::createShared();
    controller->addEndpointsToRouter(router);
    connectionProvider = oatpp::network::server::SimpleTCPConnectionProvider::createShared(8000);
    connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);
    server = oatpp::network::server::Server::createShared(connectionProvider, connectionHandler);
  }
  void run();
  void stop();
};

