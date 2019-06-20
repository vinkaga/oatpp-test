#define CATCH_CONFIG_MAIN

#include "catch2/catch.hpp"
#include "WebRunner.hpp"
#include <Controller.hpp>
#include "oatpp/web/app/Client.hpp"
#include "oatpp-test/web/ClientServerTestRunner.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"

TEST_CASE("Index returns 200", "[Controller]") {

  auto runner = WebRunner::create(8000);
  runner.addController(Controller::createShared());

  runner.run([&runner] {

    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>, clientConnectionProvider);
    OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);

    auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(clientConnectionProvider);
    auto client = oatpp::test::web::app::Client::createShared(requestExecutor, objectMapper);

    auto connection = client->getConnection();
    OATPP_ASSERT(connection);

    auto response = client->getRoot(connection);
    OATPP_ASSERT(response->getStatusCode() == 200);
    auto value = response->readBodyToString();
    OATPP_ASSERT(value == "");

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    // Stop server and unblock accepting thread

    runner.getServer()->stop();
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>, connectionProvider);
    connectionProvider->getConnection();

    ///////////////////////////////////////////////////////////////////////////////////////////////////////

  }, std::chrono::minutes(10));

  std::this_thread::sleep_for(std::chrono::seconds(1));

}
