cmake_minimum_required(VERSION 3.16)
project(he_hybrid_kmeans CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenFHE REQUIRED)

add_executable(he_hybrid_kmeans src/he_hybrid_kmeans.cpp)
target_link_libraries(he_hybrid_kmeans PRIVATE OpenFHE::openfhe)
