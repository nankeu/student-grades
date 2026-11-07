#include <deque>
#include <iomanip>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "Person.h"
#include "StudentIO.h"
#include "StudentOps.h"
#include "Timer.h"

template <typename Container>
void runTest(int n, const std::string& type) {
    std::string file = "students" + std::to_string(n) + ".txt";
    Container students, failed;
    Timer total, t;

    readStudents(file, students);
    double tRead = t.elapsed(); t.reset();

    sortStudents(students);
    double tSort = t.elapsed(); t.reset();

    splitStudents(students, failed);              // students now = passed
    double tSplit = t.elapsed(); t.reset();

    writeStudents(type + "_failed" + std::to_string(n) + ".txt", failed);
    writeStudents(type + "_passed" + std::to_string(n) + ".txt", students);
    double tWrite = t.elapsed();

    std::cout << std::left << std::setw(8) << type << std::setw(12) << n
              << std::fixed << std::setprecision(3)
              << std::setw(10) << tRead << std::setw(10) << tSort
              << std::setw(10) << tSplit << std::setw(10) << tWrite
              << total.elapsed() << std::endl;
}

int main() {
    const std::vector<int> sizes = {1000, 10000, 100000, 1000000, 10000000};

    std::cout << "1 - Generate missing test files (students1000.txt ... students10000000.txt)\n"
              << "2 - Run speed test (vector, list, deque)\n"
              << "Choice: ";
    int choice;
    if (!(std::cin >> choice) || (choice != 1 && choice != 2)) {
        std::cerr << "Invalid choice\n";
        return 1;
    }

    if (choice == 2)
        std::cout << "\n" << std::left << std::setw(8) << "Type" << std::setw(12) << "Records"
                  << std::setw(10) << "Read" << std::setw(10) << "Sort" << std::setw(10) << "Split"
                  << std::setw(10) << "Write" << "Total  (seconds)\n";

    for (int n : sizes) {
        std::string file = "students" + std::to_string(n) + ".txt";
        try {
            if (choice == 1) {
                if (fileExists(file)) {
                    std::cout << file << " already exists - skipped\n";
                    continue;
                }
                Timer t;
                generateFile(file, n);
                std::cout << file << " generated in " << t.elapsed() << " s\n";
            } else {
                runTest<std::vector<Person>>(n, "vector");
                runTest<std::list<Person>>(n, "list");
                runTest<std::deque<Person>>(n, "deque");
                std::cout << '\n';
            }
        } catch (const std::exception& e) {
            std::cerr << "Error with " << file << ": " << e.what() << '\n';
        }
    }
    return 0;
}
