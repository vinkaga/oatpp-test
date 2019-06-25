cmake_minimum_required(VERSION 3.14)
project(oatppex)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
SET(CMAKE_CXX_FLAGS     " -pthread")


include_directories(include)
include_directories(src)
include_directories(app)
include_directories(/usr/local/include/oatpp-0.19.4/oatpp)
link_directories(/usr/local/lib/oatpp-0.19.4)

add_library(oatppex STATIC)
file(GLOB_RECURSE oatppex_SOURCE src/*.c* src/*.h*)
target_sources(oatppex PUBLIC ${oatppex_SOURCE})

add_executable(oatppex_app)
target_sources(oatppex_app PUBLIC app/App.cpp)
target_link_libraries(oatppex_app PUBLIC oatppex oatpp oatpp-test)

include_directories(test)
add_executable(oatppex_test test/TestClient.hpp)
file(GLOB_RECURSE oatppex_test_SOURCE test/*.c* test/*.h*)
target_sources(oatppex_test PUBLIC ${oatppex_test_SOURCE})
target_link_libraries(oatppex_test PUBLIC oatppex oatpp oatpp-test)
