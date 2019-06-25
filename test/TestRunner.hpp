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

template <class Srvr, class Clnt>
class TestRunner {

private:
  std::shared_ptr<Srvr> srvr;

public:

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

    srvr = std::make_shared<Srvr>();
    std::thread serverThread([this] {
      srvr->run();
    });

    std::thread clientThread([this, &lambda] {
      lambda();
      srvr->stop();
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

    /* Additional scope here because config object should be deleted before call to oatpp::base::Environment::destroy(); */
    {
      TestRunner runner;
      runner.run_internal([&runner, &lambda] {

        auto client = std::make_shared<Clnt>();

        /* additional scope here */
        /* delete response object to free the connection */
        /* once connection is closed, connection processing coroutine of the server will exit */
        /* once all coroutines done we can properly stop the executor */
        {
          lambda(client);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////
        // Stop server and unblock accepting thread

        client->stop();

        ///////////////////////////////////////////////////////////////////////////////////////////////////////

      }, timeout);
    }

  }

};


