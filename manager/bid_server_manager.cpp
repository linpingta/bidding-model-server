//
//

#include "bid_server_manager.h"
#include "../include/yaml-cpp/yaml.h"
#include "../include/common.h"
#include "../include/const.h"

#include <grpcpp/grpcpp.h>
#include "../proto/bid_service.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using greeter::Greeter;
using greeter::HelloRequest;
using greeter::HelloReply;

class GreeterServiceImpl final : public Greeter::Service {
  Status SayHello(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply) override {
    std::string prefix("Hello, ");
    reply->set_message(prefix + request->name());
    return Status::OK;
  }
};

int BidServerManager::init(std::string config_filename) {

  YAML::Node config = YAML::LoadFile(config_filename);

  this->port = config["port"].as<int>(kThreadNum);
  this->send_timeout = config["send_timeout"].as<int>(kSendTimeout);
  this->recv_timeout = config["recv_timeout"].as<int>(kRecvTimeout);

  auto logger = getLogger();
  logger->info("init port: %d, send_timeout: %d, recc_timeout: %d", this->port, this->send_timeout, this->recv_timeout);

  return 1;
}

void BidServerManager::run() {
  std::string server_address("0.0.0.0:50051");
  GreeterServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

