//
//

#ifndef BIDDING_MODEL_SERVER_MANAGER_AD_INDEX_H_
#define BIDDING_MODEL_SERVER_MANAGER_AD_INDEX_H_

#include <unordered_map>
#include "index.h"

class Ad {
  int32_t id;
  int64_t budget;
};

// One example of index
class AdIndex: public Index {
 public:
  int load(const std::string filename);
  int dump(const std::string filename);
 private:
  std::unordered_map<int32_t, Ad>;
};

#endif //BIDDING_MODEL_SERVER_MANAGER_AD_INDEX_H_
