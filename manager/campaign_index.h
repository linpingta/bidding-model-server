//
//

#ifndef BIDDING_MODEL_SERVER_MANAGER_CAMPAIGN_INDEX_H_
#define BIDDING_MODEL_SERVER_MANAGER_CAMPAIGN_INDEX_H_

#include <unordered_map>
#include "index.h"

// extend this structure to real usage
class Campaign {
 public:
  Campaign() {};
  Campaign(int32_t id, int32_t start_time, int32_t end_time): id(id), start_time(start_time), end_time(end_time) {};
  Campaign(const Campaign& src) {
    id = src.id;
    start_time = src.start_time;
    end_time = src.end_time;
  };
  Campaign& operator=(const Campaign& other) {
    if (this == &other) {
      return *this;  // Handle self-assignment
    }
    id = other.id;
    start_time = other.start_time;
    end_time = other.end_time;
    return *this;
  };
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
  std::unordered_map<int32_t, Campaign> getCampaignMap() { return campaignMap; };
 private:
  std::unordered_map<int32_t, Campaign> campaignMap;
};

#endif //BIDDING_MODEL_SERVER_MANAGER_CAMPAIGN_INDEX_H_
