# Hospital Management System

A console-based hospital management system written in C for the Programming Fundamentals Complex Computing Problem.

## Overview

The system handles patient accounts, doctor scheduling, emergency priority and billing. Registration only creates a patient account. Doctor choice, emergency status, room type and booking are handled at appointment time, and billing follows the appointment.

## Features

- Switch-based main menu with Admin, Doctor and Reception sub-menus
- Patient registration with duplicate-ID check and input validation
- View and edit patient records (pass by reference)
- Doctor management: add, view and search doctors by speciality
- Appointment booking using a 2D doctor-by-timeslot array (no double-booking)
- Emergency patients jump the queue (sorted by emergency flag, then arrival order)
- Doctor marks appointment completed and records treatment
- Bill calculation by treatment, room type and insurance class
- Payment by cash, card or bank transfer, with printed bill
- Search patients by ID (recursive binary search) and by name
- Sort patients by name or ID and bills by amount
- Reports: total revenue, paid vs unpaid bills, appointments per doctor, emergency count
- File save/load of all records with missing-file check

## Usage

1. Start the program and choose a role from the main menu: Admin, Doctor or Reception.
2. **Admin:** add doctors, view patients, view reports, save data.
3. **Reception:** register patients, book or cancel appointments, view the queue, handle billing.
4. **Doctor:** view own schedule and patients, mark appointments completed.
5. Choose Exit to save and close. Data is loaded automatically on the next run.

## Data Files

- `hospital.dat` stores all patients, doctors, appointments, bills and the schedule.
- If the file is missing on first run, the program starts empty.

## Project Structure

- Structs: Patient, Doctor, Appointment, Bill
- Schedule: 2D array (doctor by time slot), 0 means free
- Recursion: binary search on sorted patient IDs, total revenue
- Input helpers validate every input

## Group Members

- Muhammad Yousuf Jamil (CT-26292)
- Syed Omer Abdul Rehman (CT-26291)
- Muhammad Owais (CT-26278)
- Asad Abbas (CT-26280)
