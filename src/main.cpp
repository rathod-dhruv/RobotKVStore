#include "kv_store.hpp"
#include <iostream>

int main() {
  std::cout << "Basic Memory KV Store";

  KVStore store("robo_1");

  // PUT Operation
  std::cout << "Test 1: PUT Operations" << std::endl;

  json robot_a_info;
  robot_a_info["position"] = {10, 20};
  robot_a_info["status"] = "ACTIVE";
  robot_a_info["battery"] = "100";

  store.put("robot_a", robot_a_info);

  json robot_b_info;
  robot_b_info["position"] = {50, 30};
  robot_b_info["status"] = "IDLE";
  robot_b_info["battery"] = "50";

  store.put("robot_b", robot_b_info);

  // GET Operation
  std::cout << "Test 2: GET Operations" << std::endl;

  json retrieved_info = store.get("robot_a");
  std::cout << "Retrieved info: " << retrieved_info.dump() << std::endl;

  retrieved_info = store.get("robot_b");
  std::cout << "Retrieved info: " << retrieved_info.dump() << std::endl;

  // PRINT Operation
  std::cout << "Test 3: PRINT Operations" << std::endl;
  store.print_store();

  // REMOVE Operation
  std::cout << "Test 4: REMOVE Operations" << std::endl;
  store.remove("robot_b");

  // GET ALL Operation
  std::cout << "Test 5: GET ALL Operations" << std::endl;
  store.get_all();

  // PRINT Operation
  std::cout << "Test 6: PRINT Operations" << std::endl;
  store.print_store();
}