#pragma once

#include <list>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <thread>

template <class Srvr, class Clnt>
class TestRunner {

public:

  /**
   * Start server, execute code block passed as lambda, stop server.
   * @tparam Lambda
   * @param lambda
   * @param timeout
   */
  template<typename Lambda>
  static void run(
      const Lambda &lambda,
      const std::chrono::duration<v_int64, std::micro> &timeout = std::chrono::hours(12)
  ) {

    oatpp::base::Environment::init();

    auto startTime = std::chrono::system_clock::now();
    bool running = true;
    std::mutex timeoutMutex;
    std::condition_variable timeoutCondition;

    std::shared_ptr<Srvr> srvr = std::make_shared<Srvr>();
    std::thread serverThread([srvr] {
      srvr->run();
    });

    std::thread clientThread([srvr, &lambda] {
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

    oatpp::base::Environment::destroy();
  }

};


