# <b> Bishal Dahal </b> (PUR081BCT014)
</br>
# <b> Aayush Neupane </b> (PUR081BCT004)







# 💬 Terminal Based Chat Application

A terminal-based chat system built in **C++17** to demonstrate OOP, file persistence, multithreading, and optional networking with sockets. It supports user accounts, friendships, and direct messaging.

---

## 📑 Table of Contents
1. Introduction  
2. Objectives & Scope  
3. Tools & Technologies  
4. System Architecture  
5. Class & Module Design  
6. Data Model & File Formats  
7. Core Workflows  
8. Algorithms (Pseudocode)  
9. Optional Networking  
10. Error Handling  
11. Security  
12. Performance  
13. Testing Plan  
14. Build & Run  
15. Future Enhancements  
16. References  

---

## 📝 Introduction
This project is a **command-line chat application** for learning and demonstrating modern C++ design.  
Features include account management, friendships, and messaging. Data is stored in simple text files, with an optional TCP client-server mode for real-time chat.

---

## 🎯 Objectives
- Modular architecture (UI, logic, persistence, networking).  
- Account system with registration/login.  
- Friend requests (send/accept/reject).  
- One-to-one messaging.  
- File-based persistence (thread-safe).  
- Optional TCP real-time messaging.  

---

## ⚙️ Tools & Technologies
- **Language:** C++17  
- **Build:** g++ / clang (`-std=c++17 -pthread`)  
- **OS:** Linux/macOS (POSIX sockets), Windows (Winsock)  
- **Key Topics:** OOP, STL containers, multithreading, file handling, hashing  

---

## 🏗️ Architecture
- **CLI Layer** → user commands & output  
- **Domain Layer** → classes for `User`, `FriendSystem`, `Message`  
- **Persistence Layer** → file I/O, safe writes  
- **Networking Layer (optional)** → TCP server & clients  

---

## 📦 Modules & Classes
- **User** → id, username, password hash, createdAt  
- **UserManager** → register, login, lookup  
- **AuthService** → hashing (SHA-256/bcrypt)  
- **FriendSystem** → requests & friendship graph  
- **MessageStore** → append, list, inbox, history  
- **CLI** → command interface  

---

## 🗄️ Data Formats
All data stored in `|`-delimited text files:
