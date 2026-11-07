#include "StudentIO.h"

#include <fstream>
#include <iomanip>
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

