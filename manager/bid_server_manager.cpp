//
//

#include "bid_server_manager.h"
#include "index_manager.h"
#include "../include/yaml-cpp/yaml.h"
#include "../include/common.h"
#include "../include/const.h"

#include <grpcpp/grpcpp.h>


//#include "../proto/ad_bid.pb.h"
//#include "../proto/ad_bid.grpc.pb.h"
//using grpc::Server;
//using grpc::ServerBuilder;
//using grpc::ServerContext;
//using grpc::Status;
//using ad::BidRequest;
//using ad::BidResponse;
//
//class BidService final : public ad::AdBidderService::Service {
// public:
//  Status bidAd(ServerContext* context, const BidRequest* request, BidResponse* response) {
//    response->set_id(request->id());
//    return Status::OK;
//  }
//};


int BidServerManager::init(std::string config_filename) {

  YAML::Node config = YAML::LoadFile(config_filename);

  this->port = config["port"].as<int>(kThreadNum);
  this->send_timeout = config["send_timeout"].as<int>(kSendTimeout);
  this->recv_timeout = config["recv_timeout"].as<int>(kRecvTimeout);

  auto logger = getLogger();
  logger->info("init port: %d, send_timeout: %d, recc_timeout: %d", this->port, this->send_timeout, this->recv_timeout);

  // init index
  logger->info("start index service");
  auto indexManager = IndexManager::getInstance();
  if (indexManager->init(config_filename) < 0) {
    logger->error("index manager init fail");
    return -1;
  }

  return 1;
}

void BidServerManager::run() {
  std::string server_address("0.0.0.0:50051");

  auto bid_service = std::make_shared<BidService>();
  bid_service->bidAdTest();

//  // test later for protobuf
//  ServerBuilder builder;
//  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
//  builder.RegisterService(bid_service.get());
//
//  std::unique_ptr<Server> server(builder.BuildAndStart());
//  std::cout << "Server listening on " << server_address << std::endl;
//  server->Wait();

}

