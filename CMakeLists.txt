cmake_minimum_required(VERSION 3.14)
project(bidding_model_server)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Option to use system libraries or local libraries
option(USE_SYSTEM_LIBS "Use system installed libraries" OFF)

# Find dependencies
find_package(protobuf CONFIG QUIET)
if(NOT protobuf_FOUND)
    find_package(Protobuf REQUIRED)
    set(protobuf_LIBRARIES ${Protobuf_LIBRARIES})
    set(protobuf_INCLUDE_DIRS ${Protobuf_INCLUDE_DIRS})
    set(protobuf_PROTOC_EXECUTABLE ${Protobuf_PROTOC_EXECUTABLE})
    set(protobuf_FOUND TRUE)
else()
    set(protobuf_LIBRARIES protobuf::libprotobuf)
    set(protobuf_INCLUDE_DIRS ${protobuf_INCLUDE_DIRS})
    set(protobuf_PROTOC_EXECUTABLE protobuf::protoc)
endif()

find_package(gRPC CONFIG QUIET)
if(NOT gRPC_FOUND)
    find_package(GRPC REQUIRED)
    set(gRPC_LIBRARIES ${GRPC_LIBRARIES})
    set(gRPC_INCLUDE_DIRS ${GRPC_INCLUDE_DIRS})
    set(gRPC_FOUND TRUE)
else()
    set(gRPC_LIBRARIES gRPC::grpc++)
    set(gRPC_INCLUDE_DIRS ${gRPC_INCLUDE_DIRS})
endif()

find_package(OpenSSL REQUIRED)
find_package(yaml-cpp CONFIG QUIET)
if(NOT yaml-cpp_FOUND)
    find_package(YAML-CPP REQUIRED)
    set(yaml-cpp_LIBRARIES ${YAML-CPP_LIBRARIES})
    set(yaml-cpp_INCLUDE_DIRS ${YAML-CPP_INCLUDE_DIRS})
    set(yaml-cpp_FOUND TRUE)
else()
    set(yaml-cpp_LIBRARIES yaml-cpp::yaml-cpp)
    set(yaml-cpp_INCLUDE_DIRS ${yaml-cpp_INCLUDE_DIRS})
endif()

# Use project's built-in log4cpp headers
set(LOG4CPP_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/log4cpp)
set(LOG4CPP_LIBRARY "")
message(STATUS "Using built-in log4cpp headers from ${LOG4CPP_INCLUDE_DIR}")

message(STATUS "Using protobuf: ${protobuf_INCLUDE_DIRS}, ${protobuf_LIBRARIES}")
message(STATUS "Using gRPC: ${gRPC_INCLUDE_DIRS}, ${gRPC_LIBRARIES}")
message(STATUS "Using yaml-cpp: ${yaml-cpp_INCLUDE_DIRS}, ${yaml-cpp_LIBRARIES}")
message(STATUS "Using log4cpp: ${LOG4CPP_INCLUDE_DIR}, ${LOG4CPP_LIBRARY}")

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${protobuf_INCLUDE_DIRS}
    ${gRPC_INCLUDE_DIRS}
    ${yaml-cpp_INCLUDE_DIRS}
    ${LOG4CPP_INCLUDE_DIR}
)

# Source files
set(SOURCES
    main.cpp
    include/common.cpp
    manager/bid_server_manager.cpp
    manager/index_manager.cpp
    manager/index.cpp
    manager/ad_index.cpp
    manager/campaign_index.cpp
    service/bid_service.cpp
    proto/ad_bid.pb.cc
    proto/ad_bid.grpc.pb.cc
)

# Header files (for IDE support)
set(HEADERS
    include/common.h
    include/const.h
    include/simple_logger.h
    handler/bid_handler.h
    manager/bid_server_manager.h
    manager/index_manager.h
    manager/index.h
    manager/ad_index.h
    manager/campaign_index.h
    service/bid_service.h
    proto/ad_bid.pb.h
    proto/ad_bid.grpc.pb.h
)

# Create executable
add_executable(bidding_model_server ${SOURCES} ${HEADERS})

# Link libraries
if(MSVC)
    # For Visual Studio - use vcpkg managed libraries
    target_link_libraries(bidding_model_server
        ${protobuf_LIBRARIES}
        ${gRPC_LIBRARIES}
        ${yaml-cpp_LIBRARIES}
        OpenSSL::SSL
        OpenSSL::Crypto
    )
else()
    # For MinGW - use system libraries or vcpkg libraries
    target_link_libraries(bidding_model_server
        ${protobuf_LIBRARIES}
        ${gRPC_LIBRARIES}
        ${yaml-cpp_LIBRARIES}
        ${OPENSSL_LIBRARIES}
    )
endif()

# Windows specific libraries
if(WIN32)
    target_link_libraries(bidding_model_server ws2_32 wsock32)
endif()

# Set output directory
set_target_properties(bidding_model_server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)

# Copy configuration files to output directory
configure_file(config.yaml ${PROJECT_SOURCE_DIR}/bin/config.yaml COPYONLY)
configure_file(data/index_config_0 ${PROJECT_SOURCE_DIR}/bin/data/index_config_0 COPYONLY)
configure_file(data/index_config_1 ${PROJECT_SOURCE_DIR}/bin/data/index_config_1 COPYONLY)

# Create data directory in output
add_custom_command(TARGET bidding_model_server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/bin/data
)
