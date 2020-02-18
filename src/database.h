#include <unordered_map>
#include <vector>

class Database
{
public:
    void put(std::string key, std::string value);
    std::vector<std::string> get(std::string key);
    void remove(std::string key);

private:
    std::unordered_map<std::string, std::vector<std::string>> umap_;
};
