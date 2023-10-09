//
//

#ifndef BIDDING_MODEL_SERVER_MANAGER_AD_INDEX_H_
#define BIDDING_MODEL_SERVER_MANAGER_AD_INDEX_H_

#include <unordered_map>
#include "index.h"

class Ad {
 public:
  Ad() {};
  Ad(int32_t id, int64_t budget, int64_t price, int32_t campaign_id): id(id), budget(budget), price(price), campaign_id(campaign_id) {};
  Ad(const Ad& src) {
    id = src.id;
    budget = src.budget;
    price = src.price;
    campaign_id = src.campaign_id;
  }
 public:
  int32_t id;
  int64_t budget;
  int64_t price; // origin price
  int32_t campaign_id;
};

// One example of index
class AdIndex: public BaseIndex {
 public:
  AdIndex() {};
  int load(const std::string filename);
  int dump(const std::string filename);
  std::unordered_map<int32_t, Ad> getAdMap() { return adMap; };
 private:
  std::unordered_map<int32_t, Ad> adMap;
};

#endif //BIDDING_MODEL_SERVER_MANAGER_AD_INDEX_H_
