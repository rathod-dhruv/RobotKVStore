#include "../include/kv_store.hpp"

#include <iostream>

KVStore::KVStore(const std::string &id, const std::string &log_file_path)
    : id(id), log_file_path(log_file_path) {
  std::cout << "KVStore created with id: " << id << std::endl;
}

bool KVStore::put(const std::string &key, const json &value) {

  {
    std::lock_guard<std::mutex> lock(store_mutex); // LOCK
    store[key] = value;
  } // UNLOCK

  // File Write
  log_operation("PUT", key, value);

  std::cout << "Stored key: " << key << " with value: " << value.dump()
            << std::endl;
  return true;
}

json KVStore::get(const std::string &key) {
  std::lock_guard<std::mutex> lock(store_mutex); // LOCK
  if (store.count(key)) {
    std::cout << "Retrieved key: " << key
              << " with value: " << store[key].dump() << std::endl;
    // File Write
    log_operation("GET", key, store[key]);
    return store[key];
  } else {
    std::cout << "Key not found: " << key << std::endl;
    return json::object();
  }
  // UNLOCK
}

bool KVStore::remove(const std::string &key) {

  if (store.count(key)) {
    std::lock_guard<std::mutex> lock(store_mutex); // LOCK
    // File Write
    log_operation("REMOVE", key, store[key]);
    store.erase(key);

    std::cout << "Removed key: " << key << std::endl;
    return true;
  } // UNLOCK

  else {
    std::cout << "Key not found: " << key << std::endl;
    return false;
  }
}

std::map<std::string, json> KVStore::get_all() {
  std::lock_guard<std::mutex> lock(store_mutex); // LOCK
  // File Write
  log_operation("GET_ALL", "", store);
  return store;
}

void KVStore::print_store() {
  std::lock_guard<std::mutex> lock(store_mutex); // LOCK
  for (const auto &pair : store) {
    std::cout << "Key: " << pair.first << ", Value: " << pair.second.dump()
              << std::endl;
  }
}

void KVStore::log_operation(const std::string &operation,
                            const std::string &key, const json &value) {
  std::lock_guard<std::mutex> lock(log_mutex); // LOCK

  // Get current time
  time_t now = time(nullptr);
  double timestamp = static_cast<double>(now);

  // Log operation
  std::ofstream log_stream(log_file_path, std::ios::app);
  if (log_stream.is_open()) {
    log_stream << "Timestamp: " << timestamp << "|"
               << "Operation: " << operation << "|"
               << "Key: " << key << "|"
               << "Value: " << value.dump() << "\n";
    log_stream.close();
  } else {
    std::cout << "Failed to open log file: " << log_file_path << std::endl;
  }
}
