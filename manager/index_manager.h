//
//

#ifndef BIDDING_MODEL_SERVER_MANAGER_INDEX_MANAGER_H_
#define BIDDING_MODEL_SERVER_MANAGER_INDEX_MANAGER_H_

#include <iostream>
#include <pthread.h>
#include <mutex>
#include <unistd.h> // For sleep function

#include "index.h"
#include "ad_index.h"
#include "campaign_index.h"
#include "const.h"

class IndexManager {
 public:
  static IndexManager* getInstance() {
    pthread_mutex_lock(&mutex_);
    if (!instance_) {
      instance_ = new IndexManager(); // create instance
    }
    pthread_mutex_unlock(&mutex_);
    return instance_;
  }
  static void* updateThread(void* obj){
    IndexManager* indexManager = static_cast<IndexManager*>(obj);
    while (true) {
      indexManager->updateIndex();
      sleep(10);
    }
  }
  static void* deleteThread(void* obj) {
    IndexManager *indexManager = static_cast<IndexManager *>(obj);
    while (true) {
      indexManager->deleteIndex();
      sleep(10);
    }
  }

  void releaseIndex(BaseIndex* index);
  int init(std::string config_filename);
  BaseIndex* getIndex(const int32_t num);
  BaseIndex* createIndex(const int32_t num);
 public:
  int updateIndex();
  int deleteIndex();
 private:
  IndexManager();
 private:
  void _lock();
  void _unlock();
  std::string getIndexConfigFile(const std::string folderPath, const int32_t num);
 private:
  static IndexManager* instance_;
  static pthread_mutex_t mutex_;
 private:
  BaseIndex* indexMap_[kMaxIndexNum][2];
  int inServeIndex_[kMaxIndexNum];
  pthread_t indexUpdateTid_;
  pthread_t indexDeleteTid_;
  int indexCount_;
  std::string indexFolderPath_;
};

#endif //BIDDING_MODEL_SERVER_MANAGER_INDEX_MANAGER_H_
