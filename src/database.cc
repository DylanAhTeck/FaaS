// Copyright 2020 Dylan Ah Teck

#include "database.h"  // NOLINT

namespace dylanwarble {

// Stores key-value pair, returns true if successful
bool Database::Put(std::string key, std::string value) {
  // Locks mutex - only current thread can access map
  const std::lock_guard<std::mutex> lock(mut);

  // Iterator pointing to key-value pair
  auto it = umap_.find(key);

  // If found, add to existing vector
  // Else initialize new vector with value as initial element
  if (it != umap_.end()) {
    umap_[key].push_back(value);
  } else {
    umap_.insert(std::make_pair(key, std::vector<std::string>(1, value)));
  }
  // Check that umap now has key
  auto check = umap_.find(key);
  if (check == umap_.end()) {
    return false;
  }

  return true;
}

// Retrieves value(s) of key if key exists, else returns empty vector
std::vector<std::string> Database::Get(std::string key) {
  // Locks mutex - only current thread can access map
  const std::lock_guard<std::mutex> lock(mut);

  // Stores return values
  std::vector<std::string> values;

  // Iterator pointing to key-value pair
  std::unordered_map<std::string, std::vector<std::string>>::const_iterator
      got = umap_.find(key);

  // Key not found, return empty vector
  // Else if key found, allocate values to second element of pairing and return
  if (got == umap_.end()) {
    return values;
  } else {
    values = got->second;
  }

  return values;
}

// Removes all values of key, returns true
bool Database::Remove(std::string key) {
  // Locks mutex - only current thread can access map
  const std::lock_guard<std::mutex> lock(mut);

  // Iterator pointing to key-value pair
  std::unordered_map<std::string, std::vector<std::string>>::const_iterator
      got = umap_.find(key);

  // Erase the element pointed by iterator if key value pair exists
  if (got != umap_.end()) {
    umap_.erase(got);
  }

  // Check that umap does not have key
  auto check = umap_.find(key);
  if (check != umap_.end()) {
    return false;
  }

  return true;
}

}  // namespace dylanwarble
