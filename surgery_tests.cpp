// Use Catch2 amalgamated header
#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

// Project headers
#include "ProcedureRequest.h"
#include "Patient.h"
#include "PriorityQueue.h"

// Standard library headers
#include <cstddef>      // std::size_t
#include <stdexcept>    // std::invalid_argument, std::runtime_error
#include <string>       // std::string
#include <vector>       // std::vector

/*
Command-line example:

clang++ -std=c++17 -Wall -Wextra -Wpedantic -O0 -g \
  surgery_tests.cpp catch_amalgamated.cpp \
  -o surgery_tests

./surgery_tests
*/

// ---------------------------------------------------------
// Helper Functions
// ---------------------------------------------------------

static std::vector<Patient> generate_ordered_patient_set()
{
    Patient p1(101, "A");
    p1.add_procedure(
        ProcedureRequest("Cardiac Surgery", 10, 8, 5)
    ); // (10 * 3) + 8 + 5 = 43

    Patient p2(102, "B");
    p2.add_procedure(
        ProcedureRequest("Neurosurgery", 9, 7, 4)
    ); // (9 * 3) + 7 + 4 = 38

    Patient p3(103, "C");
    p3.add_procedure(
        ProcedureRequest("Orthopedic Surgery", 6, 5, 3)
    ); // (6 * 3) + 5 + 3 = 26

    Patient p4(104, "D");
    p4.add_procedure(
        ProcedureRequest("General Surgery", 5, 4, 2)
    ); // (5 * 3) + 4 + 2 = 21

    Patient p5(105, "E");
    p5.add_procedure(
        ProcedureRequest("Minor Procedure", 3, 2, 3)
    ); // (3 * 3) + 2 + 3 = 14

    Patient p6(106, "F");
    p6.add_procedure(
        ProcedureRequest("Minor Procedure", 3, 2, 3)
    ); // 14

    return { p1, p2, p3, p4, p5, p6 };
}


static std::vector<Patient> generate_unordered_patient_set()
{
    auto ordered = generate_ordered_patient_set();

    /*
        Input order:

        p5 = score 14
        p1 = score 43
        p6 = score 14
        p3 = score 26
        p4 = score 21
        p2 = score 38

        Because p5 is inserted before p6 and both have
        the same score, p5 must remain ahead of p6.
    */

    return {
        ordered[4],
        ordered[0],
        ordered[5],
        ordered[2],
        ordered[3],
        ordered[1]
    };
}


// ---------------------------------------------------------
// ProcedureRequest Tests
// ---------------------------------------------------------

TEST_CASE(
    "ProcedureRequest: Constructor sets all fields",
    "[ProcedureRequest]"
)
{
    const std::string name = "Cardiac Surgery";
    const int urgency = 10;
    const int complexity = 8;
    const int waiting_days = 5;

    ProcedureRequest procedure(
        name,
        urgency,
        complexity,
        waiting_days
    );

    REQUIRE(procedure.get_name() == name);
    REQUIRE(procedure.get_urgency() == urgency);
    REQUIRE(procedure.get_complexity() == complexity);
    REQUIRE(procedure.get_waiting_days() == waiting_days);
}


TEST_CASE(
    "ProcedureRequest: Calculates correct priority score",
    "[ProcedureRequest]"
)
{
    ProcedureRequest procedure(
        "Cardiac Surgery",
        10,
        8,
        5
    );

    // (Urgency * 3) + Complexity + Waiting Days
    // (10 * 3) + 8 + 5 = 43

    REQUIRE(procedure.get_score() == 43);
}


// ---------------------------------------------------------
// Patient Tests
// ---------------------------------------------------------

TEST_CASE(
    "Patient: Constructor sets patient ID and name",
    "[Patient]"
)
{
    const int patient_id = 101;
    const std::string name = "Patient Name";

    const Patient patient(patient_id, name);

    REQUIRE(patient.get_patient_id() == patient_id);
    REQUIRE(patient.get_name() == name);
}


TEST_CASE(
    "Patient: AddProcedure stores procedures in order",
    "[Patient]"
)
{
    Patient patient(101, "Patient Name");

    const std::vector<ProcedureRequest> procedures = {

        ProcedureRequest(
            "Cardiac Surgery",
            10,
            8,
            5
        ),

        ProcedureRequest(
            "Minor Procedure",
            2,
            1,
            1
        )
    };

    for (const auto& procedure : procedures)
    {
        patient.add_procedure(procedure);
    }

    int counter = 0;

    auto* node = patient.get_procedures().begin();

    while (node != nullptr)
    {
        REQUIRE(
            node->data == procedures[counter]
        );

        ++counter;
        node = node->next;
    }

    REQUIRE(
        counter ==
        static_cast<int>(procedures.size())
    );
}


TEST_CASE(
    "Patient: Calculates total score from multiple procedures",
    "[Patient]"
)
{
    Patient patient(101, "Patient Name");

    patient.add_procedure(
        ProcedureRequest(
            "Procedure A",
            5,
            4,
            2
        )
    );

    // (5 * 3) + 4 + 2 = 21

    patient.add_procedure(
        ProcedureRequest(
            "Procedure B",
            3,
            2,
            3
        )
    );

    // (3 * 3) + 2 + 3 = 14

    // Total = 21 + 14 = 35

    REQUIRE(patient.get_score() == 35);
}


