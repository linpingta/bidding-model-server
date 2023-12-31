//
//

#include <fstream>
#include <iostream>
#include <sstream>
#include "ad_index.h"


// this is just an example, use binary file for prd usage
int AdIndex::load(const std::string filename) {
  std::ifstream inputFile(filename);
  if (!inputFile.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return -1;
  }

  std::string line;
  while (std::getline(inputFile, line)) {
    std::istringstream iss(line);
    int32_t id;
    int64_t budget;
    int32_t campaign_id;
    int64_t price;

    if (!(iss >> id >> budget >> campaign_id >> price)) {
      std::cerr << "Error reading data from file: " << filename << std::endl;
      return -1;  // Return an error code
    }
    std::cout << "ad: " << id << ", " << budget << ", " << campaign_id << ", " << price << std::endl;

    Ad ad(id, budget, price, campaign_id);
    adMap.insert(std::make_pair(id, ad));
  }

  inputFile.close();
  return 0;
}

int AdIndex::dump(const std::string filename) {
  std::ofstream outputFile(filename);
  if (!outputFile.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return -1;
  }

  for (const auto& entry : adMap) {
    outputFile << entry.first << " " << entry.second.budget << std::endl;
  }

  outputFile.close();
  return 0;
}