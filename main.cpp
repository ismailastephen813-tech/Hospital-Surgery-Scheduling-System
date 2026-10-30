/*
clang++ -std=c++17 -Wall -Wextra -Wpedantic -O0 -g \
  main.cpp \
  -o surgery_app

./surgery_app

*/

#include "Patient.h"
#include "PriorityQueue.h"
#include "ProcedureRequest.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>

// Global queue as in your starter
PriorityQueue<Patient> queue;

// -------------------------- helpers --------------------------

static void clear_stdin_line()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int read_int_in_range(const std::string& prompt, int lo, int hi)
{
    while (true) {
        std::cout << prompt;
        int x;
        if (std::cin >> x) {
            clear_stdin_line();
            if (x >= lo && x <= hi) return x;
        }
        else {
            std::cin.clear();
            clear_stdin_line();
        }

        std::cout
            << "Invalid input. Please enter a number between "
            << lo << " and " << hi << ".\n";
    }
}

static std::string read_nonempty_line(const std::string& prompt)
{
    while (true) {
        std::cout << prompt;
        std::string s;
        std::getline(std::cin, s);

        // trim basic
        while (!s.empty() &&
            (s.back() == '\r' ||
                s.back() == '\n' ||
                s.back() == ' ' ||
                s.back() == '\t'))
        {
            s.pop_back();
        }

        size_t i = 0;
        while (i < s.size() &&
            (s[i] == ' ' || s[i] == '\t'))
        {
            ++i;
        }

        s = s.substr(i);

        if (!s.empty()) return s;

        std::cout << "Input cannot be empty. Try again.\n";
    }
}

static void print_banner()
{
    std::cout
        << "************************************************************\n"
        << "*                                                          *\n"
        << "*   Fanshawe College Hospital Surgery Scheduling System    *\n"
        << "*   <Your Name Goes Here>                                  *\n"
        << "*                                                          *\n"
        << "************************************************************\n\n";
}

static void print_menu()
{
    std::cout
        << "Please make a selection:\n\n"
        << "1 - Add Patient\n"
        << "2 - Schedule Next Patient\n"
        << "3 - Display Surgery Waiting List\n"
        << "4 - View Scheduled Patient History\n"
        << "5 - Load Queue\n"
        << "6 - Save Queue\n"
        << "0 - Exit\n\n";
}

static void print_patient_summary(const Patient& p)
{
    std::cout
        << "Patient ID: " << p.get_patient_id()
        << " | " << p.get_name()
        << " | score=" << p.get_score()
        << "\n";

    // list procedure requests
    auto& procedures =
        const_cast<LinkedList<ProcedureRequest>&>(
            p.get_procedures()
            );

    auto* node = procedures.begin();
    int idx = 1;

    while (node != nullptr) {
        const auto& procedure = node->data;

        std::cout
            << "   " << idx++ << ") "
            << procedure.get_name()
            << " [urgency=" << procedure.get_urgency()
            << ", complexity=" << procedure.get_complexity()
            << ", waiting=" << procedure.get_waiting_days()
            << ", score=" << procedure.get_score()
            << "]\n";

        node = node->next;
    }
}

// -------------------------- features --------------------------

static void add_patient_interactive()
{
    std::cout << "\n--- Add Patient ---\n";

    int patient_id =
        read_int_in_range("Patient ID: ", 1, 999999);

    std::string name =
        read_nonempty_line("Patient name: ");

    Patient p(patient_id, name);

    if (queue.find_patient_in_queue(p)) {
        std::cout << "A patient with that ID is already in the queue.\n\n";
        return;
    }

    int count = read_int_in_range("Number of procedure requests (1-20): ", 1, 20);
    for (int i = 0; i < count; ++i) {
        std::cout << "\nProcedure " << (i + 1) << ":\n";
        std::string procedure_name = read_nonempty_line("Procedure name: ");
        int urgency = read_int_in_range("Urgency (0-100): ", 0, 100);
        int complexity = read_int_in_range("Complexity (0-100): ", 0, 100);
        int waiting_days = read_int_in_range("Waiting days (0-10000): ", 0, 10000);
        p.add_procedure(ProcedureRequest(procedure_name, urgency, complexity, waiting_days));
    }

    queue.enqueue(p);

    std::cout << "\nEnqueued patient:\n";
    print_patient_summary(p);
    std::cout << "\n";
}

static void schedule_next_patient(std::vector<Patient>& history)
{
    std::cout << "\n--- Schedule Next Patient ---\n";

    if (queue.empty()) {
        std::cout << "Queue is empty. Nothing to schedule.\n\n";
        return;
    }

    Patient next = queue.dequeue();
    history.push_back(next);

    std::cout << "Scheduled patient:\n";
    print_patient_summary(next);
    std::cout << "\n";
}

static void display_queue()
{
    std::cout << "\n--- Current Queue ---\n";

    if (queue.empty()) {
        std::cout << "Queue is empty.\n\n";
        return;
    }

    // Iterate through internal linked list
    auto& list = queue.get_patients();
    auto* node = list.begin();
    int pos = 1;

    while (node != nullptr) {
        std::cout << pos++ << ". ";
        print_patient_summary(node->data);
        node = node->next;
    }

    std::cout << "\n";
}

