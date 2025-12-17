//
//

#include "bid_handler.h"
#include <algorithm>
#include <ctime>

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

  // rank
  auto rankAdList = rank(adMap);

  // select and fill rsp
  int32_t rspAdNum = 1;
  bidRspData = fillResponse(bidReqData, rankAdList, rspAdNum);
}

// Comparison function to sort Ads by price in descending order
bool compareAdsByPriceDescending(const Ad& ad1, const Ad& ad2) {
  return ad1.price > ad2.price;
}

std::vector<Ad> BidHandler::rank(const std::unordered_map<int32_t, Ad>& adMap) {
  std::vector<Ad> adVector;

  for (const auto& entry : adMap) {
    adVector.push_back(entry.second);
  }

  // Sort the vector in descending order by price
  std::sort(adVector.begin(), adVector.end(), compareAdsByPriceDescending);

  return adVector;
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

  // Get current timestamp
  time_t now = time(0);
  int32_t current_time = static_cast<int32_t>(now);

  for (const auto& item: campaignMap) {
    const Campaign& campaign = item.second;
    
    // Filter 1: Campaign must be active (between start and end time)
    if (campaign.start_time > current_time || campaign.end_time < current_time) {
      logger_->info("Campaign %d filtered: not in active time range", campaign.id);
      continue;
    }
    
    // Add any additional campaign filters here based on bidReqData
    // For example, you could filter by adx_id, media_id, etc.
    
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
    const Ad& ad = item.second;
    
    // Filter 1: Ad must belong to a filtered campaign
    if (campaignMap.find(ad.campaign_id) == campaignMap.end()) {
      continue;
    }
    
    // Filter 2: Ad must have sufficient budget
    if (ad.budget <= 0) {
      logger_->info("Ad %d filtered: insufficient budget", ad.id);
      continue;
    }
    
    // Filter 3: Ad price must meet or exceed the price floor
    if (ad.price < static_cast<int64_t>(bidReqData.price_floor * 1000000)) {  // Convert to micro-units
      logger_->info("Ad %d filtered: price %ld below floor %f", ad.id, ad.price, bidReqData.price_floor);
      continue;
    }
    
    // Filter 4: Ad must match the ad slot type if specified
    if (bidReqData.ad_slot_type > 0) {
      // Assuming ad type and slot type are compatible (this is a placeholder)
      // You would need to add ad_type field to Ad class for proper filtering
      logger_->info("Ad %d: slot type matching not implemented", ad.id);
    }
    
    filteredMap[item.first] = item.second;
  }

  return filteredMap;
}

BidRspData BidHandler::fillResponse(const BidReqData& bidReqData, std::vector<Ad> candidateAdList, int32_t rspAdNum) {
  BidRspData rsp;
  rsp.req_id = bidReqData.req_id;
  rsp.user_id = bidReqData.user_id;

  if (!candidateAdList.empty()) {
    const Ad& selectedAd = candidateAdList[0];
    rsp.ad_id = selectedAd.id;
    rsp.win_price = selectedAd.price;
  }

  return rsp;
}


