cmake_minimum_required(VERSION 3.16)
project(he_kmeans_prototype LANGUAGES CXX)
if(APPLE)
  set(CMAKE_MACOSX_RPATH ON)
  set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
  set(CMAKE_INSTALL_RPATH "/opt/homebrew/lib")
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(he_hybrid_kmeans src/he_hybrid_kmeans.cpp)
add_executable(kmeans src/main.cpp)

target_include_directories(he_hybrid_kmeans PRIVATE
  /opt/homebrew/include
  /opt/homebrew/include/openfhe
  /opt/homebrew/include/openfhe/core
  /opt/homebrew/include/openfhe/pke
  /opt/homebrew/include/openfhe/binfhe
)

target_link_libraries(he_hybrid_kmeans PRIVATE OPENFHEpke OPENFHEcore OPENFHEbinfhe)