static void display_history(const std::vector<Patient>& history)
{
    std::cout << "\n--- Scheduled Patient History ---\n";

    if (history.empty()) {
        std::cout << "No patients scheduled yet.\n\n";
        return;
    }

    for (size_t i = 0; i < history.size(); ++i) {
        std::cout << (i + 1) << ". ";
        print_patient_summary(history[i]);
    }

    std::cout << "\n";
}

static std::vector<std::string> split_csv(const std::string& line)
{
    std::vector<std::string> fields;
    std::string field;
    bool quoted = false;
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            if (quoted && i + 1 < line.size() && line[i + 1] == '"') {
                field += '"';
                ++i;
            } else {
                quoted = !quoted;
            }
        } else if (c == ',' && !quoted) {
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }
    fields.push_back(field);
    return fields;
}

static std::string csv_escape(const std::string& value)
{
    if (value.find_first_of(",\"\r\n") == std::string::npos) return value;
    std::string result = "\"";
    for (char c : value) {
        if (c == '"') result += "\"\"";
        else result += c;
    }
    result += '"';
    return result;
}

static void load_queue_from_file(const std::string& path)
{
    std::cout << "\n--- Load Queue ---\n";
    std::ifstream input(path);
    if (!input) {
        std::cout << "Could not open file: " << path << "\n\n";
        return;
    }

    std::vector<Patient> loaded;
    std::string line;
    bool first_line = true;
    int rows = 0;
    while (std::getline(input, line)) {
        if (line.empty()) continue;
        auto fields = split_csv(line);
        if (first_line && !fields.empty() && fields[0] == "patient_id") {
            first_line = false;
            continue;
        }
        first_line = false;
        if (fields.size() < 2) continue;
        try {
            int id = std::stoi(fields[0]);
            auto found = std::find_if(loaded.begin(), loaded.end(),
                [id](const Patient& patient) { return patient.get_patient_id() == id; });
            if (found == loaded.end()) {
                loaded.emplace_back(id, fields[1]);
                found = loaded.end() - 1;
            }
            if (fields.size() >= 6 && !fields[2].empty()) {
                found->add_procedure(ProcedureRequest(
                    fields[2], std::stoi(fields[3]), std::stoi(fields[4]), std::stoi(fields[5])));
            }
            ++rows;
        } catch (const std::exception&) {
            std::cout << "Skipping invalid CSV row: " << line << "\n";
        }
    }

    int added = 0;
    for (auto& patient : loaded) {
        unsigned before = queue.size();
        queue.enqueue(patient);
        if (queue.size() > before) ++added;
    }
    std::cout << "Loaded " << rows << " rows; added " << added
              << " unique patients to the queue.\n\n";
}

static void save_queue_to_file(const std::string& path)
{
    std::cout << "\n--- Save Queue ---\n";
    std::ofstream output(path);
    if (!output) {
        std::cout << "Could not write to file: " << path << "\n\n";
        return;
    }

    output << "patient_id,name,procedure_name,urgency,complexity,waiting_days\n";
    auto* node = queue.get_patients().begin();
    while (node != nullptr) {
        const Patient& patient = node->data;
        auto* procedure = const_cast<LinkedList<ProcedureRequest>&>(patient.get_procedures()).begin();
        if (procedure == nullptr) {
            output << patient.get_patient_id() << "," << csv_escape(patient.get_name())
                   << ",,0,0,0\n";
        }
        while (procedure != nullptr) {
            const auto& p = procedure->data;
            output << patient.get_patient_id() << "," << csv_escape(patient.get_name())
                   << "," << csv_escape(p.get_name()) << "," << p.get_urgency()
                   << "," << p.get_complexity() << "," << p.get_waiting_days() << "\n";
            procedure = procedure->next;
        }
        node = node->next;
    }

    if (!output) {
        std::cout << "An error occurred while writing the file.\n\n";
        return;
    }
    std::cout << "Queue saved to: " << path << "\n\n";
}

// -------------------------- main --------------------------

int main()
{
    std::vector<Patient> scheduled_history;

    print_banner();

    while (true) {
        print_menu();

        int choice =
            read_int_in_range("Selection: ", 0, 6);

        switch (choice) {
        case 1:
            add_patient_interactive();
            break;

        case 2:
            schedule_next_patient(scheduled_history);
            break;

        case 3:
            display_queue();
            break;

        case 4:
            display_history(scheduled_history);
            break;

        case 5: {
            std::string path =
                read_nonempty_line(
                    "Enter file path to load (e.g., patients.csv): "
                );

            load_queue_from_file(path);
            break;
        }

        case 6: {
            std::string path =
                read_nonempty_line(
                    "Enter file path to save (e.g., patients.csv): "
                );

            save_queue_to_file(path);
            break;
        }

        case 0:
            std::cout << "Goodbye!\n";

            return 0;

        default:
            break;
        }
    }
}