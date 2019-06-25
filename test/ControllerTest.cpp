#define CATCH_CONFIG_MAIN

#include "Server.hpp"
#include "TestClient.hpp"
#include "TestRunner.hpp"

#include <cstdlib>
#include "catch2/catch.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"

TEST_CASE("Index returns 200", "[Controller]") {

  TestRunner<Server, TestClient>::run([](std::shared_ptr<TestClient> client) {
    auto response = client->getRoot();
    REQUIRE(response->getStatusCode() == 200);
    auto value = response->readBodyToString();
    REQUIRE(value == "");
  }, std::chrono::minutes(10));
}

TEST_CASE("File transfer succeeds", "[Controller]") {
  std::system(R"#(echo '{"hello":"world"}'>/tmp/tmp.json)#");

  TestRunner<Server, TestClient>::run([](std::shared_ptr<TestClient> client) {
    auto response = client->getStatic();
    REQUIRE(response->getStatusCode() == 200);
    auto value = response->readBodyToString();
    REQUIRE_THAT(value.get()->std_str(), Catch::StartsWith(R"#({"hello":"world"})#"));
  }, std::chrono::minutes(10));
}
