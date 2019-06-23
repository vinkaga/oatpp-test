#pragma once
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/web/server/HttpRouter.hpp"

#include "oatpp/network/server/Server.hpp"
#include "oatpp/network/ConnectionProvider.hpp"

#include "oatpp/core/macro/component.hpp"

#include <list>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <thread>

template <class Ctrl, class Clnt>
class TestRunner {

public:
  typedef oatpp::web::server::HttpRouter HttpRouter;
  typedef oatpp::web::server::api::ApiController ApiController;
private:
  std::shared_ptr<oatpp::network::server::Server> m_server;
  std::list<std::shared_ptr<ApiController>> m_controllers;
  OATPP_COMPONENT(std::shared_ptr<HttpRouter>, m_router);
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, m_connectionProvider);
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::server::ConnectionHandler>, m_connectionHandler);
public:

  std::shared_ptr<HttpRouter> getRouter() {
    return m_router;
  }

  /**
   * Add controller's endpoints to router
   * @param controller
   */
  void addController(const std::shared_ptr<ApiController>& controller) {
    controller->addEndpointsToRouter(m_router);
    m_controllers.push_back(controller);
  }

  std::shared_ptr<oatpp::network::server::Server> getServer() {
    return m_server;
  }

  /**
   * Start server, execute code block passed as lambda, stop server.
   * @tparam Lambda
   * @param lambda
   * @param timeout
   */
  template<typename Lambda>
  void run_internal(
      const Lambda &lambda,
      const std::chrono::duration<v_int64, std::micro> &timeout = std::chrono::hours(12)
  ) {



    auto startTime = std::chrono::system_clock::now();
    bool running = true;
    std::mutex timeoutMutex;
    std::condition_variable timeoutCondition;

    m_server = std::make_shared<oatpp::network::server::Server>(m_connectionProvider, m_connectionHandler);
    OATPP_LOGD("\033[1;34mClientServerTestRunner\033[0m", "\033[1;34mRunning server on port %s. Timeout %lld(micro)\033[0m",
               m_connectionProvider->getProperty("port").toString()->c_str(),
               timeout.count());

    std::thread serverThread([this]{
      m_server->run();
    });

    std::thread clientThread([this, &lambda]{

      lambda();

      m_server->stop();
      m_connectionHandler->stop();
      m_connectionProvider->close();

    });

    std::thread timerThread([&timeout, &startTime, &running, &timeoutMutex, &timeoutCondition]{

      auto end = startTime + timeout;
      std::unique_lock<std::mutex> lock(timeoutMutex);
      while(running) {
        timeoutCondition.wait_for(lock, std::chrono::seconds(1));
        auto elapsed = std::chrono::system_clock::now() - startTime;
        OATPP_ASSERT("ClientServerTestRunner: Error. Timeout." && elapsed < timeout);
      }

    });

    serverThread.join();
    clientThread.join();

    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - startTime);
    OATPP_LOGD("\033[1;34mClientServerTestRunner\033[0m", "\033[1;34mFinished with time %lld(micro). Stopping server...\033[0m", elapsed.count());

    running = false;
    timeoutCondition.notify_one();

    timerThread.join();

  }

  /**
   * Setup server and run test
   * @tparam Lambda
   * @param lambda
   * @param timeout
   */
  template<typename Lambda>
  static void run(
      const Lambda &lambda,
      const std::chrono::duration<v_int64, std::micro> &timeout
  ) {
    /* Init oatpp environment with default logger */
    oatpp::base::Environment::init();

    /* Additional scope here because config object should be deleted before call to oatpp::base::Environment::destroy(); */
    {

      /* config should be alive at the scope of the test. */
      /* should be deleted before call to oatpp::base::Environment::destroy(); */
      /* Use port == 0 for oatpp virtual networking without occupying the port */
      TestComponents components;

      TestRunner runner;
      OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);
      runner.addController(Ctrl::createShared());

      runner.run_internal([&runner, &lambda] {

        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>, clientConnectionProvider);
        OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);

        auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(clientConnectionProvider);
        auto client = Clnt::createShared(requestExecutor, objectMapper);

        /* additional scope here */
        /* delete response object to free the connection */
        /* once connection is closed, connection processing coroutine of the server will exit */
        /* once all coroutines done we can properly stop the executor */
        {
          lambda(client);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////
        // Stop server and unblock accepting thread

        runner.getServer()->stop();
        clientConnectionProvider->getConnection();

        ///////////////////////////////////////////////////////////////////////////////////////////////////////

      }, timeout);
    }

    /* Check for oatpp leaking objects */
    OATPP_ASSERT(oatpp::base::Environment::getObjectsCount() == 0);

    oatpp::base::Environment::destroy();

  }

};


