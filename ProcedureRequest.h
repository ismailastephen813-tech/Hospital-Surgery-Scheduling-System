#pragma once

#include <string>
#include <stdexcept>
#include <utility>

class ProcedureRequest {
    std::string procedure_name_{};
    int urgency_{ 0 };
    int complexity_{ 0 };
    int waiting_days_{ 0 };

public:
    ProcedureRequest() = default;

    ProcedureRequest(std::string procedure_name, int urgency,
        int complexity, int waiting_days)
        : procedure_name_(std::move(procedure_name)),
        urgency_(urgency),
        complexity_(complexity),
        waiting_days_(waiting_days)
    {
        if (urgency_ < 0 || complexity_ < 0 || waiting_days_ < 0) {
            throw std::invalid_argument(
                "Procedure request values must be non-negative");
        }
    }

    const std::string& get_name() const {
        return procedure_name_;
    }

    int get_urgency() const {
        return urgency_;
    }

    int get_complexity() const {
        return complexity_;
    }

    int get_waiting_days() const {
        return waiting_days_;
    }

    // Priority score: (Urgency × 3) + Complexity + Waiting Days
    int get_score() const {
        return (urgency_ * 3) + complexity_ + waiting_days_;
    }

    friend bool operator==(const ProcedureRequest& a,
        const ProcedureRequest& b) {
        return a.procedure_name_ == b.procedure_name_
            && a.urgency_ == b.urgency_
            && a.complexity_ == b.complexity_
            && a.waiting_days_ == b.waiting_days_;
    }

    friend bool operator!=(const ProcedureRequest& a,
        const ProcedureRequest& b) {
        return !(a == b);
    }
};