TEST_CASE(
    "Patient: Calculates correct scores",
    "[Patient]"
)
{
    auto patients = generate_ordered_patient_set();

    const std::vector<int> scores = {
        43,
        38,
        26,
        21,
        14,
        14
    };

    for (size_t i = 0; i < patients.size(); ++i)
    {
        REQUIRE(
            patients[i].get_score() == scores[i]
        );
    }
}


// ---------------------------------------------------------
// PriorityQueue Tests
// ---------------------------------------------------------

TEST_CASE(
    "PriorityQueue: Constructor yields empty queue",
    "[PriorityQueue]"
)
{
    PriorityQueue<Patient> queue;

    REQUIRE(queue.empty());
    REQUIRE(queue.size() == 0);
}


TEST_CASE(
    "PriorityQueue: SinglePatient enqueue and dequeue",
    "[PriorityQueue]"
)
{
    PriorityQueue<Patient> queue;

    const Patient patient(
        101,
        "Bob Smith"
    );

    queue.enqueue(patient);

    REQUIRE_FALSE(queue.empty());
    REQUIRE(queue.size() == 1);

    Patient out = queue.dequeue();

    REQUIRE(out == patient);

    REQUIRE(queue.empty());
    REQUIRE(queue.size() == 0);
}


TEST_CASE(
    "PriorityQueue: Ordered patients dequeue in same order",
    "[PriorityQueue]"
)
{
    PriorityQueue<Patient> queue;

    const std::vector<Patient> ordered =
        generate_ordered_patient_set();

    size_t counter = 0;

    for (const auto& patient : ordered)
    {
        queue.enqueue(patient);

        ++counter;

        REQUIRE_FALSE(queue.empty());
        REQUIRE(queue.size() == counter);
    }

    for (const auto& expected : ordered)
    {
        --counter;

        Patient out = queue.dequeue();

        REQUIRE(out == expected);
        REQUIRE(queue.size() == counter);

        if (counter == 0)
        {
            REQUIRE(queue.empty());
        }
        else
        {
            REQUIRE_FALSE(queue.empty());
        }
    }

    REQUIRE(queue.empty());
    REQUIRE(queue.size() == 0);
}


TEST_CASE(
    "PriorityQueue: Unordered patients dequeue by descending priority",
    "[PriorityQueue]"
)
{
    PriorityQueue<Patient> queue;

    const std::vector<Patient> unordered =
        generate_unordered_patient_set();

    const std::vector<Patient> expected =
        generate_ordered_patient_set();

    for (const auto& patient : unordered)
    {
        queue.enqueue(patient);
    }

    REQUIRE(
        queue.size() == unordered.size()
    );

    for (const auto& expected_patient : expected)
    {
        Patient out = queue.dequeue();

        REQUIRE(out == expected_patient);
    }

    REQUIRE(queue.empty());
}


TEST_CASE(
    "PriorityQueue: Equal scores maintain FIFO order",
    "[PriorityQueue]"
)
{
    PriorityQueue<Patient> queue;

    Patient first(201, "First Patient");
    first.add_procedure(
        ProcedureRequest(
            "Procedure A",
            3,
            2,
            3
        )
    ); // 14

    Patient second(202, "Second Patient");
    second.add_procedure(
        ProcedureRequest(
            "Procedure B",
            3,
            2,
            3
        )
    ); // 14

    Patient third(203, "Third Patient");
    third.add_procedure(
        ProcedureRequest(
            "Procedure C",
            3,
            2,
            3
        )
    ); // 14

    queue.enqueue(first);
    queue.enqueue(second);
    queue.enqueue(third);

    REQUIRE(
        queue.dequeue().get_patient_id() == 201
    );

    REQUIRE(
        queue.dequeue().get_patient_id() == 202
    );

    REQUIRE(
        queue.dequeue().get_patient_id() == 203
    );
}


TEST_CASE(
    "PriorityQueue: Duplicate patient ID is not added",
    "[PriorityQueue]"
)
{
    PriorityQueue<Patient> queue;

    Patient patient1(
        101,
        "Maria Smith"
    );

    Patient patient2(
        101,
        "Maria Smith Updated"
    );

    queue.enqueue(patient1);
    queue.enqueue(patient2);

    REQUIRE(queue.size() == 1);
}


TEST_CASE(
    "PriorityQueue: Different patient IDs are not duplicates",
    "[PriorityQueue]"
)
{
    PriorityQueue<Patient> queue;

    Patient patient1(
        101,
        "Maria Smith"
    );

    Patient patient2(
        102,
        "Maria Smith"
    );

    queue.enqueue(patient1);
    queue.enqueue(patient2);

    REQUIRE(queue.size() == 2);
}


TEST_CASE(
    "PriorityQueue: Dequeue removes highest-priority patient",
    "[PriorityQueue]"
)
{
    PriorityQueue<Patient> queue;

    Patient low_priority(
        101,
        "Low Priority"
    );

    low_priority.add_procedure(
        ProcedureRequest(
            "Procedure A",
            2,
            1,
            1
        )
    ); // 8

    Patient high_priority(
        102,
        "High Priority"
    );

    high_priority.add_procedure(
        ProcedureRequest(
            "Procedure B",
            10,
            8,
            5
        )
    ); // 43

    queue.enqueue(low_priority);
    queue.enqueue(high_priority);

    Patient out = queue.dequeue();

    REQUIRE(
        out.get_patient_id() == 102
    );
}


TEST_CASE(
    "PriorityQueue: Dequeue from empty queue throws exception",
    "[PriorityQueue]"
)
{
    PriorityQueue<Patient> queue;

    REQUIRE_THROWS_AS(
        queue.dequeue(),
        std::runtime_error
    );
}
