# Hospital Management System

A Hospital Management System written in C for the Programming Fundamentals Complex Computing Problem.

---

## Overview

The system manages patient records, doctor schedule, appointment scheduling, emergency priority queues, billing, and data storage. Data for patients is managed using dynamic memory allocation, while appointments and doctors are tracked using fixed size arrays.

---

## Project Contributors

- **Muhammad Yousuf Jamil** (CT-26292) – *Appointments & Reports Modules*
- **Syed Omer Abdul Rehman** (CT-26291) – *Navigation, Main Menus & Data Persistence*
- **Muhammad Owais** (CT-26278) – *Patient Management & Priority Queue Modules*
- **Asad Abbas** (CT-26280) – *Doctor Management & Billing System*

---

## Features

- **Portal-Based Navigation:** Admin, Reception, Doctor, and Reports portals.
- **Dynamic Memory Management:** Patient records utilize dynamic allocation (`malloc` and `realloc`) to scale capacity safely.
- **Patient Management:** Unique ID validation, registration, updates with pointers, and binary search
- **Doctor Management:** Track doctor profiles, specializations, room allocations, consultation fees, and auto-suggest doctors based on availability.
- **Slot-Based Scheduling:** 2D matrix (`schedule[MAX_DOC][MAX_SLOT]`) mapping doctors to hourly slots (9:00 AM – 4:00 PM) to fix double-booking.
- **Priority Queue System:** Priority sorting (`sortQueue`) that promotes emergency appointments while maintaining arrival order for non-emergency patients.
- **Automated Billing & Insurance:** Calculates totals based on doctor consultation fees, room type (General, Private, ICU), treatment type (General, Surgery, Other), and applies tier-based insurance discounts (Basic, Standard, Premium) with maximum coverage caps.
- **Financial Processing:** Supports cash, card, and bank transfer payments with change calculation and unpaid bill tracking.
- **Recursive Functions:**
  - `recursiveBinarySearchID`: Performs binary search on sorted patient IDs.
  - `recursiveTotalRevenue`: Calculates overall hospital revenue recursively.
- **Data Persistence:** Automatic binary file storage (`hospital_data.txt`) on startup and exit.

## Technical Details

- **Language:** C
- **Data Structures:** `Patient`, `Doctor`, `Appointment`, `Bill`, `SlotTime`
- **Memory Allocation:** Dynamic re-allocation (`realloc`) for `Patient` records; static arrays for `Doctor` (100 max), `Appointment` (5000 max), and `Bill` (5000 max).
- **Time Slots:** 8 daily slots per doctor ranging from 09:00 AM to 04:00 PM.

---

## General Course Requirements (CCP Guidelines)

This project satisfies all baseline specifications set for the Programming Fundamentals Complex Computing Problem (CCP):

- **Input Validation:** Strict validation applied across all input routines (`takeValidInt`, `takeStringInput`) to reject invalid choices, out-of-bounds numbers, and buffer glitches.
- **Structured Menus:** Switch-based main menu with multi-level sub-menus (Admin, Reception, Doctor, Reports).
- **Modular Design:** Divided into more than 10 dedicated functions with parameters and appropriate return values.
- **Data Structures & Memory Allocation:**
  - Dynamic memory management (`malloc` and `realloc`) for safely growing patient records.
  - 1D arrays of custom `struct` types (`Patient`, `Doctor`, `Appointment`, `Bill`).
  - 2D grid matrix mapping doctor-by-timeslot availability.
- **Search & Sort Algorithms:**
  - Iterative string search by patient/doctor name and sorting algorithms (Bubble Sort) for records and queues.
  - Pass-by-reference/pointers used for updating records.
- **Recursion:** Implemented for key operations, including binary search on sorted IDs and total hospital revenue calculation.
- **Data Persistence & Robust File Handling:** File read/write capabilities (`hospital_data.txt`) with binary corruption checks and missing-file handling.

## Hospital Management System Specific Requirements

The project satisfies all required criteria specific to Problem 1 (Hospital Management System):

