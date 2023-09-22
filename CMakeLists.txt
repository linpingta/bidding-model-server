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

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(OPENSSL_ROOT_DIR "${PROJECT_SOURCE_DIR}/3rd-party/openssl_test")
#set(PROTOBUF_ROOT "${PROJECT_SOURCE_DIR}/3rd-party/protobuf-24.3")
#set(PROTOBUF_ROOT "/opt/homebrew/Cellar/protobuf/24.3/")
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/protobuf/24.3/")

##set(CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/3rd-party/openssl_test" ${CMAKE_PREFIX_PATH})
#
#find_package(protobuf CONFIG)
#if(NOT Protobuf_FOUND)
#    find_package(protobuf REQUIRED)
#endif()
##find_package(protobuf CONFIG REQUIRED)
#set(PROTOBUF_LIBRARY "protobuf::libprotobuf")
#if(NOT protobuf_PROTOC_EXE)
#    set(protobuf_PROTOC_EXE "protobuf::protoc")
#endif()
#if(NOT TARGET protobuf::protoc)
#    message(FATAL_ERROR "Missing Protobuf compiler.")
#endif()

find_package(gRPC CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

#if(PROTOBUF_FOUND)
#    message("Found Protobuf: ${PROTOBUF_INCLUDE_DIR}")
#    message("Protobuf Libraries: ${PROTOBUF_LIBRARIES}")
#else()
#    message(FATAL_ERROR "Protobuf not found.")
#endif()

#message(STATUS "Using protobuf ${protobuf_VERSION}")
message(STATUS "Using gRPC ${gRPC_VERSION}")

link_directories(${PROJECT_SOURCE_DIR}/lib/yaml-cpp)
link_directories(${PROJECT_SOURCE_DIR}/lib/log4cpp)
#link_directories(/opt/homebrew/Cellar/protobuf/24.3/lib/)

#include_directories(${PROJECT_SOURCE_DIR}/3rd-party/openssl_test/include)
#link_directories(${PROJECT_SOURCE_DIR}/3rd-party/openssl_test/lib)
#link_libraries(ssl crypto)

add_executable(bidding_model_server main.cpp include/common.h manager/bid_server_manager.cpp manager/bid_server_manager.h include/const.h include/common.cpp service/bid_service.cpp service/bid_service.h manager/index_manager.cpp manager/index_manager.h manager/index.h manager/index.cpp manager/ad_index.cpp manager/ad_index.h)
#add_executable(bidding_model_server main.cpp include/common.h manager/bid_server_manager.cpp manager/bid_server_manager.h include/const.h include/common.cpp proto/ad_bid.pb.cc proto/ad_bid.pb.h)
#add_executable(bidding_model_server main.cpp include/common.h manager/bid_server_manager.cpp manager/bid_server_manager.h include/const.h include/common.cpp proto/ad_bid.pb.cc proto/ad_bid.pb.h proto/ad_bid.grpc.pb.cc proto/ad_bid.grpc.pb.h)
#add_executable(bidding_model_server main.cpp include/common.h manager/bid_server_manager.cpp manager/bid_server_manager.h include/const.h include/common.cpp service/bid_service.cpp service/bid_service.h proto/ad_bid.pb.cc proto/ad_bid.pb.h)

#target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include /opt/homebrew/Cellar/protobuf/24.3/include)
target_link_libraries(bidding_model_server libyaml-cpp.a liborocos-log4cpp.dylib gRPC::grpc++ OpenSSL::SSL protobuf::libprotobuf)
#target_link_libraries(bidding_model_server libyaml-cpp.a liborocos-log4cpp.dylib gRPC::grpc++ OpenSSL::SSL libprotobuf.dylib)
