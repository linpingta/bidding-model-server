//
//

#ifndef BIDDING_MODEL_SERVER_SERVICE_BID_SERVICE_H_
#define BIDDING_MODEL_SERVER_SERVICE_BID_SERVICE_H_

//#include <grpcpp/grpcpp.h>
#include "../handler/bid_handler.h"
//#include "../proto.bak/ad_bid.pb.h"
#include "../proto/ad_bid.grpc.pb.h"
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using ad::BidRequest;
using ad::BidResponse;

class BidService final : public ad::AdBidderService::Service {
 public:
  BidService();
  Status bidAd(ServerContext* context, const BidRequest* request, BidResponse* response) {
    return Status::OK;
  }
  void bidAdTest();
 private:
  std::shared_ptr<BidHandler> bid_handler_;
};

//class BidService {
// public:
//  BidService();
//  void bidAdTest();
// private:
//  std::shared_ptr<BidHandler> bid_handler_;
//};

#endif //BIDDING_MODEL_SERVER_SERVICE_BID_SERVICE_H_
