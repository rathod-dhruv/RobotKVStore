#ifndef KV_STORE_HPP
#define KV_STORE_HPP

#include <ctime>
#include <fstream>
#include <map>
#include <mutex>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class KVStore {
private:
  std::map<std::string, json> store;
  std::string id;
  std::string log_file_path;

  std::mutex store_mutex;

  std::mutex log_mutex;

  void log_operation(const std::string &operation, const std::string &key,
                     const json &value);

public:
  KVStore(const std::string &id, const std::string &log_file_path);

  bool put(const std::string &key, const json &value);

  json get(const std::string &key);

  bool remove(const std::string &key);

  std::map<std::string, json> get_all();

  void print_store();
};

#endif // KV_STORE_HPP
