//
//

#ifndef BIDDING_MODEL_SERVER_MANAGER_BID_SERVER_MANAGER_H_
#define BIDDING_MODEL_SERVER_MANAGER_BID_SERVER_MANAGER_H_

#include <iostream>
#include <string>

class BidServerManager {
 public:
  int init(std::string config_filename);
  void run();
 private:
  int port; // port for serve
  int thread_num;
  int send_timeout;
  int recv_timeout;
};

#endif //BIDDING_MODEL_SERVER_MANAGER_BID_SERVER_MANAGER_H_
