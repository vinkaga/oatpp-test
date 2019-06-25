#define CATCH_CONFIG_MAIN

#include "catch2/catch.hpp"

#include <Server.hpp>
#include "TestClient.hpp"

#include "oatpp/web/client/HttpRequestExecutor.hpp"
#include "TestRunner.hpp"

TEST_CASE("Index returns 200", "[Controller]") {

  TestRunner<Server, TestClient>::run([](shared_ptr<TestClient> client) {
    auto response = client->getRoot();
    REQUIRE(response->getStatusCode() == 200);
    auto value = response->readBodyToString();
    REQUIRE(value == "");
  }, std::chrono::minutes(10));

}
