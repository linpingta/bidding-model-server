//
//

#include "bid_service.h"

Status bidAd(ServerContext* context, const BidRequest* request, BidResponse* response) {

  // 示例：简单地将请求的 id 复制到响应中
  response->set_id(request->id());

  return Status::OK;
}