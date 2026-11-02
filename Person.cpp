#include "Person.h"

#include <algorithm>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <utility>

Person::Person() : exam(0), finalAvg(0.0), finalMed(0.0) {}

Person::Person(const std::string& n, const std::string& s,
               const std::vector<int>& hw, int e)
    : name(n), surname(s), homework(hw), exam(e), finalAvg(0.0), finalMed(0.0) {
    calculateFinal();
}

Person::Person(const Person& o)
    : name(o.name), surname(o.surname), homework(o.homework),
      exam(o.exam), finalAvg(o.finalAvg), finalMed(o.finalMed) {}

Person& Person::operator=(const Person& o) {
    if (this != &o) {
        name = o.name;
        surname = o.surname;
        homework = o.homework;
        exam = o.exam;
        finalAvg = o.finalAvg;
        finalMed = o.finalMed;
    }
    return *this;
}

Person::~Person() {}

Person::Person(Person&& o) noexcept
    : name(std::move(o.name)), surname(std::move(o.surname)),
      homework(std::move(o.homework)), exam(o.exam),
      finalAvg(o.finalAvg), finalMed(o.finalMed) {}

Person& Person::operator=(Person&& o) noexcept {
    if (this != &o) {
        name = std::move(o.name);
        surname = std::move(o.surname);
        homework = std::move(o.homework);
        exam = o.exam;
        finalAvg = o.finalAvg;
        finalMed = o.finalMed;
    }
    return *this;
}

void Person::calculateFinal() {
    if (homework.empty())
        throw std::domain_error("No homework grades for " + name + " " + surname);

    double avg = std::accumulate(homework.begin(), homework.end(), 0.0) / homework.size();

    std::vector<int> sorted = homework;
    std::sort(sorted.begin(), sorted.end());
    size_t n = sorted.size();
    double med = (n % 2 == 0) ? (sorted[n / 2 - 1] + sorted[n / 2]) / 2.0
                              : sorted[n / 2];

    finalAvg = 0.4 * avg + 0.6 * exam;
    finalMed = 0.4 * med + 0.6 * exam;
}

// Reads one line: Name Surname HW1 HW2 ... HWn Exam
// (works with any number of homework grades: 7, 15, 20 ...)
std::istream& operator>>(std::istream& in, Person& p) {
    std::string line;
    while (std::getline(in, line))                      // skip empty lines
        if (line.find_first_not_of(" \t\r") != std::string::npos) break;
    if (line.find_first_not_of(" \t\r") == std::string::npos) {
        in.setstate(std::ios::failbit);                 // end of file
        return in;
    }

    std::istringstream ss(line);
    std::string name, surname;
    if (!(ss >> name >> surname))
        throw std::invalid_argument("Bad line: " + line);

    std::vector<int> grades;
    int g;
    while (ss >> g) {
        if (g < 1 || g > 10)
            throw std::out_of_range("Grade not in 1-10: " + line);
        grades.push_back(g);
    }
    if (!ss.eof())
        throw std::invalid_argument("Not a number in line: " + line);
    if (grades.size() < 2)
        throw std::invalid_argument("Need homework and exam: " + line);

    int exam = grades.back();
    grades.pop_back();
    p = Person(name, surname, grades, exam);
    return in;
}

std::ostream& operator<<(std::ostream& out, const Person& p) {
    out << std::left << std::setw(20) << p.surname
        << std::setw(20) << p.name
        << std::fixed << std::setprecision(2)
        << std::setw(16) << p.finalAvg
        << p.finalMed;
    return out;
}
