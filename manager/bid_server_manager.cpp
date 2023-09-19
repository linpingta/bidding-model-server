//
//

#include "bid_server_manager.h"
#include "../include/yaml-cpp/yaml.h"
#include "../include/common.h"
#include "../include/const.h"

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

  bid_service = std::make_shared<BidService>();

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(bid_service);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();

//  GreeterServiceImpl service;
//
//  ServerBuilder builder;
//  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
//  builder.RegisterService(&service);
//
//  std::unique_ptr<Server> server(builder.BuildAndStart());
//  std::cout << "Server listening on " << server_address << std::endl;
//  server->Wait();
}

