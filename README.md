# Bidding Model Server

![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)

The Bidding Model Server is a powerful tool for managing real-time bidding (RTB) service using gRPC communication and a dual-buffer indexing system. It plans to serve for ad management in the context of online advertising and bidding scenarios.

## Table of Contents

- [Features](#features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Usage](#usage)
- [Status](#status)
- [License](#license)

## Features

- **gRPC Communication**: Bidding Model Server leverages gRPC, a high-performance RPC (Remote Procedure Call) framework, for efficient and reliable communication between clients and the server.

- **Dual-Buffer Indexing**: This server utilizes a dual-buffer indexing system to manage and retrieve RTB models, ensuring low latency and high throughput in model retrieval operations.

- **Scalable**: Bidding Model Server is designed with scalability in mind, allowing you to efficiently handle a large number of concurrent requests.

## Getting Started

### Prerequisites

Before you begin, ensure you have met the following requirements: 
- cmake
- grpc
- protobuf
grpc and protobuf should be matched

### Installation

To get the Bidding Model Server up and running with a C++ codebase, follow these steps:

1. Clone this repository to your local machine:

   ```bash
   git clone https://github.com/yourusername/bidding-model-server.git
   ```

2. Navigate to the project directory:

   ```bash
   cd bidding-model-server
   ```

3. **Build the C++ Server**:

   You'll need to compile the C++ server code. The exact compilation steps may vary depending on your project structure, but typically, you can use a C++ build system like CMake or Make.

   For example, using CMake:

   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

   Replace the above commands with the appropriate build commands for your specific C++ project.

4. **Run the Server**:

   After successfully building the C++ server, run it:

   ```bash
   ./bidding-model-server
   ```

Ensure that your C++ server executable is named accordingly, and replace `./bidding-model-server` with the correct path to your executable.

Please note that the specific build and run instructions for your C++ project may differ based on your project's structure and dependencies. Make sure to adapt the installation steps to match your project's requirements.

## Usage

To use the Bidding Model Server in your RTB application, follow these steps:

1. Import the gRPC client library into your project.

2. Establish a connection to the Bidding Model Server using gRPC.

3. Send requests to the server to retrieve RTB models or perform other related operations.

## Status

The project is still in progress, we will update examples and fix bugs.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
