# Banker's Algorithm – Deadlock Avoidance System

## Overview

This project is a C++ implementation of the **Banker's Algorithm**, a deadlock avoidance algorithm used in operating systems to allocate resources safely among multiple processes. The application provides a graphical user interface (GUI) built with **Qt**, allowing users to enter resource allocation data, calculate safe sequences, and determine whether the system is in a safe state.

The project demonstrates how the Banker's Algorithm prevents deadlocks by ensuring that every resource allocation request leaves the system in a safe state.

---

## Features

* Graphical User Interface (GUI) using Qt
* Input for Available, Maximum, and Allocated resources
* Automatic calculation of the Need Matrix
* Safe state detection
* Safe sequence generation
* Deadlock avoidance simulation
* User-friendly interface for experimenting with different resource allocation scenarios

---

## Technologies Used

* **Language:** C++
* **Framework:** Qt
* **IDE:** Qt Creator
* **Algorithm:** Banker's Algorithm

---

## Project Structure

```text
.
├── main.cpp
├── mainwindow.cpp
├── mainwindow.h
├── mainwindow.ui
├── ResourceManager.cpp
├── ResourceManager.h
├── Bankers_alg.pro
└── README.md
```

---

## How Banker's Algorithm Works

The Banker's Algorithm checks whether granting a resource request keeps the system in a **safe state**.

The algorithm performs the following steps:

1. Calculate the **Need Matrix**.

   ```
   Need = Maximum - Allocation
   ```

2. Initialize the available resources.

3. Find a process whose resource needs can be satisfied.

4. Simulate the completion of that process and release its allocated resources.

5. Repeat until all processes complete or determine that no safe sequence exists.

If all processes can complete successfully, the system is considered **safe**.

---

## How to Run

1. Clone the repository.

```bash
git clone https://github.com/Mirdhuna/Deadlock_detection.git
```

2. Open the project in **Qt Creator**.

3. Build the project.

4. Run the application.

5. Enter the required resource information and verify whether the system is in a safe state.

---

## Learning Outcomes

This project helped in understanding:

* Deadlock avoidance techniques
* Resource allocation strategies
* Safe state verification
* Operating System scheduling concepts
* GUI application development using Qt
* Object-Oriented Programming in C++

---

## Future Improvements

* Support for dynamic process creation
* Visualization of resource allocation graphs
* Step-by-step execution of the algorithm
* Request Resource Algorithm implementation
* Export simulation results
* Improved UI with charts and animations

---

## Author

**Mirdhuna Nandhakumar**

M.Sc. Theoretical Computer Science

---

## License

This project is intended for educational and academic purposes.
