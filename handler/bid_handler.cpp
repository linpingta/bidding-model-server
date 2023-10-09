//
//

#include "bid_handler.h"

void BidHandler::Init() {
  index_manager_ = IndexManager::getInstance();
  logger_ = getLogger();
}

void BidHandler::Bid(const BidReqData& bidReqData, BidRspData& bidRspData) {
  if (!reqFilter(bidReqData)) {
    logger_->info("invalid req, filter");
    return;
  }

  auto campaignMap = campaignFilter(bidReqData);
  if (campaignMap.empty()) {
    logger_->info("no campaign after campaign check");
    return;
  }

  auto adMap = adFilter(bidReqData, campaignMap);
  if (adMap.empty()) {
    logger_->info("no ad after ad check");
    return;
  }
}

bool BidHandler::reqFilter(const BidReqData &bidReqData) {
  auto campaignIndex = index_manager_->getIndex(kCampaignIndex);
  if (campaignIndex == nullptr) {
    return false;
  }
  index_manager_->releaseIndex(campaignIndex);

  auto adIndex = index_manager_->getIndex(kAdIndex);
  if (adIndex == nullptr) {
    return false;
  }
  index_manager_->releaseIndex(adIndex);

  // random filter if needed

  return true;
}

std::unordered_map<int32_t, Campaign> BidHandler::campaignFilter(const BidReqData& bidReqData) {
  std::unordered_map<int32_t, Campaign> filteredMap;

  auto index = index_manager_->getIndex(kCampaignIndex);
  auto campaignIndex = dynamic_cast<CampaignIndex*>(index);
  auto campaignMap = campaignIndex->getCampaignMap();

  for (const auto& item: campaignMap) {
    // filter campaign by req
    // TODO

    filteredMap[item.first] = item.second;
  }

  return filteredMap;
}

std::unordered_map<int32_t, Ad> BidHandler::adFilter(const BidReqData& bidReqData, const std::unordered_map<int32_t, Campaign>& campaignMap) {
  std::unordered_map<int32_t, Ad> filteredMap;

  auto index = index_manager_->getIndex(kAdIndex);
  auto adIndex = dynamic_cast<AdIndex*>(index);
  auto adMap = adIndex->getAdMap();

  for (const auto& item: adMap) {
    // filter ad by campaign
    if (campaignMap.find(item.second.campaign_id) == campaignMap.end()) {
      continue;
    }

    // filter ad
    // TODO

    filteredMap[item.first] = item.second;
  }

  return filteredMap;
}

bool BidHandler::rank() {
  return false;
}

std::vector<Ad> BidHandler::selectAds() {
  std::vector<Ad> ads;
  return ads;
}

BidRspData BidHandler::fillResponse(const BidReqData& bidReqData, std::vector<Ad> rspAdList) {
  return BidRspData();
}


