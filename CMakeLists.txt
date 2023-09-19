cmake_minimum_required(VERSION 3.14)
project(bidding_model_server)

set(CMAKE_CXX_STANDARD 14)

#find_package(Protobuf REQUIRED)
## 检查是否找到了protobuf库
#if(PROTOBUF_FOUND)
#    message(STATUS "Found Protobuf: ${PROTOBUF_VERSION}")
#else()
#    message(FATAL_ERROR "Protobuf not found")
#endif()

find_package(gRPC CONFIG REQUIRED)
#find_package(OpenSSL REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

link_directories(${PROJECT_SOURCE_DIR}/lib/yaml-cpp)
link_directories(${PROJECT_SOURCE_DIR}/lib/log4cpp)

add_executable(bidding_model_server main.cpp include/common.h manager/bid_server_manager.cpp manager/bid_server_manager.h include/const.h include/common.cpp service/bid_service.cpp service/bid_service.h proto/ad_bid.pb.cc proto/ad_bid.pb.h)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(bidding_model_server libyaml-cpp.a liborocos-log4cpp.dylib gRPC::grpc++ OpenSSL::SSL protobuf::libprotobuf)