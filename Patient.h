#pragma once

#include "ProcedureRequest.h"
#include "LinkedList.h"

#include <string>
#include <utility>
#include <stdexcept>

class Patient
{
    int patient_id_{ 0 };
    std::string name_{};
    LinkedList<ProcedureRequest> procedures_{};

public:
    Patient() = default;

    Patient(int patient_id, std::string name)
        : patient_id_(patient_id), name_(std::move(name))
    {
        if (patient_id_ < 0) {
            throw std::invalid_argument("Patient ID must be non-negative");
        }
    }

    int get_patient_id() const
    {
        return patient_id_;
    }

    const std::string& get_name() const
    {
        return name_;
    }

    // Expose procedure requests for tests
    LinkedList<ProcedureRequest>& get_procedures()
    {
        return procedures_;
    }

    const LinkedList<ProcedureRequest>& get_procedures() const
    {
        return procedures_;
    }

    void add_procedure(const ProcedureRequest& procedure)
    {
        procedures_.push_back(procedure);
    }

    // Patient score = sum of all procedure request scores
    int get_score() const
    {
        int total = 0;

        auto* node = const_cast<LinkedList<ProcedureRequest>&>(procedures_).begin();
        while (node != nullptr) {
            total += node->data.get_score();
            node = node->next;
        }

        return total;
    }

    friend bool operator==(const Patient& a, const Patient& b)
    {
        return a.patient_id_ == b.patient_id_;
    }

    friend bool operator!=(const Patient& a, const Patient& b)
    {
        return !(a == b);
    }
};