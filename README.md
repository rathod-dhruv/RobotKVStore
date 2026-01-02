# Distributed Log-Based Key-Value Store for Robot Fleets

A high-performance, fault-tolerant distributed key-value store written in C++ with append-only logging and peer-to-peer replication. Designed for robotics fleet coordination and real-time state synchronization.

## 🎯 Overview

This project implements a **distributed state management system** where multiple robots (or nodes) can share and synchronize their state without a central server. Each robot maintains its own copy of the shared state, and updates are automatically replicated to all peers.

### Key Features

- ✅ **Distributed Architecture**: No single point of failure
- ✅ **Log-Based Replication**: Durability and crash recovery via append-only logs
- ✅ **Thread-Safe Operations**: Full multithreading support with mutex protection
- ✅ **Peer-to-Peer Communication**: TCP socket-based message passing
- ✅ **Real-Time Synchronization**: Updates broadcast instantly to all peers
- ✅ **Automatic Recovery**: Lost data recovered from log files on restart
- ✅ **Cross-Platform**: Works on Linux and Windows

### Use Cases

- 🤖 **Robot Fleet Coordination**: Share positions, statuses, and sensor data
- 🏭 **Warehouse Automation**: Coordinate multiple autonomous agents
- 📡 **Distributed Systems**: Implement consensus without complex protocols
- ⚡ **Real-Time Applications**: Low-latency state synchronization

---

## 🏗️ System Architecture

```text
┌─────────────────────┐
│   Robot A Node      │
├─────────────────────┤
│  In-Memory KV Store │◄─── Fast access
├─────────────────────┤
│ Append-Only Log     │◄─── Durability
├─────────────────────┤
│ Network Layer       │◄─── Replication
└─────────────────────┘
        │
        ├─► Robot B Node
        ├─► Robot C Node
        └─► Robot D Node
```

### Components

1. **KVStore** (`include/kv_store.hpp`, `src/kv_store.cpp`)
   - In-memory key-value storage
   - Append-only log file for durability
   - Thread-safe operations with mutexes
   - Automatic recovery from logs
   - **Log File Format**: `KEY=VALUE\n` in `logs/robot_name.log`

2. **NetworkNode** (`include/network.hpp`, `src/network.cpp`)
   - TCP socket communication (Simulated in this version)
   - Multi-threaded server for receiving updates
   - Non-blocking broadcast to peers
   - Automatic peer registration

3. **RobotNode** (`include/robot_node.hpp`, `src/robot_node.cpp`)
   - High-level robot interface
   - Combines KV store and networking
   - Position tracking and status management
   - Simple, intuitive API

---

## 📋 Prerequisites

### Linux (Ubuntu/Debian)

```bash
# Update package manager
sudo apt-get update

# Install build tools
sudo apt-get install -y \
    build-essential \
    cmake \
    nlohmann-json3-dev \
    libpthread-dev

# Verify installations
g++ --version
cmake --version
```

## 🚀 Building and Running

1. **Build the project**
   ```bash
   mkdir -p build && cd build
   cmake ..
   make
   ```

2. **Run a node**
   ```bash
   # Usage: ./robot_kv_store <node_name> <port>
   ./robot_kv_store robot_A 8080
   ```

3. **Interact**
   ```text
   > put status active
   Stored status=active
   > get status
   Value: active
   ```
