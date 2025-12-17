#ifndef BIDDING_MODEL_SERVER_COMMON_CONST_H_
#define BIDDING_MODEL_SERVER_COMMON_CONST_H_

#include <string>
#include <vector>
#include <unordered_map>
#include "simple_logger.h"

struct AdInfo {
    int32_t ad_id;
    int32_t bid_price;
};

struct BidReqData {
    std::string req_id;
    std::string user_id;
    std::string device;
    int32_t adx_id;
    int32_t media_id;
    int32_t ad_slot_id;
    int32_t ad_slot_type;
    double price_floor;
};

struct BidRspData {
    std::string req_id;
    std::string user_id;
    int32_t ad_id;
    int32_t win_price;
};

class AdSelector {
public:
    AdSelector() : ads() {}
    
    void addAd(int32_t adId, int32_t bidPrice) {
        AdInfo ad;
        ad.ad_id = adId;
        ad.bid_price = bidPrice;
        ads.emplace_back(ad);
    }
    
private:
    std::vector<AdInfo> ads;
};

#endif // BIDDING_MODEL_SERVER_COMMON_CONST_H_
