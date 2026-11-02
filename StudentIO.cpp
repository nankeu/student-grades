#include "StudentIO.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <random>
#include <sstream>
#include <stdexcept>

bool fileExists(const std::string& fileName) {
    std::ifstream f(fileName);
    return f.good();
}

// Same format as the teacher's files:
// Vardas  Pavarde  ND1 ... NDn  Egz.
void generateFile(const std::string& fileName, int count, int hwCount) {
    if (count <= 0 || hwCount <= 0)
        throw std::invalid_argument("Count must be positive");

    std::ofstream out(fileName);
    if (!out) throw std::runtime_error("Cannot create file: " + fileName);

    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> grade(1, 10);

    std::ostringstream buf;      // build in memory, write in big blocks (faster)
    buf << std::left << std::setw(25) << "Vardas" << std::setw(25) << "Pavarde";
    for (int i = 1; i <= hwCount; ++i) buf << std::right << std::setw(10) << ("ND" + std::to_string(i));
    buf << std::setw(10) << "Egz." << '\n';

    for (int i = 1; i <= count; ++i) {
        buf << std::left << std::setw(25) << ("Vardas" + std::to_string(i))
            << std::setw(25) << ("Pavarde" + std::to_string(i)) << std::right;
        for (int j = 0; j <= hwCount; ++j)          // hwCount homework + 1 exam
            buf << std::setw(10) << grade(gen);
        buf << '\n';

        if (i % 100000 == 0) {                      // flush every 100 000 lines
            out << buf.str();
            buf.str("");
        }
    }
    out << buf.str();
}

void readStudents(const std::string& fileName, std::vector<Person>& students) {
    std::ifstream in(fileName);
    if (!in) throw std::runtime_error("File not found: " + fileName);

    std::string header;
    if (!std::getline(in, header)) throw std::runtime_error("File is empty: " + fileName);

    int skipped = 0;
    while (true) {
        try {
            Person p;
            if (!(in >> p)) break;
            students.push_back(std::move(p));
        } catch (const std::exception& e) {          // bad line: skip it, continue
            if (++skipped <= 5) std::cerr << "Skipped: " << e.what() << '\n';
        }
    }
    if (skipped > 0) std::cerr << skipped << " bad lines skipped\n";
    if (students.empty()) throw std::runtime_error("No students in " + fileName);
}

// Best grade first
void sortStudents(std::vector<Person>& students) {
    std::sort(students.begin(), students.end(),
              [](const Person& a, const Person& b) { return a.getFinalAvg() > b.getFinalAvg(); });
}

// The list is already sorted (best first), so all "failed" students are at the end.
// We move them into 'failed' and remove them from 'students' -> 'students' keeps only "passed".
void splitStudents(std::vector<Person>& students, std::vector<Person>& failed) {
    auto firstFailed = std::find_if(students.begin(), students.end(),
                                    [](const Person& s) { return s.getFinalAvg() < 5.0; });
    failed.assign(std::make_move_iterator(firstFailed),
                  std::make_move_iterator(students.end()));
    students.erase(firstFailed, students.end());
}

void writeStudents(const std::string& fileName, const std::vector<Person>& students) {
    std::ofstream out(fileName);
    if (!out) throw std::runtime_error("Cannot create file: " + fileName);

    std::ostringstream buf;
    buf << std::left << std::setw(20) << "Surname" << std::setw(20) << "Name"
        << std::setw(16) << "Final (Avg.)" << "Final (Med.)\n";
    int i = 0;
    for (const Person& s : students) {
        buf << s << '\n';
        if (++i % 100000 == 0) { out << buf.str(); buf.str(""); }
    }
    out << buf.str();
}
