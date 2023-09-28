//
//

#include "bid_handler.h"

void BidHandler::doBidding(const BidReqData& bidReqData, BidRspData& bidRspData) {
  auto logger = getLogger();
}

bool BidHandler::targetingFilter(const BidReqData& bidReqData) {
  return false;
}

bool BidHandler::rank() {
  return false;
}

std::vector<Ad> BidHandler::selectAds() {
  std::vector<Ad> ads;
  return ads;
}

BidRspData BidHandler::fillResponse(const BidReqData& bidReqData, std::vector<Ad> rspAdList) {
  return BidRspData();
}


