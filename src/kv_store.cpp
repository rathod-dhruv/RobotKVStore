#include "../include/kv_store.hpp"

#include <iostream>

KVStore::KVStore(const std::string &id, const std::string &log_file_path)
    : id(id), log_file_path(log_file_path) {
  std::cout << "KVStore created with id: " << id << std::endl;
  recover_log_file();
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
  std::lock_guard<std::mutex> lock(log_mutex);

  time_t now = time(nullptr);
  double timestamp = static_cast<double>(now);

  std::ofstream log_stream(log_file_path, std::ios::app);
  if (log_stream.is_open()) {
    // SIMPLER FORMAT: timestamp|operation|key|value
    log_stream << timestamp << "|" << operation << "|" << key << "|"
               << value.dump() << "\n";
    log_stream.close();
  } else {
    std::cout << "Failed to open log file: " << log_file_path << std::endl;
  }
}

void KVStore::recover_log_file() {
  std::cout << "[" << id << "] " << "Recovering log file: " << log_file_path
            << std::endl;

  std::ifstream log_stream(log_file_path);
  if (!log_stream.is_open()) {
    std::cout << "[" << id << "] "
              << "No log file found. Starting fresh." << std::endl;
    return;
  }

  std::lock_guard<std::mutex> lock(log_mutex);

  std::string line;
  int count = 0;

  while (std::getline(log_stream, line)) {
    if (line.empty())
      continue;

    std::stringstream ss(line);
    std::string timestamp, operation, key, value;

    // Parse: timestamp|operation|key|value
    std::getline(ss, timestamp, '|'); // Gets timestamp
    std::getline(ss, operation, '|'); // Gets operation
    std::getline(ss, key, '|');       // Gets key
    std::getline(ss, value, '|');     // Gets value (rest of line)

    if (operation == "PUT") {
      try {
        json value_json = json::parse(value);
        store[key] = value_json;
        count++;
      } catch (const std::exception &e) {
        std::cout << "Failed to parse value: " << value << std::endl;
      }
    } else if (operation == "REMOVE") {
      store.erase(key);
      count++;
    }
    // Skip GET and GET_ALL operations (they don't change state)
  }

  log_stream.close();

  std::cout << "[" << id << "] " << "Recovered " << count
            << " operations from log file: " << log_file_path << std::endl;
}