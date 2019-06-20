#include "Server.hpp"
#include "Controller.hpp"
#include "Components.hpp"

#include "oatpp/network/server/Server.hpp"
#include <iostream>


/**
 *  run() method.
 *  1) set Environment components.
 *  2) add ApiController's endpoints to router
 *  3) run server
 */
  void Server::run() {

    oatpp::base::Environment::init();
    Components components; // Create scope Environment components

    /* create ApiControllers and add endpoints to router */

    auto router = components.httpRouter.getObject();
    auto controller = Controller::createShared();
    controller->addEndpointsToRouter(router);

    /* create server */

    oatpp::network::server::Server server(components.serverConnectionProvider.getObject(),
                                          components.serverConnectionHandler.getObject());

  std::cout << "\n Server Running on port " << components.serverConnectionProvider.getObject()->getProperty("port").toString()->c_str() << "\n";

    server.run();

    oatpp::base::Environment::setLogger(nullptr); ///< free Logger

    /* Print how much objects were created during app running, and what have left-probably leaked */
    /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
    std::cout << "\nEnvironment:\n";
    std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
    std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";
    oatpp::base::Environment::destroy();
  }
