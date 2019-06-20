#pragma once
#include "oatpp-test/web/ClientServerTestRunner.hpp"

class WebRunner {
public:
  static oatpp::test::web::ClientServerTestRunner create(int port);
};
