# 🖥️ System Monitoring Application (C++)

## 📌 Overview

This project is a **modular system monitoring application** developed in C++.
It monitors system resources such as **CPU, RAM, and processes**, evaluates them against configurable thresholds, and produces alerts and logs accordingly.

The main goal of this project is to demonstrate:

* Multithreading
* System-level programming
* Modular architecture design
* Asynchronous logging
* Configuration-driven systems

---

## 🎯 Motivation

This project was developed to gain deeper understanding of **system-level programming concepts**, particularly:

* Thread management and synchronization
* Performance-aware system design
* Scalable and modular architecture

Rather than building a simple monitoring tool, the focus was on designing a structure similar to real-world system services.

---

## 🧠 Architecture

The system is designed with a **layered and modular architecture**:

```text
Monitors (Threads)
    ↓
MonitorManager
    ↓
AlertManager
    ↓
Logger (Async)
```

### Components:

* **IMonitor** → Interface for all monitors
* **CpuMonitor / MemoryMonitor / ProcessMonitor** → Collect system metrics
* **MonitorManager** → Manages all monitors
* **AlertManager** → Evaluates thresholds and triggers alerts
* **Logger** → Asynchronous logging system
* **Config System** → Loads and validates configuration

---

## ⚙️ Features

### 🔹 System Monitoring

* CPU usage monitoring
* RAM usage monitoring
* Process monitoring (extensible)

### 🔹 Multithreading

* Each monitor runs in its own thread
* Thread-safe data sharing using mutexes

### 🔹 Alert System

* Threshold-based alerts (Warning / Critical)
* State-based logging (prevents log spam)

### 🔹 Asynchronous Logging

* Thread-safe queue
* Dedicated worker thread
* Multiple outputs:

  * Console
  * File

### 🔹 Configuration System

* INI-based configuration (`system.ini`)
* Schema validation
* Type-safe parsing
* Fallback to default values on error

---

## 🧵 Threading Model

* Each monitor runs independently in its own thread
* Main thread periodically evaluates alerts
* Logger runs on a separate worker thread

```text
Monitor Threads → produce data
Main Thread → evaluates alerts
Logger Thread → writes logs
```

---

## 📊 Data Flow

```text
Monitor → GetLastData()
        ↓
MonitorManager
        ↓
AlertManager → Check thresholds
        ↓
Logger → Queue → Worker Thread → Output
```

---

## ⚠️ Challenges & Solutions

### 1. Thread-safe data sharing

**Problem:**
Multiple monitor threads produce data while the main thread reads it.

**Solution:**
Used `std::mutex` and `std::lock_guard` to ensure safe access.

---

### 2. Preventing logging bottlenecks

**Problem:**
Synchronous logging blocks execution and impacts performance.

**Solution:**
Implemented asynchronous logging using a thread-safe queue and worker thread.

---

### 3. Avoiding log spam

**Problem:**
Continuous threshold violations caused excessive logging.

**Solution:**
Implemented a state-based alert system that logs only when the state changes.

---

## 📈 Performance Considerations

* Asynchronous logging prevents blocking the main execution flow
* Condition variables are used instead of busy waiting
* Minimal locking strategy reduces contention

---

## 🧠 Design Patterns Used

- **Singleton** → Config, Logger
- **Strategy Pattern** → Logging outputs (Console/File)
- **Producer-Consumer** → Async logging system
- **Manager Pattern** → MonitorManager, AlertManager
- **State Pattern** → Alert state transitions
- **Template Method** → Monitor interface design
- **RAII** → Resource management (mutex, file, threads)
- **Facade Pattern** → Application entry point
- **Dependency Injection** (basic) → Injecting AlertManager

---

## 🔧 Configuration Example

```ini
[CPU]
IntervalSeconds=1
WarningThreshold=70
CriticalThreshold=90

[RAM]
IntervalSeconds=3
WarningThreshold=75
CriticalThreshold=90

[APPLICATION]
SleepMs=100

[LOGGER]
MinLevel=INFO
EnableConsoleLog=true
EnableFileLog=true
LogFilePath=logs/system.log
```

---

## 🧾 Sample Output

```text
[2026-04-14 12:00:01] [INFO] Application is running
[2026-04-14 12:00:02] [WARNING] CPU HIGH: 78%
[2026-04-14 12:00:05] [ERROR] CPU CRITICAL: 92%
[2026-04-14 12:00:10] [INFO] CPU back to normal: 45%
```

---

## 📁 Project Structure

```text
/src
  ├── Application
  ├── MonitorManager
  ├── Monitors
  │     ├── CpuMonitor
  │     ├── MemoryMonitor
  │     └── ProcessMonitor
  ├── AlertManager
  ├── Logger
  │     ├── FileOutput
  │     └── ConsoleOutput
  ├── Config
  │     ├── Config
  │     ├── ConfigLoader
  │     └── ConfigValidator
```

---

## ▶️ How to Run

1. Clone the repository
2. (Optional) Edit `config/system.ini`
3. Build with a C++17 compatible compiler
4. Run the executable

---

## 🔌 Extensibility

The system is designed to be easily extended:

* New monitors can be added by implementing `IMonitor`
* New output types can be added via `ILogOutput`
* New metrics can be integrated without modifying core logic

---

## ⚠️ Limitations

* CLI-based (no GUI yet)
* Logging queue is unbounded (potential memory growth under heavy load)
* No log rotation implemented

---

## 🚀 Future Improvements

* Qt-based GUI
* Real-time charts (CPU/RAM usage)
* Process-level alerting
* Log rotation support
* Bounded logging queue
* Dynamic config reload

---

## 💡 Learning Outcomes

Through this project, I gained experience in:

* Multithreading and synchronization
* Asynchronous system design
* Modular architecture
* Logging system design
* Configuration parsing and validation
* Windows system programming

---

## 👤 Author

**Mustafa Yeşil**
