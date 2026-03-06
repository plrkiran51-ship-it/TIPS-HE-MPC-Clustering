cmake_minimum_required(VERSION 3.16)
project(he_hybrid_kmeans CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Manually set OpenFHE paths
set(OpenFHE_INCLUDE_DIRS "/usr/local/include/openfhe.BAK;/usr/local/include/openfhe.BAK/core;/usr/local/include/openfhe.BAK/pke;/usr/local/include/openfhe.BAK/binfhe")
set(OpenFHE_LIBRARIES "/usr/local/lib/libOPENFHEcore.dylib;/usr/local/lib/libOPENFHEpke.dylib;/usr/local/lib/libOPENFHEbinfhe.dylib")

include_directories(${OpenFHE_INCLUDE_DIRS})

add_executable(he_hybrid_kmeans src/he_hybrid_kmeans.cpp)
target_link_libraries(he_hybrid_kmeans PRIVATE ${OpenFHE_LIBRARIES})
