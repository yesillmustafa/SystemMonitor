# 🖥️ System Monitoring Application (C++)

## 📌 Overview

This project is a **multi-threaded, modular system monitoring application** developed in C++.

It monitors system resources such as **CPU, RAM, and processes**, evaluates them against configurable thresholds, and generates alerts using an **asynchronous (non-blocking) logging system**.

The project focuses on building a system that is not only functional, but also:

- Scalable  
- Maintainable  
- Performance-aware  
- Fault-tolerant  

---

## 🧠 Architecture Overview

The system follows a **pipeline-based architecture**:
```
Monitors (Threads)
↓
MonitorManager
↓
AlertManager
↓
Logger (Async)
↓
Outputs (Console / File)
```

Each component is designed with **single responsibility** and **loose coupling** principles.

---

## 🧩 Core Components

### 🔹 Monitors
- `CpuMonitor`
- `MemoryMonitor`
- `ProcessMonitor`

Each monitor:
- Runs in its own thread  
- Collects system metrics periodically  
- Provides thread-safe access to data  

---

### 🔹 MonitorManager
- Registers and manages monitors  
- Starts and stops monitoring threads  
- Acts as a central coordination layer  

---

### 🔹 AlertManager
- Evaluates metrics against thresholds  
- Generates alerts (Warning / Critical)  
- Logs only **state changes** to prevent spam  

---

### 🔹 Logger (Asynchronous)
- Thread-safe logging system  
- Implements **Producer–Consumer pattern**  
- Uses a background worker thread  

Supports:
- Console output  
- File output  

---

### 🔹 Configuration System
- INI-based configuration (`system.ini`)  
- Strongly typed parsing  
- Schema validation  
- **Fail-safe fallback to default values**  

---

## 🧵 Threading Model

The system uses multiple threads:

- **Monitor Threads** → collect system data  
- **Main Thread** → evaluates alerts  
- **Logger Thread** → processes logs asynchronously  

This separation ensures:

- Non-blocking execution  
- Better performance  
- Clear responsibility boundaries  

---

## 🔄 Data Flow
```
Monitor → GetLastData()
↓
MonitorManager
↓
AlertManager → Evaluate thresholds
↓
Logger → Queue → Worker Thread
↓
Console / File Output
```

The system transforms:

- Raw system data → structured metrics → meaningful alerts → logs  

---

## ⚙️ Features

### ✔️ System Monitoring
- CPU usage tracking  
- RAM usage tracking  
- Process monitoring (extensible)

---

### ✔️ Multithreading
- Independent monitor threads  
- Thread-safe data sharing  

---

### ✔️ Alert System
- Threshold-based alerts  
- Warning & Critical states  
- State-based logging (no redundant logs)

---

### ✔️ Asynchronous Logging
- Non-blocking architecture  
- Queue-based logging system  
- Multiple output targets  

---

### ✔️ Configuration-Driven Design
- Easily customizable via config file  
- Safe fallback mechanism  

---

## 🔧 Configuration Example
```
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

## 🧪 Key Design Decisions

- **Asynchronous logging** → prevents performance bottlenecks  
- **State-based alerting** → avoids unnecessary logs  
- **Thread isolation** → improves responsiveness  
- **Config-driven design** → increases flexibility  
- **Variant-based data model** → enables type-safe metric handling  

---

## ⚖️ Trade-offs

- Thread-per-monitor model (simple but not infinitely scalable)  
- Unbounded logging queue (risk under extreme load)  
- Static configuration (no runtime reload)  
- Windows-specific implementation  

---

## 🚀 Future Improvements

- GUI integration (Qt)  
- Real-time charts  
- Dynamic config reload  
- Log rotation  
- Bounded logging queue  
- Cross-platform support  
- Plugin-based monitor system  

---

## 🧠 Learning Outcomes

This project helped me gain practical experience in:

- Multithreading & synchronization  
- Designing modular and scalable systems  
- Asynchronous programming patterns  
- System-level programming (Windows APIs)  
- Configuration parsing & validation  
- Thinking in trade-offs and system design  

---
