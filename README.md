# 🚇 UrbanGraph: Delhi Metro Navigation System

![C++](https://img.shields.io/badge/Language-C++17-blue.svg)
![Algorithm](https://img.shields.io/badge/Algorithm-Dijkstra-green.svg)
![Visualization](https://img.shields.io/badge/Visualization-Graphviz-orange.svg)
![Status](https://img.shields.io/badge/Status-Completed-success.svg)

> **An advanced, CLI-based navigation engine for the Delhi Metro network. Built from scratch using Graph Data Structures, it features shortest-path optimization, smart station search, and dynamic map generation.**

---

## 🚀 Overview

**UrbanGraph** is not just a simple pathfinder. It is a full-scale simulation of the Delhi Metro network (DMRC). Unlike basic projects that hardcode values, this system loads data dynamically from external files, calculates fares using real-world logic, and generates visual maps of your specific route.

Whether you need the quickest path from **Noida Sector 18** to **Cyber City** or just want to check the fare, UrbanGraph processes the complex web of **280+ stations** and **10+ lines** in milliseconds.

---

## ✨ Key Features (The "Pro" Level)

### 🧠 1. Smart Station Search
* **Case-Insensitive:** Type `rithala`, `RITHALA`, or `Rithala` — the system understands it all.
* **Robust Input Handling:** Automatically trims spaces and handles typos in formatting.

### ⚡ 2. Optimized Pathfinding
* **Dijkstra’s Algorithm:** Uses a `std::set` based implementation (instead of `priority_queue`) for optimized "decrease-key" operations.
* **Efficiency:** Finds the shortest path in $O(E \log V)$ time complexity.

### 💸 3. Dynamic Fare Calculation
* **Logic-Over-Data:** Instead of reading a massive static CSV file, the system calculates fares in real-time using official DMRC distance slabs (0-2km, 2-5km, etc.).

### 🗺️ 4. Visual Map Generation
* **Graphviz Integration:** The program doesn't just print text. It generates a `finalmap.dot` file that renders a **color-coded map** of your specific journey.
* **Route Highlighting:** The stations on your path are highlighted in **Green**, while the rest of the network is drawn in the background.

### 🎨 5. Interactive "Timeline" UI
* **Color-Coded Output:** The terminal output changes color (Red/Blue/Yellow) to match the actual Metro Line you are riding.
* **Step-by-Step Guide:** Shows exactly where to **Interchange**, board, and de-board.

---

## 🛠️ Tech Stack

| Component | Technology Used |
| :--- | :--- |
| **Language** | C++ (GCC 6.3.0 Compatible) |
| **Data Structure** | Graph Adjacency List (`std::map<string, list<Edge>>`) |
| **Algorithm** | Dijkstra’s Shortest Path Algorithm |
| **Containers** | STL (`vector`, `set`, `map`, `pair`) |
| **File I/O** | `fstream` for reading `stations.txt` & `edges.txt` |
| **Visualization** | Graphviz DOT Language |

---

## 📂 Project Structure

```text
📦 UrbanGraph-DelhiMetro
 ┣ 📜 Delhimetro.cpp       # The Main Engine (Source Code)
 ┣ 📜 stations.txt         # Database of all station names
 ┣ 📜 edges.txt            # Connection Logic (Source, Dest, Dist, Color)
 ┣ 📜 finalmap.dot         # Generated Map File (Output)
 ┗ 📜 README.md            # Documentation
