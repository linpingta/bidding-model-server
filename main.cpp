#include <iostream>
#include <string>


#include "include/common.h"
#include "manager/bid_server_manager.h"

int main() {
  std::cout << "Hello, World!" << std::endl;

  // init log
  initLogger();

  auto logger = getLogger();
  logger->info("init service");

  BidServerManager manager;
  if (manager.init("config.yaml") < 0) {
    logger->error("init bid service fail");
  }

  manager.run();

  logger->info("shutdown service");

  gLogger->shutdown();
  return 0;
}