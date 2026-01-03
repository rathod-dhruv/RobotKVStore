#include "../include/network.hpp"
#include "../include/kv_store.hpp"
#include <arpa/inet.h>
#include <chrono>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <nlohmann/json.hpp>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

using json = nlohmann::json;

NetworkNode::NetworkNode(const std::string &id, const std::string &host,
                         int port, KVStore *kv_store)
    : id(id), host(host), port(port), kv_store(kv_store) {
  std::cout << "NetwrokNode created with id: " << id << ", host: " << host
            << ", port: " << port << std::endl;
}
NetworkNode::~NetworkNode() {
  stop_server();
  std::cout << "NetwrokNode destroyed with id: " << id << std::endl;
}

void NetworkNode::start_server() {

  std::lock_guard<std::mutex> lock(running_mutex);
  if (is_running) {
    std::cout << "[" << id << "] " << "Server is already running" << std::endl;
    return;
  }
  is_running = true;
  server_thread = std::make_unique<std::thread>(&NetworkNode::run_server, this);

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  std::cout << "[" << id << "] " << "Server started on " << host << ":" << port
            << std::endl;
}
void NetworkNode::stop_server() {
  {
    std::lock_guard<std::mutex> lock(running_mutex);
    is_running = false;
  }

  if (server_socket != -1) {
    close(server_socket);
    server_socket = -1;
  }
  if (server_thread && server_thread->joinable()) {
    server_thread->join();
  }

  std::cout << "[" << id << "] " << "Server stopped on " << host << ":" << port
            << std::endl;
}

void NetworkNode::run_server() {

  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    std::cerr << "Error creating socket" << std::endl;
    return;
  }

  int opt = 1;
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) <
      0) {
    std::cerr << "[" << id << "] " << "Error setting socket options"
              << std::endl;
    close(server_socket);
    return;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr);

  if (bind(server_socket, (struct sockaddr *)&server_addr,
           sizeof(server_addr)) < 0) {
    std::cerr << "[" << id << "] " << "Error binding socket" << std::endl;
    close(server_socket);
    return;
  }

  listen(server_socket, 5);
  std::cout << "[" << id << "] " << "Server listening on " << host << ":"
            << port << std::endl;

  while (is_running) {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_socket = accept(server_socket, (struct sockaddr *)&client_addr,
                               &client_addr_len);
    if (client_socket < 0) {
      if (is_running)
        std::cerr << "[" << id << "] " << "Error accepting connection"
                  << std::endl;
      continue;
    }
    std::thread client_thread(&NetworkNode::handle_client, this, client_socket);
    client_thread.detach();
  }

  close(server_socket);
}

void NetworkNode::handle_client(int client_socket) {

  char buffer[4096] = {0};
  try {
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received > 0) {
      buffer[bytes_received] = '\0';
      std::string message(buffer);

      try {
        json msg = json::parse(message);

        std::cout << "[" << id << "] " << "Received message: from " << msg["id"]
                  << " : " << msg.dump() << std::endl;
      } catch (const std::exception &e) {
        std::cerr << "[" << id << "] "
                  << "JSON Error parsing message: " << e.what() << std::endl;
      }
    }
  } catch (const std::exception &e) {
    std::cerr << "[" << id << "] " << "Error handling client: " << e.what()
              << std::endl;
  }
  close(client_socket);
}