INFO 3135 – Project 1: Hospital Surgery Scheduling System

Completed source files:
- ProcedureRequest.h
- Patient.h
- PriorityQueue.h
- main.cpp

Unmodified starter files retained:
- LinkedList.h
- surgery_tests.cpp
- catch_amalgamated.hpp
- catch_amalgamated.cpp

Build and run the console program:
  g++ -std=c++17 -Wall -Wextra -Wpedantic -O0 main.cpp -o surgery_app
  ./surgery_app

Build and run unit tests:
  g++ -std=c++17 -Wall -Wextra -Wpedantic -O0 surgery_tests.cpp catch_amalgamated.cpp -o surgery_tests
  ./surgery_tests

The provided tests were run successfully: 15 test cases, 71 assertions passed.

CSV format for Save/Load:
patient_id,name,procedure_name,urgency,complexity,waiting_days
Each procedure for a patient is stored on a separate row. Names/procedure names containing commas or quotation marks are CSV-escaped.

Before submitting, review the code so you can explain the implementation, run the program for your own video demonstration, and follow your instructor's submission rules.
