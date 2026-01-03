#include "kv_store.hpp"
#include "network.hpp"
#include <arpa/inet.h>
#include <chrono>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

void print_section(const std::string &title) {
  std::cout << "\n" << std::string(60, '=') << "\n";
  std::cout << " " << title << "\n";
  std::cout << std::string(60, '=') << "\n";
}

int main() {
  // Create KV store
  KVStore store("node_1", "logs/node_1_operations.log");
  // Create network node
  NetworkNode network("node_1", "127.0.0.1", 5000, &store);

  std::cout << "\n--- TEST 1: Start Server ---" << std::endl;
  network.start_server();

  std::cout << "\n--- TEST 2: Send Test Message---" << std::endl;

  try {
    // Create client socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Preapre address to connect to
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5000);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    // Try to connect
    if (connect(client_socket, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
      std::cout << "Connection successful" << std::endl;

      // Create a json message
      json msg;
      msg["sender"] = "test_client";
      msg["type"] = "PUT";
      msg["key"] = "robot_a";
      msg["value"] = json::object();
      msg["value"]["position"] = {10, 20};

      std::string msg_str = msg.dump();

      if (send(client_socket, msg_str.c_str(), msg_str.length(), 0) > 0) {
        std::cout << "Sent test message!" << std::endl;
        std::cout << "Message: " << msg_str << std::endl;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
    } else {
      std::cerr << "Failed to connect" << std::endl;
    }

    close(client_socket);
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  std::cout << "\n--- TEST 3: Server Running ---" << std::endl;
  std::cout << "Server is running in background thread" << std::endl;
  std::cout << "Main thread can continue working!" << std::endl;

  std::this_thread::sleep_for(std::chrono::seconds(1));

  std::cout << "\n--- TEST 4: Stop Server ---" << std::endl;
  network.stop_server();

  std::cout << "\n--- TEST 5: Server Stopped ---" << std::endl;
  std::cout << "Server stopped successfully" << std::endl;
}