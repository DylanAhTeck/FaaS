#include <glog/logging.h>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace dylanwarble {

// Database data-structure to implement thread-safe
// storage with no prior knowledge of use-cases
class Database {
 public:
  // Stores key-value pair
  bool Put(std::string key, std::string value);
  // Retrieves value(s) of key
  std::vector<std::string> Get(std::string key);
  // Removes all values of key
  bool Remove(std::string key);

 private:
  // Internal hash_map used for now
  std::unordered_map<std::string, std::vector<std::string>> umap_;
  std::mutex mut;
};

}  // namespace dylanwarble
