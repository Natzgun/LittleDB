# LittleDB - Educational Database Management System

⚠️ **Disclaimer:**  
This project was developed **for educational purposes only** as part of my **Database II course** at the *School of Computer Science - UNSA*.  
It is **not intended for production use**.

The implementation was built **from scratch in C++** to understand how a **Database Management System (DBMS)** works internally.

---

## 🖼️ Diagrams

### High-Level Architecture
<p align="center">
  <img width="358" height="497" alt="LittleDB high-level" src="https://github.com/user-attachments/assets/b369854f-2958-445f-bbe2-33682c1a831e" />
</p>


### System Diagram
<img width="1479" height="1014" alt="LittleDB system" src="https://github.com/user-attachments/assets/880a0139-f9dc-43e3-ada7-0e22db70d477" />

---

## 📖 References & Learning Resources

This project was inspired by and based on the following materials:

- **Course:** [Carnegie Mellon University - 15-445/645 Database Systems (Spring 2024)](https://15445.courses.cs.cmu.edu/spring2024/)  
- **Book:** *Database System Concepts* (7th Edition)  
  - Avi Silberschatz  
  - Henry F. Korth  
  - S. Sudarshan  
- **YouTube Playlist:** [CMU Database Systems](https://youtube.com/playlist?list=PLSE8ODhjZXjbj8BMuIrRcacnQh20hmY9g&si=pjqgG1G76WJy-Vge)

---

## 📂 Project Overview

**LittleDB** is a **miniature DBMS** that implements the main building blocks of a database system, including:

- Buffer Manager  
- Disk Manager  
- Query Execution  
- Storage Organization  
- Indexing (B+ Tree)  
- Interactive Console  

It is designed to simulate how modern databases manage storage, memory, and queries.

---

## ⚙️ Main Features

### 🗄️ Storage & Disk Management
- Creation of a custom **disk structure**  
- Persistent storage with **LOAD** functionality to recover data between executions  
- HeapFile management for free blocks  
- Disk sector and block allocation for relations  

### 🧠 Buffer Management
- **Frames** with pinning/unpinning, dirty flags, and reference bits  
- **Page** abstraction with support for adding/deleting records  
- **Replacement Policies**:  
  - Least Recently Used (LRU)  
  - Clock Algorithm  
- Functions to load, update, delete, and persist pages  

### 🌳 Indexing
- **B+ Tree** implementation supporting:  
  - Insertions  
  - Searches  
  - Deletions (leaf and internal nodes)  
  - DOT export for visualization  

### 📋 Query Execution
- **DatabaseMediator** to connect buffer, disk, and query layers  
- Insert, select, and delete operations  
- CSV import support  
- Schema validation and management  

### 💻 Interactive Console
- Execute SQL-like commands:
  - `createTable`
  - `insertSchema`
  - `selectTable`
  - `showTable`
  - `readCSV`
- Data visualization in memory (RAM view)  
- Simple command validation  

---

## 📝 Example Behavior

- When inserting into an empty table, the **HeapFile** allocates free blocks, and the **B+ Tree** assigns the first page to the buffer.  
- The console shows the relation’s blocks and sectors.  
- The system maintains **persistence** through the `LOAD` command.  
- Data modifications are reflected correctly on disk via the buffer manager.  

---

## 📑 Policies of Operation

- **Buffer Manager**
  - `insert` → loads and pins page, writes record, marks dirty, later flushed to disk  
  - `delete` → loads page, removes record, updates disk on flush  
  - `select` → loads required pages for query optimization  
  - `where` → evaluates conditions on loaded pages  

- **Disk Manager**
  - Manages writing, deleting, and retrieving data blocks  
  - Ensures efficient organization of storage  
  - Keeps catalog and metadata updated  

- **Indexing**
  - Accelerates queries via B+ Tree  
  - Maintains index consistency on insert/delete operations  


---

## 📜 License

This project is released under the **GNU General Public License v3.0 (GPL-3.0)**.

---

## 👨‍💻 Authors

**Erick Malcoaccha (Natzgun)**  
Educational DBMS project - UNSA, Database II Course.  
Focused on systems programming and database internals.  

**Erik Ramos**  
Main collaborator in the development of core components (Buffer Manager, B+Tree, Clock, etc.).
