cmake_minimum_required(VERSION 3.16)
project(TIPS-HE-MPC CXX)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH "/opt/homebrew" ${CMAKE_PREFIX_PATH})
find_package(yaml-cpp REQUIRED)

add_executable(kmeans src/main.cpp)
target_link_libraries(kmeans yaml-cpp)

