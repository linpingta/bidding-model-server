//
//

#include "common.h"

#include "log4cpp/Category.hh"
#include "log4cpp/Priority.hh"
#include "log4cpp/FixedContextCategory.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"

log4cpp::Category* gLogger = nullptr;

void initLogger() {
  log4cpp::Appender* appender =
      new log4cpp::OstreamAppender("appender", &std::cout);

  log4cpp::Layout* layout = new log4cpp::BasicLayout();
  appender->setLayout(layout);

  log4cpp::Category& root = log4cpp::Category::getRoot();
  root.addAppender(appender);
  root.setPriority(log4cpp::Priority::INFO);

  gLogger = &log4cpp::Category::getRoot();
}

log4cpp::Category* getLogger() {
  return gLogger;
}