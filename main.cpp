#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Person {
private:
    std::string firstName;
    std::string lastName;
    std::vector<double> homeworkScores;
    double examScore;
    double finalGrade;
    std::string method;

public:
    Person() : firstName(""), lastName(""), examScore(0.0), finalGrade(0.0) {
    }

    Person(std::string fName, std::string lName, double exam)
        : firstName(fName), lastName(lName), examScore(exam), finalGrade(0.0) {
    }

    // Rule of three: copy constructor
    Person(const Person& other)
        : firstName(other.firstName),
          lastName(other.lastName),
          homeworkScores(other.homeworkScores),
          examScore(other.examScore),
          finalGrade(other.finalGrade),
          method(other.method) {
    }

    // Rule of three: copy assignment operator
    Person& operator=(const Person& other) {
        if (this != &other) {
            firstName = other.firstName;
            lastName = other.lastName;
            homeworkScores = other.homeworkScores;
            examScore = other.examScore;
            finalGrade = other.finalGrade;
            method = other.method;
        }
        return *this;
    }

    // Rule of three: destructor
    ~Person() {
    }

    const std::string& getFirstName() const { return firstName; }
    const std::string& getLastName() const { return lastName; }

    // Input overload (keyboard)
    friend std::istream& operator>>(std::istream& in, Person& p) {
        std::cout << "First name: ";
        in >> p.firstName;
        std::cout << "Last name: ";
        in >> p.lastName;

        p.homeworkScores.clear();
        char more = 'y';
        while (more == 'y' || more == 'Y') {
            double score;
            std::cout << "Enter homework score: ";
            in >> score;
            p.homeworkScores.push_back(score);
            std::cout << "Add another homework score? (y/n): ";
            in >> more;
        }

        std::cout << "Exam score: ";
        in >> p.examScore;
        return in;
    }

    // Output overload: one table row (Average | Median)
    friend std::ostream& operator<<(std::ostream& out, const Person& p) {
        out << std::left << std::setw(12) << p.firstName
            << std::setw(14) << p.lastName
            << std::right << std::fixed << std::setprecision(2)
            << std::setw(14) << p.averageGrade()
            << " | "
            << std::setw(11) << p.medianGrade();
        return out;
    }

    double averageGrade() const {
        double sum = 0.0;
        for (double s : homeworkScores) sum += s;
        double avg = homeworkScores.empty() ? 0.0 : sum / homeworkScores.size();
        return 0.4 * avg + 0.6 * examScore;
    }

    double medianGrade() const {
        std::vector<double> sorted = homeworkScores;
        std::sort(sorted.begin(), sorted.end());
        size_t n = sorted.size();
        double med = 0.0;
        if (n > 0) {
            if (n % 2 == 0) med = (sorted[n / 2 - 1] + sorted[n / 2]) / 2.0;
            else med = sorted[n / 2];
        }
        return 0.4 * med + 0.6 * examScore;
    }

    void calculateFinalAverage() {
        finalGrade = averageGrade();
        method = "Average";
    }

    void calculateFinalMedian() {
        finalGrade = medianGrade();
        method = "Median";
    }

    void generateRandom(int numHomework) {
        homeworkScores.clear();
        for (int i = 0; i < numHomework; i++) {
            homeworkScores.push_back(rand() % 11);
        }
        examScore = rand() % 11;
    }

    // Read one line from the file: Name Surname HW1 ... HWn Exam
    bool readFromLine(const std::string& line) {
        std::istringstream ss(line);
        if (!(ss >> firstName >> lastName)) return false;
        std::vector<double> values;
        double v;
        while (ss >> v) values.push_back(v);
        if (values.empty()) return false;
        examScore = values.back();
        values.pop_back();
        homeworkScores = values;
        return true;
    }
};

int main() {
    std::ifstream file("Students.txt");
    if (!file) {
        std::cout << "Error: cannot open Students.txt\n";
        return 1;
    }

    std::vector<Person> students;
    std::string line;
    std::getline(file, line); // skip the header line

    while (std::getline(file, line)) {
        Person p;
        if (p.readFromLine(line)) {
            students.push_back(p);
        }
    }
    file.close();

    // Sort by surname, then by name
    std::sort(students.begin(), students.end(),
              [](const Person& a, const Person& b) {
                  if (a.getLastName() != b.getLastName())
                      return a.getLastName() < b.getLastName();
                  return a.getFirstName() < b.getFirstName();
              });

    std::cout << std::left << std::setw(12) << "Name"
              << std::setw(14) << "Surname"
              << std::right << std::setw(14) << "Final (Avg.)"
              << " | " << std::setw(11) << "Final (Med.)" << "\n";
    std::cout << std::string(53, '-') << "\n";

    for (const Person& p : students) {
        std::cout << p << "\n";
    }

    return 0;
}