//
//

#ifndef BIDDING_MODEL_SERVER_COMMON_CONST_H_
#define BIDDING_MODEL_SERVER_COMMON_CONST_H_

#include "simple_logger.h"
#include <string>
#include <vector>

// logger
extern SimpleLogger* gLogger;

void initLogger();
SimpleLogger* getLogger();


struct AdInfo {
  std::string ad_id; // 广告 ID
  double bid_price; // 出价
};

// Fill bid request
class BidReqData {
 public:
  BidReqData() {};

  // transfer rtb protocol into bid req info
  void fillBidReqInfo(const std::string& requestId, const std::string& userId,
      const std::string& exchangeId, const std::string& publisherId,
      int32_t cityId, const std::string appId, double bidFloor) {
    request_id = requestId;
    user_id = userId;
    exchange_id = exchangeId;
    bid_floor = bidFloor;
    city_id = cityId;
    app_id = appId;
  }

  std::string getRequestId() const {
    return request_id;
  }
  std::string getUserId() const {
    return user_id;
  }
  std::string getExchangeId() const {
    return exchange_id;
  }
  double getBidFloor() const {
    return bid_floor;
  }
  std::string getAppId() const {
    return app_id;
  }
  int32_t getCityId() const {
    return city_id;
  }

 private:
  std::string request_id; // req id
  std::string user_id;    // user id
  std::string exchange_id; // exchange id
  double bid_floor; // bid floor
  int32_t city_id;  // city info
  std::string app_id; // app id
};

// 定义 BidResponse 结构体
struct BidRspData {
  std::string request_id; // 请求 ID，与请求相关联
  std::vector<AdInfo> ads; // 响应的广告列表

  // 添加广告信息到响应
  void addAd(const std::string& adId, double bidPrice) {
    AdInfo ad;
    ad.ad_id = adId;
    ad.bid_price = bidPrice;
    ads.emplace_back(ad);
  }
};

#endif //BIDDING_MODEL_SERVER_COMMON_CONST_H_