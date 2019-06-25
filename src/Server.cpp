#include "Server.hpp"

#include "oatpp/network/server/Server.hpp"
#include <iostream>

/**
 *  run() method.
 *  1) set Environment components.
 *  2) add ApiController's endpoints to router
 *  3) run server
 */
void Server::run() {
  std::cout << "\n Server Running on port " << connectionProvider->getProperty("port").getData() << "\n";
  server->run();
  std::cout << "\n Server Stopped" << "\n";
}


void Server::stop() {
  server->stop();
  connectionHandler->stop();
  connectionProvider->close();
}