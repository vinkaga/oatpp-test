#include "../include/Server.hpp"

int main(int argc, const char * argv[]) {
  oatpp::base::Environment::init();

  Server server;
  server.run();

  oatpp::base::Environment::destroy();

  return 0;
}
