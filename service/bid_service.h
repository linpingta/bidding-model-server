//
//

#ifndef BIDDING_MODEL_SERVER_SERVICE_BID_SERVICE_H_
#define BIDDING_MODEL_SERVER_SERVICE_BID_SERVICE_H_

#include <grpcpp/grpcpp.h>
#include "../proto/ad_bid.pb.h"
#include "../proto/ad_bid.grpc.pb.h"


using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

//class GreeterServiceImpl final : public Greeter::Service {
//  Status SayHello(ServerContext* context, const HelloRequest* request,
//                  HelloReply* reply) override {
//    std::string prefix("Hello, ");
//    reply->set_message(prefix + request->name());
//    return Status::OK;
//  }
//};

 class BidService final : public AdBidderService::Service {
 public:
  Status bidAd(ServerContext* context, const BidRequest* request, BidResponse* response);
};

#endif //BIDDING_MODEL_SERVER_SERVICE_BID_SERVICE_H_
