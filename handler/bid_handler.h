//
//

#ifndef BIDDING_MODEL_SERVER_HANDLER_BID_HANDLER_H_
#define BIDDING_MODEL_SERVER_HANDLER_BID_HANDLER_H_

#include <vector>
#include "../include/common.h"
#include "../manager/index_manager.h"

class BidHandler {
 public:
  BidHandler() {};
  void Init();
  void Bid(const BidReqData& bidReqData, BidRspData& bidRspData);
 private:
  bool reqFilter(const BidReqData& bidReqData);
  std::unordered_map<int32_t, Campaign> campaignFilter(const BidReqData& bidReqData);
  std::unordered_map<int32_t, Ad> adFilter(const BidReqData& bidReqData, const std::unordered_map<int32_t, Campaign>& campaignMap);
  std::vector<Ad> rank(const std::unordered_map<int32_t, Ad>& adMap);
  BidRspData fillResponse(const BidReqData& bidReqData, std::vector<Ad> candidateAdList, int32_t rspAdNum);
 private:
  IndexManager* index_manager_;
  log4cpp::Category* logger_;
};

#endif //BIDDING_MODEL_SERVER_HANDLER_BID_HANDLER_H_
