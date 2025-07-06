# Hospital Management System

A C++-based console application that simulates a hospital environment using core data structures like linked lists, priority queues, AVL trees, and maps. This project was developed as part of the **Data Structures, Algorithms, and Applications (DSAA)** course to demonstrate real-world application of data structures in healthcare management.

## Table of Contents

* [Overview](#overview)
* [Features](#features)
* [Data Structures Used](#data-structures-used)
* [System Design](#system-design)
* [Getting Started](#getting-started)
* [Limitations & Future Work](#limitations--future-work)

## Overview

Managing patients manually in hospitals can be error-prone and inefficient, especially during emergencies. This project aims to automate patient check-ins, doctor assignments, emergency handling, and medical record storage using optimized data structures.

The system supports core functionalities like:

* Prioritizing emergency cases
* Assigning doctors based on specialization
* Modifying and updating patient information
* Securely storing discharged records for future use

## Features

* Register new patients with emergency/non-emergency status
* Assign patients to appropriate doctor queues based on disease
* Prioritize emergency patients using a priority queue
* Modify checked-in patient details
* Track and update payment status
* Check out patients and archive their records in an AVL tree
* Search patients in both active queues and past records
* Display all patients (active and archived)

## Data Structures Used

* **Linked List**: To manage patient queues in real time
* **Priority Queue**: For prioritizing emergency cases
* **Map**: To associate doctors/specializations with specific queues
* **AVL Tree (Balanced BST)**: To store and search historical patient records efficiently

## System Design

* Object-oriented design with modular classes: `PatientQueue`, `PatientBST`, `HospitalManagementSystem`
* Patients are first queued, and upon discharge, moved to a self-balancing BST (AVL)
* The system is menu-driven and interacts via command-line interface

## Getting Started

### Prerequisites

* C++ compiler (e.g., g++, clang++)
* Basic knowledge of C++ and data structures

### Run the Project

1. Clone the repository:

   ```bash
   git clone https://github.com/<your-username>/hospital-management-system.git
   cd hospital-management-system
   ```

2. Compile:

   ```bash
   g++ main.cpp -o hospital
   ```

3. Run:

   ```bash
   ./hospital
   ```

> Replace `main.cpp` with actual source filenames if needed.

## Limitations & Future Work

* No persistent storage: patient data is lost after program exit
* No GUI: CLI-only interface may be hard for non-technical users
* No real-time multi-user support
* Emergency handling doesn't include ambulance logistics
* Lacks robust input validation and error handling

**Future Enhancements:**

* Integrate with a database (e.g., SQLite or MySQL)
* Build a graphical interface using Qt or other frameworks
* Add real-time updates and multi-user functionality
* Improve encryption and security for sensitive data

