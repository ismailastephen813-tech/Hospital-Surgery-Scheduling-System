#pragma once

#include "LinkedList.h"
#include <iostream>
#include <stdexcept>

template <typename T>
class PriorityQueue
{
private:
    LinkedList<T> patients_;

public:
    bool empty() const
    {
        return patients_.empty();
    }

    unsigned size() const
    {
        return patients_.size();
    }

    LinkedList<T>& get_patients()
    {
        return patients_;
    }

    const LinkedList<T>& get_patients() const
    {
        return patients_;
    }

    // Linear search for duplicate patients using operator==
    bool find_patient_in_queue(const T& item)
    {
        auto* node = patients_.begin();

        while (node != nullptr)
        {
            if (node->data == item)
                return true;

            node = node->next;
        }

        return false;
    }

    void enqueue(const T& item)
    {
        // Do not add duplicate patients
        if (find_patient_in_queue(item))
            return;

        // Insert at end, then sort the whole list
        patients_.push_back(item);
        sort_priority();
    }

    T dequeue()
    {
        if (patients_.empty())
        {
            throw std::runtime_error(
                "Attempted to dequeue from an empty queue");
        }

        // Highest-priority patient is at the front after sorting
        return patients_.pop_front();
    }

    // Sort patients by score in descending order.
    // Patients with equal scores must remain in FIFO order.
    void sort_priority()
    {
        // Stable insertion sort by swapping node data. Equal scores are not swapped,
        // so patients with equal priority retain their arrival order.
        if (patients_.empty()) return;

        auto* current = patients_.begin()->next;
        while (current != nullptr) {
            auto* scan = current;
            while (scan->previous != nullptr &&
                   scan->previous->data.get_score() < scan->data.get_score()) {
                T temporary = scan->previous->data;
                scan->previous->data = scan->data;
                scan->data = temporary;
                scan = scan->previous;
            }
            current = current->next;
        }
    }

    void display_queue(std::ostream& os = std::cout)
    {
        auto* node = patients_.begin();

        while (node != nullptr)
        {
            os << node->data.get_name()
                << " (score=" << node->data.get_score() << ")\n";

            node = node->next;
        }
    }
};