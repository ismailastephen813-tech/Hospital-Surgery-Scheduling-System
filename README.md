# Hospital Surgery Scheduling System

## Overview

A C++17 console-based Hospital Surgery Scheduling System developed as part of INFO 3135 – Project 1.

The application manages patient information and surgical procedure requests while organizing surgeries according to their priority.

## Features

* Patient information management.
* Surgical procedure request management.
* Priority-based surgery scheduling.
* Patient urgency and procedure complexity tracking.
* CSV file saving and loading.
* Unit testing using Catch2.

## Technologies

* C++17
* Standard Template Library (STL)
* Catch2
* CSV file handling

## Project Structure

* `main.cpp` – Main application.
* `Patient.h` – Patient class.
* `ProcedureRequest.h` – Procedure request class.
* `PriorityQueue.h` – Priority queue implementation.
* `LinkedList.h` – Linked list implementation.
* `surgery_tests.cpp` – Unit tests.
* `catch_amalgamated.hpp` – Catch2 header.
* `catch_amalgamated.cpp` – Catch2 implementation.

## Build and Run

Compile the application:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -O0 main.cpp -o surgery_app
```

Run:

```bash
./surgery_app
```

## Run Unit Tests

Compile:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -O0 surgery_tests.cpp catch_amalgamated.cpp -o surgery_tests
```

Run:

```bash
./surgery_tests
```

## Testing Results

The provided unit tests passed successfully:

* 15 test cases passed.
* 71 assertions passed.

## CSV Format

The application supports saving and loading patient procedure requests using CSV files.

Format:

`patient_id,name,procedure_name,urgency,complexity,waiting_days`

Each procedure for a patient is stored on a separate row.

---

**Course:** INFO 3135
**Project:** Project 1 – Hospital Surgery Scheduling System
