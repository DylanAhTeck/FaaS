#include "database.h"

namespace dylanwarble
{

bool Database::Put(std::string key, std::string value)
{
  // Iterator pointing to key-value pair
  auto it = umap_.find(key);

  // If found, add to existing vector
  if (it != umap_.end())
  {
    umap_[key].push_back(value);
  } // Else initialize new vector with value as initial element
  else
  {
    umap_.insert(std::make_pair(key, std::vector<std::string>(1, value)));
  }

  return true;
}

std::vector<std::string> Database::Get(std::string key)
{
  // Iterator pointing to key-value pair
  std::unordered_map<std::string, std::vector<std::string>>::const_iterator
      got = umap_.find(key);

  // Key not found, return empty vector
  if (got == umap_.end())
    return std::vector<std::string>();

  // Key found, return second element
  else
    return got->second;
}
bool Database::Remove(std::string key)
{
  // Iterator pointing to key-value pair
  std::unordered_map<std::string, std::vector<std::string>>::const_iterator
      got = umap_.find(key);

  // Erase the element pointed by iterator
  if (got != umap_.end())
    umap_.erase(got);

  return true;
}

} // namespace dylanwarble

int main() { return 1; }
