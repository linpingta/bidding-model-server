# DSP Bidding Model Server

![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)

The DSP Bidding Model Server is a real-time bidding (RTB) service built with C++ that uses gRPC communication and a dual-buffer indexing system for efficient ad serving. It manages ad campaigns and ad information, processes bid requests, and returns optimal bids based on configured rules and models.

## Table of Contents

- [Features](#features)
- [Architecture](#architecture)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Usage](#usage)
  - [Configuration](#configuration)
  - [API](#api)
  - [Running the Server](#running-the-server)
- [Status](#status)
- [License](#license)

## Features

- **gRPC Communication**: Uses gRPC for high-performance, reliable communication between clients and server.
- **Dual-Buffer Indexing**: Implements a dual-buffer system for managing ad and campaign data through IndexManager, ensuring low latency and high throughput.
- **Ad Campaign Management**: Manages ad campaigns and ad information with efficient indexing.
- **Bid Request Processing**: Complete bidding pipeline including request filtering, campaign filtering, ad filtering, ranking, and response generation.
- **Logging System**: Custom SimpleLogger implementation for comprehensive logging with different log levels.
- **YAML Configuration**: Uses YAML for easy configuration management.
- **Singleton Pattern**: IndexManager implemented as a singleton for global access to indexing services.
- **Scalable Design**: Built to handle high volumes of concurrent requests.

## Architecture

The server follows a modular architecture:

1. **Main Entry Point** (`main.cpp`): Initializes logger, creates BidServerManager, and starts the service.
2. **Server Manager** (`bid_server_manager.h/cpp`): Manages server initialization, configuration loading, and service lifecycle.
3. **gRPC Service** (`bid_service.h/cpp`): Implements the AdBidderService with bidAd RPC method.
4. **Bid Handler** (`bid_handler.h/cpp`): Core bidding logic including request processing, filtering, ranking, and response generation.
5. **Indexing System**:
   - `IndexManager`: Singleton that manages dual-buffer indexing and updates
   - `CampaignIndex`: Manages campaign data
   - `AdIndex`: Manages ad data
6. **Data Structures** (`common.h`): Defines bid request/response data structures and AdSelector.
7. **Logging** (`simple_logger.h`): Custom logging implementation with different log levels.

## Getting Started

### Prerequisites

Before you begin, ensure you have met the following requirements:
- cmake
- grpc
- protobuf
- yaml-cpp

Note: grpc and protobuf versions should be compatible.

### Installation

To get the DSP Bidding Model Server up and running, follow these steps:

1. Clone this repository to your local machine:

   ```bash
   git clone https://github.com/yourusername/bidding-model-server.git
   cd bidding-model-server
   ```

2. **Install Dependencies**:

   - **On Windows**: Use vcpkg to install dependencies
     ```bash
     vcpkg install protobuf grpc yaml-cpp --triplet x64-windows
     ```
   - **On Linux/Mac**: Use your package manager to install dependencies
     ```bash
     # Ubuntu/Debian
     sudo apt-get install libprotobuf-dev libgrpc++-dev libyaml-cpp-dev
     
     # macOS with Homebrew
     brew install protobuf grpc yaml-cpp
     ```

3. **Build the Server**:

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

## Usage

### Configuration

The server reads configuration from `config.yaml` file, which should be located in the same directory as the executable. The configuration includes:

- Server port
- Timeout settings
- Index configuration paths

### API

The server exposes a gRPC API defined in `proto/ad_bid.proto`:

```protobuf
syntax = "proto3";

package ad;

service AdBidderService {
    rpc bidAd (BidRequest) returns (BidResponse);
}

// Message definitions follow...
```

#### BidRequest
- `id`: Request unique identifier
- `publisher`: Publisher information
- `site`: Website information
- `device`: User device information
- `user`: User information
- `impressions`: List of ad slot information

#### BidResponse
- `id`: Request identifier for correlation
- `seat_bids`: List of seat bids containing individual bids

### Running the Server

After successfully building the server, run it:

- **On Windows**: 
  ```bash
  bin\Release\bidding_model_server.exe
  ```
- **On Linux/Mac**: 
  ```bash
  ./bin/bidding_model_server
  ```

The server will start listening on `0.0.0.0:50051` by default.

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
using ad::Device;
using ad::User;
using ad::Site;
using ad::Impression;

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
  
  // Set site information
  Site* site = request.mutable_site();
  site->set_id("site_123");
  site->set_name("Test Site");
  
  // Set device information
  Device* device = request.mutable_device();
  device->set_id("device_123");
  device->set_os("android");
  device->set_model("Pixel 5");
  
  // Set user information
  User* user = request.mutable_user();
  user->set_id("user_123");
  user->set_gender("male");
  user->set_age(30);
  
  // Add impression
  Impression* impression = request.add_impressions();
  impression->set_id("imp_123");
  impression->set_ad_type("1");
  impression->set_bid_floor(0.5);
  
  BidResponse response = client.Bid(request);
  std::cout << "Received bid response: " << response.id() << std::endl;
  if (response.seat_bids_size() > 0) {
    const auto& seat_bid = response.seat_bids(0);
    if (seat_bid.bids_size() > 0) {
      const auto& bid = seat_bid.bids(0);
      std::cout << "Bid ID: " << bid.id() << std::endl;
      std::cout << "Ad ID: " << bid.adid() << std::endl;
      std::cout << "Price: " << bid.price() << std::endl;
    }
  }
  
  return 0;
}
```

## Status

The project is currently in development phase with core functionality implemented:
- ✅ Basic server framework with gRPC communication
- ✅ Index management system with dual-buffer support
- ✅ Complete bidding pipeline
- ✅ Logging and configuration management
- ✅ YAML configuration support

TODO:
- Complete implementation of campaign and ad filtering logic
- Add more comprehensive tests
- Implement advanced bidding strategies
- Add monitoring and metrics collection

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
