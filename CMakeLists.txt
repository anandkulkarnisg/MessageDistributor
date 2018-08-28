cmake_minimum_required(VERSION 2.8.4)
project(MessageDistributor)

include_directories(../FlexBarrier)
link_directories("$ENV{HOME}/git/FlexBarrier")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

# threads
find_package(Threads REQUIRED)

# Generate libCountDownLatch.so file
ADD_LIBRARY(MessageDistributor SHARED MessageDistributor.cpp)

add_executable(Example1.out Example1.cpp)
target_link_libraries(Example1.out MessageDistributor pthread PocoFoundation FlexBarrier)