| Requirement | Implementation Details |
| :--- | :--- |
| **Array of Structs** | Implemented using `Patient` (dynamically managed), `Doctor`, `Appointment`, and `Bill` structures. |
| **2D Schedule Grid** | `schedule[MAX_DOC][MAX_SLOT]` matrix mapping doctors to 8 fixed hourly slots (09:00 AM – 04:00 PM) to eliminate double-booking. |
| **Switch-Based Menus** | Interactive menus tailored for Admin, Doctor, Reception, and Reports portals. |
| **Emergency Priority Queue** | `sortQueue` promotes emergency patients to the top of the queue while preserving arrival order for regular appointments. |
| **Search Capabilities** | Linear/string search by name (`searchPatientByName`, `searchDoctor`) and ID lookup. |
| **Nested Logic Billing** | Multi-variable bill calculation assessing doctor fee, room class (General, Private, ICU), treatment type (General, Surgery, Other), and tier-capped insurance discounts. |
| **Recursive Binary Search** | `recursiveBinarySearchID` executes binary search over sorted patient IDs. |
| **File Persistence** | Full save/load functionality preserving system counters, structures, and schedule state in binary format. |

---

## How to compile and run

### Prerequisites
- GCC Compiler (MinGW for Windows, or default GCC on Linux/macOS)

### Steps
1. Clone or download the repository.
2. Open your terminal/command prompt and navigate to the project directory.
3. Compile the source code:
   ```bash
   gcc main.c -o hospital_management_system
   ```
4. Run the executable:
  - Linux / macOS:
  ```bash
  ./hospital_management_system
  ```
  - Windows:
  ```cmd
  hospital_management_system.exe
  ```

---

## System Workflow

1. **Main Menu:** Select between Admin, Reception, Doctor, and Reports portals.
2. **Admin Portal:** Add new doctors to the system and view existing doctor profiles.
3. **Reception Portal:** Register patients, edit records, book/cancel appointments, complete appointments, process payments, and view the live priority queue.
4. **Doctor Portal:** View current daily schedules across all time slots.
5. **Reports Menu:** Access system-wide patient/doctor lists, total hospital schedule, current queue status, and total revenue calculations.
6. **Exit / Save:** System automatically writes all record counts, array data, and schedule matrices to `hospital_data.txt`.

## Sample Outputs
### Menus
![Main Menu Screenshot](Screenshots/output%2001.png)
_Figure 1: Main Menu_

![Admin Menu Screenshot](Screenshots/output%2002.png)
_Figure 2: Admin Menu_

![Reception Menu Screenshot](Screenshots/output%2003.png)
_Figure 3: Reception Menu_

![Doctor Menu Screenshot](Screenshots/output%2004.png)
_Figure 4: Doctor Menu_

![Reports Menu Screenshot](Screenshots/output%2005.png)
_Figure 5: Reports Menu_

### Reports to show an example state of system

![Stored Patients Screenshot](Screenshots/output%2006.png)
_Figure 6: Stored Patients_

![Stored Doctors Screenshot](Screenshots/output%2007.png)
_Figure 7: Stored Doctors_

![Current Schedule Screenshot](Screenshots/output%2008.png)
_Figure 8: Current Schedule_

![Patient Queue Screenshot](Screenshots/output%2009.png)
_Figure 9: Patient Queue_

![Bill Payment Screenshot](Screenshots/output%2010.png)
_Figure 10: Bill Payment_

![Hospital Revenue Screenshot](Screenshots/output%2011.png)
_Figure 11: Hospital Revenue_

![File Saving Screenshot](Screenshots/output%2012.png)
_Figure 12: Demonstration of file saving_

### Erronous Data Entry

![Duplicate ID Input Screenshot](Screenshots/output%2013.png)
_Figure 13: Demonstration of duplicate ID handling_

![Unavailable Specialization Screenshot](Screenshots/output%2014.png)
_Figure 14: Demonstration of unavailable specialization_

![Invalid Option Screenshot](Screenshots/output%2015.png)
_Figure 15: Demonstration of invalid option input screenshot_

---

## Data Storage

- **Data File:** `hospital_data.txt`
- Stores counts and raw binary structures for patients, doctors, appointments, bills, and the schedule matrix.
- Automatically handles file non-existence on first run and validates save files against corruption on subsequent launches.

---

## Repository Structure

```text
PF_CCA_Project_GroupNo4/
│── Source_Code/
│   ├── Dummy inputs.txt
│   ├── hospital_data backup.txt
│   ├── hospital_data.txt
│   └── main.c
│── Screenshots/
│   ├── output 01.png
│   ├── output 02.png
│   ├── output 03.png
│   ├── output 04.png
│   ├── output 05.png
│   ├── output 06.png
│   ├── output 07.png
│   ├── output 08.png
│   ├── output 09.png
│   ├── output 10.png
│   ├── output 11.png
│   ├── output 12.png
│   ├── output 13.png
│   ├── output 14.png
│   └── output 15.png
│── Project_Report.pdf
└── README.md
```
