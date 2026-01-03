#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "kv_store.hpp"
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

class KVStore;

class NetworkNode {
private:
  std::string id;
  std::string host;
  int port;
  KVStore *kv_store;

  bool is_running = false;
  int server_socket = -1;
  std::unique_ptr<std::thread> server_thread;
  std::mutex running_mutex;

  void run_server();

  void handle_client(int client_socket);

public:
  NetworkNode(const std::string &id, const std::string &host, int port,
              KVStore *kv_store);

  void start_server();

  void stop_server();

  ~NetworkNode();
};

#endif // NETWORK_HPP
