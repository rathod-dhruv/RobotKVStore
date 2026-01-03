#ifndef KV_STORE_HPP
#define KV_STORE_HPP

#include <nlohmann/json.hpp>

#include <map>
#include <string>

using json = nlohmann::json;

class KVStore {
private:
  std::map<std::string, json> store;
  std::string id;

public:
  KVStore(const std::string &id);

  bool put(const std::string &key, const json &value);

  json get(const std::string &key);

  bool remove(const std::string &key);

  std::map<std::string, json> get_all();

  void print_store();
};

#endif // KV_STORE_HPP
