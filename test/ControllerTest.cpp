#define CATCH_CONFIG_MAIN

#include "catch2/catch.hpp"

#include <Controller.hpp>
#include "TestClient.hpp"
#include "TestComponents.hpp"

#include "oatpp/web/client/HttpRequestExecutor.hpp"
#include "oatpp-test/web/ClientServerTestRunner.hpp"

TEST_CASE("Index returns 200", "[Controller]") {

  /* Init oatpp environment with default logger */
  oatpp::base::Environment::init();

  /* Additional scope here because config object should be deleted before call to oatpp::base::Environment::destroy(); */
  {

    /* config should be alive at the scope of the test. */
    /* should be deleted before call to oatpp::base::Environment::destroy(); */
    /* Use port == 0 for oatpp virtual networking without occupying the port */
    TestComponents config;

    oatpp::test::web::ClientServerTestRunner runner;
    runner.addController(Controller::createShared());

    runner.run([&runner] {

      OATPP_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>, clientConnectionProvider);
      OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);

      auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(clientConnectionProvider);
      auto client = TestClient::createShared(requestExecutor, objectMapper);

      /* additional scope here */
      /* delete response object to free the connection */
      /* once connection is closed, connection processing coroutine of the server will exit */
      /* once all coroutines done we can properly stop the executor */
      {

        auto response = client->getRoot();
        REQUIRE(response->getStatusCode() == 200);
        auto value = response->readBodyToString();
        REQUIRE(value == "");

      }

      ///////////////////////////////////////////////////////////////////////////////////////////////////////
      // Stop server and unblock accepting thread

      runner.getServer()->stop();
      clientConnectionProvider->getConnection();

      ///////////////////////////////////////////////////////////////////////////////////////////////////////

    }, std::chrono::minutes(10));
  }

  /* Check for oatpp leaking objects */
  REQUIRE(oatpp::base::Environment::getObjectsCount() == 0);

  oatpp::base::Environment::destroy();

}
