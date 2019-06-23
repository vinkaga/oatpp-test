#define CATCH_CONFIG_MAIN

#include "catch2/catch.hpp"

#include <Controller.hpp>
#include "TestClient.hpp"
#include "TestComponents.hpp"

#include "oatpp/web/client/HttpRequestExecutor.hpp"
#include "TestRunner.hpp"

TEST_CASE("Index returns 200", "[Controller]") {

  TestRunner<Controller, TestClient>::run([](shared_ptr<TestClient> client) {
    auto response = client->getRoot();
    REQUIRE(response->getStatusCode() == 200);
    auto value = response->readBodyToString();
    REQUIRE(value == "");
  }, std::chrono::minutes(10));

}
