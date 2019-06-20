cmake_minimum_required(VERSION 3.14)
project(oatpp_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


include_directories(include)
include_directories(src)
include_directories(/usr/local/include/oatpp-0.19.4/oatpp)
link_directories(/usr/local/lib/oatpp-0.19.4)

add_library(oatpp_test STATIC)
file(GLOB_RECURSE oatpp_test_SOURCE src/*.c* src/*.h*)
target_sources(oatpp_test PUBLIC ${oatpp_test_SOURCE})

include_directories(test)
add_executable(oatpp_test_test)
file(GLOB_RECURSE oatpp_test_test_SOURCE test/*.c* test/*.h*)
target_sources(oatpp_test_test PUBLIC ${oatpp_test_test_SOURCE})
target_link_libraries(oatpp_test_test PUBLIC oatpp_test oatpp oatpp-test)
