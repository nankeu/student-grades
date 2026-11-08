#include <deque>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <new>
#include <stdexcept>
#include <string>
#include <vector>

#include "Person.h"
#include "StudentIO.h"
#include "StudentOps.h"
#include "Timer.h"

const std::vector<int> SIZES = {1000, 10000, 100000, 1000000, 10000000};

// Strategy 2 erases one by one: for vector/deque this is O(n^2).
// Above this size it would take hours, so it is skipped.
const int STRATEGY2_LIMIT = 100000;

// Reads a number between min and max, throws if the input is wrong
int readChoice(const std::string& question, int min, int max) {
    std::cout << question;
    int x;
    if (!(std::cin >> x)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("Please enter a number");
    }
    if (x < min || x > max)
        throw std::out_of_range("Choose a number from " + std::to_string(min) +
                                " to " + std::to_string(max));
    return x;
}

void printRow(const std::string& type, int strategy, int n,
              double tRead, double tSort, double tSplit, double tWrite, double tTotal) {
    std::cout << std::left << std::setw(8) << type << std::setw(10) << strategy
              << std::setw(11) << n << std::fixed << std::setprecision(3)
              << std::setw(10) << tRead << std::setw(10) << tSort
              << std::setw(10) << tSplit << std::setw(10) << tWrite
              << tTotal << std::endl;
}

template <typename Container>
void runTest(int n, int strategy, const std::string& type) {
    if (strategy == 2 && !isList<Container> && n > STRATEGY2_LIMIT) {
        std::cout << std::left << std::setw(8) << type << std::setw(10) << strategy
                  << std::setw(11) << n << "skipped (erase one by one is O(n^2), too slow)\n";
        return;
    }

    std::string file = "students" + std::to_string(n) + ".txt";
    std::string failedFile = "failed" + std::to_string(n) + ".txt";
    std::string passedFile = "passed" + std::to_string(n) + ".txt";

    Container students, failed, passed;
    Timer total, t;

    readStudents(file, students);
    double tRead = t.elapsed(); t.reset();

    sortStudents(students);
    double tSort = t.elapsed(); t.reset();

    switch (strategy) {
        case 1: splitStrategy1(students, failed, passed); break;
        case 2: splitStrategy2(students, failed); break;
        case 3: splitStrategy3(students, failed); break;
        case 4: splitStrategy4(students, failed); break;
    }
    double tSplit = t.elapsed(); t.reset();

    writeStudents(failedFile, failed);
    writeStudents(passedFile, strategy == 1 ? passed : students);
    double tWrite = t.elapsed();

    printRow(type, strategy, n, tRead, tSort, tSplit, tWrite, total.elapsed());
}

void speedTest() {
    int container = readChoice("\nContainer: 1 - vector, 2 - list, 3 - deque, 4 - all three: ", 1, 4);
    int strategy  = readChoice("Strategy:\n"
                               "  1 - copy to two new containers (failed + passed)\n"
                               "  2 - move failed out, erase one by one\n"
                               "  3 - strategy 2 with <algorithm> (find_if + move + erase)\n"
                               "  4 - strategy 2 with <algorithm> (stable_partition + move + erase)\n"
                               "  5 - all strategies\n"
                               "Choice: ", 1, 5);
    int maxIndex  = readChoice("Largest file: 1 - 1 000, 2 - 10 000, 3 - 100 000, "
                               "4 - 1 000 000, 5 - 10 000 000: ", 1, 5);

    std::cout << "\n" << std::left << std::setw(8) << "Type" << std::setw(10) << "Strategy"
              << std::setw(11) << "Records" << std::setw(10) << "Read" << std::setw(10) << "Sort"
              << std::setw(10) << "Split" << std::setw(10) << "Write" << "Total  (seconds)\n";

    for (int i = 0; i < maxIndex; ++i) {
        int n = SIZES[i];
        for (int s = 1; s <= 4; ++s) {
            if (strategy != 5 && s != strategy) continue;
            try {
                if (container == 1 || container == 4) runTest<std::vector<Person>>(n, s, "vector");
                if (container == 2 || container == 4) runTest<std::list<Person>>(n, s, "list");
                if (container == 3 || container == 4) runTest<std::deque<Person>>(n, s, "deque");
            } catch (const std::bad_alloc&) {
                std::cerr << "Not enough memory for " << n << " records (strategy " << s << ")\n";
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << '\n';
            }
        }
        std::cout << '\n';
    }
}

void generateFiles() {
    for (int n : SIZES) {
        std::string file = "students" + std::to_string(n) + ".txt";
        try {
            if (fileExists(file)) {
                std::cout << file << " already exists - skipped\n";
                continue;
            }
            Timer t;
            generateFile(file, n);
            std::cout << file << " generated in " << t.elapsed() << " s\n";
        } catch (const std::exception& e) {
            std::cerr << "Error with " << file << ": " << e.what() << '\n';
        }
    }
}

int main() {
    while (true) {
        try {
            int choice = readChoice("\n=== Student Grades v1.0 ===\n"
                                    "1 - Generate missing test files\n"
                                    "2 - Speed test\n"
                                    "0 - Exit\n"
                                    "Choice: ", 0, 2);
            if (choice == 0) break;
            if (choice == 1) generateFiles();
            if (choice == 2) speedTest();
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << '\n';
        }
    }
    return 0;
}
