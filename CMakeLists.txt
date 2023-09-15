cmake_minimum_required(VERSION 3.14)
project(bidding_model_server)

set(CMAKE_CXX_STANDARD 14)

link_directories(${PROJECT_SOURCE_DIR}/lib/yaml-cpp)
link_directories(${PROJECT_SOURCE_DIR}/lib/log4cpp)

find_package(gRPC CONFIG REQUIRED)

add_executable(bidding_model_server main.cpp include/common.h manager/bid_server_manager.cpp manager/bid_server_manager.h include/const.h include/common.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(bidding_model_server libyaml-cpp.a liborocos-log4cpp.dylib gRPC::grpc++)