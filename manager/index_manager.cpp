//
//

#include "index_manager.h"
#include <iostream>
#include <string>

#include "../include/common.h"
#include "../include/yaml-cpp/yaml.h"

IndexManager* IndexManager::instance_ = nullptr;
pthread_mutex_t IndexManager::mutex_ = PTHREAD_MUTEX_INITIALIZER;

IndexManager::IndexManager() : indexMap_{}, inServeIndex_{-1}, indexCount_(2) {
}

int IndexManager::init(std::string config_filename) {

  auto logger = getLogger();
  logger->info("index manager init");

  YAML::Node config = YAML::LoadFile(config_filename);
  indexFolderPath_ = config["index_folder_path"].as<std::string>("");
  if (indexFolderPath_ == "") {
    logger->error("index manager skip, invalid path");
    return -1;
  }

  for (int i = 0; i < indexCount_; i++) {
    indexMap_[i][0] = createIndex(i);
    if (indexMap_[i][0] != nullptr) { // init each index
      auto indexConfigFile = getIndexConfigFile(indexFolderPath_, i);
      if (indexMap_[i][0]->load(indexConfigFile) < 0) {
        logger->error("index:%d load file fails", i);
        return -1;
      }
      inServeIndex_[i] = 0; // make index 0 ready for serve
    } else {
      inServeIndex_[i] = -1;
      return -1;
    }
  }
  // create update thread
  pthread_create(&indexUpdateTid_, nullptr, updateThread, this);
  // create delete thread
  pthread_create(&indexDeleteTid_, nullptr, deleteThread, this);
  return 0;
}

std::string IndexManager::getIndexConfigFile(const std::string folderPath, const int32_t num) {
  return folderPath + "/" + "index_config_" + std::to_string(num);
}

BaseIndex* IndexManager::createIndex(const int32_t num) {
  if (num == kAdIndex) {
    return new (std::nothrow) AdIndex();
  } else if (num == kCampaignIndex) {
    return new (std::nothrow) CampaignIndex();
  } else {
    throw std::invalid_argument("Invalid value of input");
  }
}

BaseIndex* IndexManager::getIndex(const int32_t num) {
  if (num < 0 || num >= kMaxIndexNum) {
    return nullptr;
  }
  _lock();
  BaseIndex* index = indexMap_[num][ inServeIndex_[num] ];
  index->inUseCount++;
  _unlock();
  return index;
}

void IndexManager::releaseIndex(BaseIndex* index) {
  if (!index) {
    return;
  }
  _lock();
  index->inUseCount--;
  _unlock();
}

void IndexManager::_lock() {
  pthread_mutex_lock(&mutex_);
}

void IndexManager::_unlock() {
  pthread_mutex_unlock(&mutex_);
}

int IndexManager::updateIndex() {
  std::cout << "update index " << std::endl;
  for (int i = 0; i < indexCount_; i++) {
    auto index = createIndex(i);
    if (index != nullptr) { // init each index
      auto indexConfigFile = getIndexConfigFile(indexFolderPath_, i);
      if (index->load(indexConfigFile) < 0) {
        delete index;
        continue;
      }
      if (indexMap_[i][1 - inServeIndex_[i]] == nullptr) {
        _lock();
        indexMap_[i][1 - inServeIndex_[i]] = index;
        inServeIndex_[i] = 1 - inServeIndex_[i];
        _unlock();
        continue;
      }
    }
  }
  return 0;
}

int IndexManager::deleteIndex() {
  std::cout << "delete index " << std::endl;
  for (int32_t i = 0; i < indexCount_; i++)
  {
    if (inServeIndex_[i] == -1)
      continue;
    if (indexMap_[i][ 1 - inServeIndex_[i] ] != nullptr)
    {
      BaseIndex* index = nullptr;
      _lock();
      if (indexMap_[i][ 1 - inServeIndex_[i] ] != nullptr)
      {
        index = indexMap_[i][ 1 - inServeIndex_[i] ];
        indexMap_[i][ 1 - inServeIndex_[i] ] = nullptr;
      }
      _unlock();
      while (index != nullptr)
      {
        if (index->inUseCount <= 0)
        {
          delete index;
          break;
        }
        sleep(1);
      }
    }
  }
  return 0;
}