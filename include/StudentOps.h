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

// true if Container is std::list<Person>
template <typename Container>
constexpr bool isList = std::is_same_v<Container, std::list<Person>>;

inline bool isFailed(const Person& s) { return s.getFinalAvg() < 5.0; }

// ---------------------------------------------------------------- read
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
        } catch (const std::bad_alloc&) {
            throw;                                    // out of memory: stop
        } catch (const std::exception& e) {
            if (++skipped <= 5) std::cerr << "Skipped: " << e.what() << '\n';
        }
    }
    if (skipped > 0) std::cerr << skipped << " bad lines skipped\n";
    if (students.empty()) throw std::runtime_error("No students in " + fileName);
}

// ---------------------------------------------------------------- sort (best grade first)
template <typename Container>
void sortStudents(Container& students) {
    auto byGrade = [](const Person& a, const Person& b) { return a.getFinalAvg() > b.getFinalAvg(); };
    if constexpr (isList<Container>)
        students.sort(byGrade);                       // std::list has its own sort
    else
        std::sort(students.begin(), students.end(), byGrade);
}

// ---------------------------------------------------------------- Strategy 1
// Copy every student into one of TWO new containers.
// 'students' keeps all data -> uses the most memory.
template <typename Container>
void splitStrategy1(const Container& students, Container& failed, Container& passed) {
    for (const Person& s : students) {
        if (isFailed(s)) failed.push_back(s);         // copy
        else             passed.push_back(s);         // copy
    }
}

// ---------------------------------------------------------------- Strategy 2 (simple loop)
// Move failed students to ONE new container and erase them from 'students'.
// After this, 'students' contains only passed students.
// erase() in the middle is slow for vector and deque (all next elements are shifted).
template <typename Container>
void splitStrategy2(Container& students, Container& failed) {
    auto it = students.begin();
    while (it != students.end()) {
        if (isFailed(*it)) {
            failed.push_back(std::move(*it));
            it = students.erase(it);                  // "painful" for vector / deque
        } else {
            ++it;
        }
    }
    if constexpr (!isList<Container>)
        students.shrink_to_fit();                     // resize to the new smaller size
}

// ---------------------------------------------------------------- Strategy 3 (Strategy 2 + <algorithm>)
// The list is sorted (best first), so all failed students are at the end.
//   std::find_if   -> finds the first failed student
//   std::move      -> moves the failed block to 'failed' in one operation
//   erase(first,last) -> removes the whole block at once (no shifting)
template <typename Container>
void splitStrategy3(Container& students, Container& failed) {
    auto firstFailed = std::find_if(students.begin(), students.end(), isFailed);
    if constexpr (isList<Container>) {
        failed.splice(failed.end(), students, firstFailed, students.end());  // no copy at all
    } else {
        std::move(firstFailed, students.end(), std::back_inserter(failed));
        students.erase(firstFailed, students.end());
        students.shrink_to_fit();
    }
}

// ---------------------------------------------------------------- Strategy 4 (unsorted data)
// Same result, but works even if the data is NOT sorted:
//   std::stable_partition -> puts passed students first, failed last (order kept)
//   std::move + erase     -> moves the failed part out
template <typename Container>
void splitStrategy4(Container& students, Container& failed) {
    auto firstFailed = std::stable_partition(students.begin(), students.end(),
                                             [](const Person& s) { return !isFailed(s); });
    if constexpr (isList<Container>) {
        failed.splice(failed.end(), students, firstFailed, students.end());
    } else {
        std::move(firstFailed, students.end(), std::back_inserter(failed));
        students.erase(firstFailed, students.end());
        students.shrink_to_fit();
    }
}

// ---------------------------------------------------------------- write
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
