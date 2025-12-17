//
//

#include "bid_service.h"
#include "../include/common.h"
#include "../manager/index_manager.h"

BidService::BidService() {
  bid_handler_ = std::make_shared<BidHandler>();
  bid_handler_->Init();
}

Status BidService::bidAd(ServerContext* context, const BidRequest* request, BidResponse* response) {
  // 将请求的 id 复制到响应中
  response->set_id(request->id());
  
  // 简单示例：创建一个座位出价
  auto seat_bid = response->add_seat_bids();
  
  // 添加一个出价
  auto bid = seat_bid->add_bids();
  bid->set_id("bid_" + request->id());
  bid->set_imp_id(request->impressions(0).id());
  bid->set_price(0.5);
  bid->set_adid("ad_123");
  
  return Status::OK;
}

void BidService::bidAdTest() {
  auto logger = getLogger();
  logger->info("do bid test");
  auto indexManager = IndexManager::getInstance();
}