//
//

#include "common.h"

SimpleLogger* gLogger = nullptr;

void initLogger() {
  gLogger = &SimpleLogger::getInstance();
}

SimpleLogger* getLogger() {
  return gLogger;
}