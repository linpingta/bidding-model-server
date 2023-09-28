//
//

#include <fstream>
#include <iostream>
#include <sstream>
#include "campaign_index.h"


// this is just an example, use binary file for prd usage
int CampaignIndex::load(const std::string filename) {
  std::ifstream inputFile(filename);
  if (!inputFile.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return -1;
  }

  std::string line;
  while (std::getline(inputFile, line)) {
    std::istringstream iss(line);
    int32_t id;
    int32_t start_time;
    int32_t end_time;

    if (!(iss >> id >> start_time >> end_time)) {
      std::cerr << "Error reading data from file: " << filename << std::endl;
      return -1;  // Return an error code
    }

    std::cout << "campaign: " << id << ", " << start_time << ", " << end_time << std::endl;

    Campaign campaign(id, start_time, end_time);
    campaignMap.insert(std::make_pair(id, campaign));
  }

  inputFile.close();
  return 0;
}

int CampaignIndex::dump(const std::string filename) {
  std::ofstream outputFile(filename);
  if (!outputFile.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return -1;
  }

  for (const auto &entry : campaignMap) {
    outputFile << entry.first << " " << entry.second.start_time << " " << entry.second.end_time << std::endl;
  }

  outputFile.close();
  return 0;
}