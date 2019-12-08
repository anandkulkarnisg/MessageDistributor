cmake_minimum_required(VERSION 2.8.4)
project(MessageDistributor)

include_directories(../CyclicBarrier)
link_directories("$ENV{HOME}/git/CyclicBarrier")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")

# threads
find_package(Threads REQUIRED)

# Generate libCountDownLatch.so file
ADD_LIBRARY(MessageDistributor SHARED MessageDistributor.cpp)

add_executable(Example1.out Example1.cpp)
add_executable(Example2.out Example2.cpp)

target_link_libraries(Example1.out MessageDistributor pthread PocoFoundation CyclicBarrier)
target_link_libraries(Example2.out MessageDistributor pthread PocoFoundation CyclicBarrier)
