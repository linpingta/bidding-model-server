//
//

#ifndef BIDDING_MODEL_SERVER_COMMON_CONST_H_
#define BIDDING_MODEL_SERVER_COMMON_CONST_H_

#include "log4cpp/Category.hh"

#define MAX_BUFFER_LEN 256

// logger
extern log4cpp::Category* gLogger;

void initLogger();
log4cpp::Category* getLogger();

#endif //BIDDING_MODEL_SERVER_COMMON_CONST_H_
