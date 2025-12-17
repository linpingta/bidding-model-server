//
//

#ifndef BIDDING_MODEL_SERVER_MANAGER_INDEX_MANAGER_H_
#define BIDDING_MODEL_SERVER_MANAGER_INDEX_MANAGER_H_

#include <iostream>
#include <mutex>
#include <thread>
#include <chrono> // For sleep function

// Windows thread support
#ifdef _WIN32
#include <windows.h>
typedef HANDLE pthread_t;
typedef CRITICAL_SECTION pthread_mutex_t;
#define PTHREAD_MUTEX_INITIALIZER {0}
#define pthread_mutex_init(mutex, attr) InitializeCriticalSection(mutex)
#define pthread_mutex_lock(mutex) EnterCriticalSection(mutex)
#define pthread_mutex_unlock(mutex) LeaveCriticalSection(mutex)
#define pthread_mutex_destroy(mutex) DeleteCriticalSection(mutex)
#define pthread_create(thread, attr, start_routine, arg) \
    (*thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start_routine, arg, 0, NULL)) != NULL ? 0 : -1
#define pthread_join(thread, retval) WaitForSingleObject(thread, INFINITE)
#define sleep(seconds) Sleep(seconds * 1000)
#else
#include <pthread.h>
#include <unistd.h>
#endif

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
