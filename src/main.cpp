#include "kv_store.hpp"
#include <iostream>

void print_section(const std::string &title) {
  std::cout << "\n" << std::string(60, '=') << "\n";
  std::cout << " " << title << "\n";
  std::cout << std::string(60, '=') << "\n";
}

int main() {
  // std::cout << "Basic Memory KV Store";

  print_section("CRASH RECOVERY from log file");

  KVStore store("Store_1", "logs/store_1.log");

  // PUT Operation
  std::cout << "Adding Data to store" << std::endl;

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

  json robot_c_info;

  robot_c_info["position"] = {50, 60};
  robot_c_info["status"] = "CHARGING";
  robot_c_info["battery"] = 100;
  store.put("robot_c", robot_c_info);

  std::cout << "\nCurrent Store:" << std::endl;
  store.print_store();

  std::cout << "\n ***** SIMULATING CRASH *****" << std::endl;

  print_section("CRASH RECOVERY from log file");

  {
    KVStore store_restarted("Store_1", "logs/store_1.log");
    std::cout << "\n Store after restart(recovery from log file) " << std::endl;
    store_restarted.print_store();

    std::cout << "\n Recovered Data: " << std::endl;
    store_restarted.get_all();
  }

  print_section("Phase 3: Test deletion and recovery");

  {
    std::cout << "\nCreating store again..." << std::endl;
    KVStore store("store_1", "logs/store_1.log");

    std::cout << "\nAll data recovered:" << std::endl;
    store.print_store();

    std::cout << "\nDeleting robot_b..." << std::endl;
    store.remove("robot_b");

    std::cout << "\nStore after deletion:" << std::endl;
    store.print_store();

    std::cout << "\n*** SIMULATING CRASH ***" << std::endl;
  }

  std::cout << "\n*** RESTARTING ***" << std::endl;

  {
    KVStore store_final("store_1", "logs/store_1.log");

    std::cout << "\nFinal store (after deleting robot_b):" << std::endl;
    store_final.print_store();
  }

  // // GET Operation
  // std::cout << "Getting Data from store" << std::endl;

  // json retrieved_info = store.get("robot_a");
  // std::cout << "Retrieved info: " << retrieved_info.dump() << std::endl;

  // retrieved_info = store.get("robot_b");
  // std::cout << "Retrieved info: " << retrieved_info.dump() << std::endl;

  // // PRINT Operation
  // std::cout << "Test 3: PRINT Operations" << std::endl;
  // store.print_store();

  // // REMOVE Operation
  // std::cout << "Test 4: REMOVE Operations" << std::endl;
  // store.remove("robot_b");

  // // GET ALL Operation
  // std::cout << "Test 5: GET ALL Operations" << std::endl;
  // store.get_all();

  // // PRINT Operation
  // std::cout << "Test 6: PRINT Operations" << std::endl;
  // store.print_store();

  // Log File Content
  print_section("LOG File Content");
  std::ifstream log_stream("logs/store_1.log");
  if (log_stream.is_open()) {
    std::string line;
    int count = 0;
    std::cout << "Log entries:" << std::endl;
    while (std::getline(log_stream, line)) {
      if (line.length() > 100) {
        std::cout << " " << line.substr(0, 100) << "..." << std::endl;
      } else {
        std::cout << " " << line << std::endl;
      }
      count++;
    }
    log_stream.close();
    std::cout << "Total log entries: " << count << std::endl;
  } else {
    std::cout << "Failed to open log file" << std::endl;
  }
}