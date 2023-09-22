//
//

#ifndef BIDDING_MODEL_SERVER_MANAGER_INDEX_H_
#define BIDDING_MODEL_SERVER_MANAGER_INDEX_H_

#include <string>

class Index {
 public:
  Index(): inUseCount(0) {}
  virtual int load(const std::string filename) = 0; // load index from file
  virtual int dump(const std::string filename) = 0; // dump index to file
 public:
  int inUseCount;
};
#endif //BIDDING_MODEL_SERVER_MANAGER_INDEX_H_
