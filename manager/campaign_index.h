//
//

#ifndef BIDDING_MODEL_SERVER_MANAGER_CAMPAIGN_INDEX_H_
#define BIDDING_MODEL_SERVER_MANAGER_CAMPAIGN_INDEX_H_

#include <unordered_map>
#include "index.h"

// extend this structure to real usage
class Campaign {
 public:
  Campaign(int32_t id, int32_t start_time, int32_t end_time): id(id), start_time(start_time), end_time(end_time) {};
 public:
  int32_t id;
  int32_t start_time;
  int32_t end_time;
};

// One example of index
class CampaignIndex: public BaseIndex {
 public:
  CampaignIndex() {};
  int load(const std::string filename);
  int dump(const std::string filename);
 private:
  std::unordered_map<int32_t, Campaign> campaignMap;
};

#endif //BIDDING_MODEL_SERVER_MANAGER_CAMPAIGN_INDEX_H_
