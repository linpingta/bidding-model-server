//
//

#ifndef BIDDING_MODEL_SERVER_MANAGER_INDEX_MANAGER_H_
#define BIDDING_MODEL_SERVER_MANAGER_INDEX_MANAGER_H_

#include <iostream>
#include <pthread.h>
#include "index.h"
#include "const.h"

class IndexManager {
 public:
  static IndexManager* getInstance();
  int init(std::string config_filename);
  Index* getIndex(const int32_t num);
  void releaseIndex(Index* index);
 private:
  IndexManager();
  static void updateThread();
  static void deleteThread();
 private:
  void _lock();
  void _unlock();
  int _updateIndex();
  int _deleteIndex();
 private:
  static IndexManager* instance_;
  static pthread_mutex_t* mutex_;
  Index* indexMap_[kMaxIndexNum][2];
  int inServeIndex_[kMaxIndexNum];
};

#endif //BIDDING_MODEL_SERVER_MANAGER_INDEX_MANAGER_H_
