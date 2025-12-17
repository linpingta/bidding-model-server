# DSP Bidding Model Server

![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)

The DSP Bidding Model Server is a real-time bidding (RTB) service built with C++ that uses gRPC communication and a dual-buffer indexing system for efficient ad serving. It manages ad campaigns and ad information, processes bid requests, and returns optimal bids based on configured rules and models.

## Table of Contents

- [Features](#features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Usage](#usage)
- [Status](#status)
- [License](#license)

## Features

- **gRPC Communication**: Uses gRPC for high-performance, reliable communication between clients and server.
- **Dual-Buffer Indexing**: Implements a dual-buffer system for managing ad and campaign data, ensuring low latency and high throughput.
- **Ad Campaign Management**: Manages ad campaigns and ad information with efficient indexing.
- **Bid Request Processing**: Processes bid requests and returns optimal bids based on configured rules.
- **Logging System**: Integrates log4cpp for comprehensive logging.
- **YAML Configuration**: Uses YAML for easy configuration management.
- **Scalable Design**: Built to handle high volumes of concurrent requests.

## Getting Started

### Prerequisites

Before you begin, ensure you have met the following requirements: 
- cmake
- grpc
- protobuf
grpc and protobuf should be matched

### Installation

To get the DSP Bidding Model Server up and running with a C++ codebase, follow these steps:

1. Clone this repository to your local machine:

   ```bash
   git clone https://github.com/yourusername/bidding-model-server.git
   cd bidding-model-server
   ```

2. **Install Dependencies**:

   - **On Windows**: Use vcpkg to install dependencies
     ```bash
     vcpkg install protobuf grpc yaml-cpp log4cpp --triplet x64-windows
     ```
   - **On Linux/Mac**: Use your package manager to install dependencies
     ```bash
     # Ubuntu/Debian
     sudo apt-get install libprotobuf-dev libgrpc++-dev libyaml-cpp-dev liblog4cpp5-dev
     
     # macOS with Homebrew
     brew install protobuf grpc yaml-cpp log4cpp
     ```

3. **Build the C++ Server**:

   - **On Windows**: Use the provided build.bat script
     ```bash
     build.bat
     ```
   - **Using CMake (cross-platform)**: 
     ```bash
     mkdir build
     cd build
     cmake .. -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake -A x64  # Windows with vcpkg
     # Or on Linux/Mac: cmake ..
     cmake --build . --config Release
     ```

4. **Run the Server**:

   After successfully building the C++ server, run it:

   - **On Windows**: 
     ```bash
     bin\Release\bidding_model_server.exe
     ```
   - **On Linux/Mac**: 
     ```bash
     ./bin/bidding_model_server
     ```

Ensure that your C++ server executable is named accordingly, and replace the path with the correct path to your executable.

## Usage

### Configuration

The server reads configuration from `config.yaml` file, which should be located in the same directory as the executable. You can modify this file to configure:

- Server listening address and port
- Logging settings
- Index configuration paths
- Other server parameters

### Using the gRPC API

To use the DSP Bidding Model Server in your RTB application, follow these steps:

1. **Import the gRPC Client Library** into your project
2. **Generate Client Code** from the proto file `proto/ad_bid.proto`
3. **Establish a Connection** to the server using gRPC
4. **Send Bid Requests** to the server using the `bidAd` method
5. **Process Bid Responses** returned by the server

### Example Client Code

```cpp
#include <grpcpp/grpcpp.h>
#include "ad_bid.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using ad::AdBidderService;
using ad::BidRequest;
using ad::BidResponse;

class AdBidClient {
public:
  AdBidClient(std::shared_ptr<Channel> channel)
      : stub_(AdBidderService::NewStub(channel)) {}

  BidResponse Bid(const BidRequest& request) {
    BidResponse response;
    ClientContext context;
    Status status = stub_->bidAd(&context, request, &response);
    if (status.ok()) {
      return response;
    } else {
      std::cerr << "RPC failed: " << status.error_message() << std::endl;
      return response;
    }
  }

private:
  std::unique_ptr<AdBidderService::Stub> stub_;
};

int main() {
  AdBidClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
  
  BidRequest request;
  request.set_id("test_request_123");
  // Set other request fields...
  
  BidResponse response = client.Bid(request);
  std::cout << "Received bid response: " << response.id() << std::endl;
  
  return 0;
}
```

## Status

The project is still in progress, we will update examples and fix bugs.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
