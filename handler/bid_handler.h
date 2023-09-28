//
//

#ifndef BIDDING_MODEL_SERVER_HANDLER_BID_HANDLER_H_
#define BIDDING_MODEL_SERVER_HANDLER_BID_HANDLER_H_

#include <vector>
#include "../include/common.h"
#include "../manager/ad_index.h"
#include "../manager/campaign_index.h"

class BidHandler {
 public:
  BidHandler() {};
  void doBidding(const BidReqData& bidReqData, BidRspData& bidRspData);
 private:
  bool targetingFilter(const BidReqData& bidReqData);
  bool rank();
  std::vector<Ad> selectAds();
  BidRspData fillResponse(const BidReqData& bidReqData, std::vector<Ad> rspAdList);
 private:
  AdIndex* ad_index_;
  CampaignIndex* campaign_index_;
};

#endif //BIDDING_MODEL_SERVER_HANDLER_BID_HANDLER_H_
