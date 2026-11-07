#ifndef STUDENTOPS_H
#define STUDENTOPS_H

// Template functions: the same code works with std::vector, std::list and std::deque.
// Templates must be written in a header file.

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include "Person.h"

template <typename Container>
void readStudents(const std::string& fileName, Container& students) {
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
        } catch (const std::exception& e) {
            if (++skipped <= 5) std::cerr << "Skipped: " << e.what() << '\n';
        }
    }
    if (skipped > 0) std::cerr << skipped << " bad lines skipped\n";
    if (students.empty()) throw std::runtime_error("No students in " + fileName);
}

template <typename Container>
void sortStudents(Container& students) {
    auto byGrade = [](const Person& a, const Person& b) { return a.getFinalAvg() > b.getFinalAvg(); };
    if constexpr (std::is_same_v<Container, std::list<Person>>)
        students.sort(byGrade);                              // std::list has its own sort
    else
        std::sort(students.begin(), students.end(), byGrade);
}

// Sorted best first -> failed students are at the end. Move them out.
template <typename Container>
void splitStudents(Container& students, Container& failed) {
    auto firstFailed = std::find_if(students.begin(), students.end(),
                                    [](const Person& s) { return s.getFinalAvg() < 5.0; });
    if constexpr (std::is_same_v<Container, std::list<Person>>) {
        failed.splice(failed.end(), students, firstFailed, students.end());  // no copy at all
    } else {
        failed.assign(std::make_move_iterator(firstFailed),
                      std::make_move_iterator(students.end()));
        students.erase(firstFailed, students.end());
    }
}

template <typename Container>
void writeStudents(const std::string& fileName, const Container& students) {
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

#endif
