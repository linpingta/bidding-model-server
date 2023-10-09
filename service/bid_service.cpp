//
//

#include "bid_service.h"
#include "../include/common.h"
#include "../manager/index_manager.h"

//Status bidAd(ServerContext* context, const BidRequest* request, BidResponse* response) {
//
//  // 示例：简单地将请求的 id 复制到响应中
//  response->set_id(request->id());
//
//  return Status::OK;
//}

BidService::BidService() {
  bid_handler_ = std::make_shared<BidHandler>();
  bid_handler_->Init();
}

void BidService::bidAdTest() {

  auto logger = getLogger();
  logger->info("do bid test");

  auto indexManager = IndexManager::getInstance();
}