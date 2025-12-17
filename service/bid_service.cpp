#include "bid_service.h"
#include <iostream>
#include <string>

BidService::BidService() {
    bid_handler_ = std::make_shared<BidHandler>();
    bid_handler_->Init();
}

Status BidService::bidAd(ServerContext* context, const BidRequest* request, BidResponse* response) {
    BidReqData bidReqData;
    BidRspData bidRspData;

    // Parse request
    bidReqData.req_id = request->id();
    bidReqData.user_id = request->user().id();
    // Convert device object to string representation
    std::string device_info = request->device().os() + ":" + request->device().model();
    bidReqData.device = device_info;
    
    // Set default values since these fields don't exist in proto
    bidReqData.adx_id = 0;
    bidReqData.media_id = 0;
    if (!request->site().id().empty()) {
        bidReqData.media_id = std::stoi(request->site().id());
    }
    bidReqData.ad_slot_id = 0;
    bidReqData.ad_slot_type = 0;
    if (request->impressions_size() > 0) {
        const auto& impression = request->impressions(0);
        if (!impression.id().empty()) {
            bidReqData.ad_slot_id = std::stoi(impression.id());
        }
        if (!impression.ad_type().empty()) {
            bidReqData.ad_slot_type = std::stoi(impression.ad_type());
        }
        bidReqData.price_floor = impression.bid_floor();
    }

    // Handle bid
    bid_handler_->Bid(bidReqData, bidRspData);

    // Build response
    response->set_id(bidRspData.req_id);
    auto* seat_bid = response->add_seat_bids();
    auto* bid = seat_bid->add_bids();
    bid->set_id(std::to_string(bidRspData.ad_id));
    bid->set_price(bidRspData.win_price);
    bid->set_adid(std::to_string(bidRspData.ad_id));

    return Status::OK;
}

void BidService::bidAdTest() {
    BidReqData bidReqData;
    BidRspData bidRspData;

    // Set test data
    bidReqData.req_id = "test_req_001";
    bidReqData.user_id = "test_user_001";
    bidReqData.device = "android";
    bidReqData.adx_id = 1;
    bidReqData.media_id = 1001;
    bidReqData.ad_slot_id = 2001;
    bidReqData.ad_slot_type = 1;
    bidReqData.price_floor = 0.5;

    // Handle bid
    bid_handler_->Bid(bidReqData, bidRspData);

    // Print result
    std::cout << "Test Bid Result:" << std::endl;
    std::cout << "Req ID: " << bidRspData.req_id << std::endl;
    std::cout << "User ID: " << bidRspData.user_id << std::endl;
    std::cout << "Win Price: " << bidRspData.win_price << std::endl;
    std::cout << "Ad ID: " << bidRspData.ad_id << std::endl;
}