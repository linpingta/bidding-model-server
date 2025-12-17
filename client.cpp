#include <grpcpp/grpcpp.h>
#include "proto/ad_bid.grpc.pb.h"

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
