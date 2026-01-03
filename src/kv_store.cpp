#include "../include/kv_store.hpp"
#include <iostream>

KVStore::KVStore(const std::string &id) : id(id) {
  std::cout << "KVStore created with id: " << id << std::endl;
}

bool KVStore::put(const std::string &key, const json &value) {
  store[key] = value;
  std::cout << "Stored key: " << key << " with value: " << value.dump()
            << std::endl;
  return true;
}

json KVStore::get(const std::string &key) {
  if (store.count(key)) {
    std::cout << "Retrieved key: " << key
              << " with value: " << store[key].dump() << std::endl;
    return store[key];
  } else {
    std::cout << "Key not found: " << key << std::endl;
    return json::object();
  }
}

bool KVStore::remove(const std::string &key) {
  if (store.count(key)) {
    store.erase(key);
    std::cout << "Removed key: " << key << std::endl;
    return true;
  } else {
    std::cout << "Key not found: " << key << std::endl;
    return false;
  }
}

std::map<std::string, json> KVStore::get_all() { return store; }

void KVStore::print_store() {
  for (const auto &pair : store) {
    std::cout << "Key: " << pair.first << ", Value: " << pair.second.dump()
              << std::endl;
  }
}
