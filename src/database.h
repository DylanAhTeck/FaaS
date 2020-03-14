// Copyright 2020 Dylan Ah Teck

#ifndef SRC_DATABASE_H_
#define SRC_DATABASE_H_

#include <glog/logging.h>
#include <iostream>
#include <mutex>  //NOLINT
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#ifndef SRC_DATABASE_H_
namespace dylanwarble {

// Internal database data structure to implement thread-safe
// storage with no prior knowledge of use-cases
class Database {
 public:
  // Stores key-value pair, returns true if successful
  bool Put(std::string key, std::string value);
  // Retrieves value(s) of key if key exists, else returns empty vector
  std::vector<std::string> Get(std::string key);
  // Removes all values of key, returns true if successful
  bool Remove(std::string key);

 private:
  // Internal data storage
  std::unordered_map<std::string, std::vector<std::string>> umap_;
  // Mutex to enable concurrency
  std::mutex mut;
};

}  // namespace dylanwarble

#endif  // SRC_DATABASE_H_
