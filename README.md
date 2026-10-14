*******************************************************************************/
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

class Person {
private:
    std::string firstName;
    std::string lastName;
    std::vector<double> homeworkScores;
    double examScore;
    double finalGrade;
    std::string method; // "Average" or "Median"

public:
    // Default constructor
    Person() : firstName(""), lastName(""), examScore(0.0), finalGrade(0.0) {
    }

    // Parameterized constructor
    Person(std::string fName, std::string lName, double exam)
        : firstName(fName), lastName(lName), examScore(exam), finalGrade(0.0) {
    }

    // Copy constructor
    Person(const Person& other)
        : firstName(other.firstName),
          lastName(other.lastName),
          homeworkScores(other.homeworkScores),
          examScore(other.examScore),
          finalGrade(other.finalGrade) {
    }

    // Copy assignment operator
    Person& operator=(const Person& other) {
        if (this != &other) {
            firstName = other.firstName;
            lastName = other.lastName;
            homeworkScores = other.homeworkScores;
            examScore = other.examScore;
            finalGrade = other.finalGrade;
        }
        return *this;
    }

    // Destructor
    ~Person() {
    }

    // Input overload
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

    // Output overload (formatted for the table)
    friend std::ostream& operator<<(std::ostream& out, const Person& p) {
        out << std::left << std::setw(12) << p.firstName
            << std::setw(14) << p.lastName
            << std::right << std::setw(10) << std::fixed << std::setprecision(2)
            << p.finalGrade;
        return out;
    }

    // Calculate final grade using average of homework
    void calculateFinalAverage() {
        double sum = 0.0;
        for (double score : homeworkScores) {
            sum += score;
        }
        double homeworkAvg = (homeworkScores.empty()) ? 0.0 : sum / homeworkScores.size();

        finalGrade = 0.4 * homeworkAvg + 0.6 * examScore;
        method = "Average";
    }

    // Calculate final grade using median of homework
    void calculateFinalMedian() {
        std::vector<double> sorted = homeworkScores;
        std::sort(sorted.begin(), sorted.end());

        double homeworkMedian;
        size_t n = sorted.size();
        if (n == 0) {
            homeworkMedian = 0.0;
        } else if (n % 2 == 0) {
            homeworkMedian = (sorted[n / 2 - 1] + sorted[n / 2]) / 2.0;
        } else {
            homeworkMedian = sorted[n / 2];
        }

        finalGrade = 0.4 * homeworkMedian + 0.6 * examScore;
        method = "Median";
    }

    // Randomly generate homework scores and exam score
    void generateRandom(int numHomework) {
        homeworkScores.clear();
        for (int i = 0; i < numHomework; i++) {
            homeworkScores.push_back(rand() % 11); // 0 to 10
        }
        examScore = rand() % 11; // 0 to 10
    }
};

int main() {
    srand(static_cast<unsigned int>(time(0)));

    int numStudents;
    std::cout << "How many students? ";
    std::cin >> numStudents;

    std::vector<Person> students(numStudents);

    std::cout << "Generate random scores instead of manual input? (y/n): ";
    char randomChoice;
    std::cin >> randomChoice;

    for (int i = 0; i < numStudents; i++) {
        std::cout << "\n--- Student " << (i + 1) << " ---\n";
        if (randomChoice == 'y' || randomChoice == 'Y') {
            std::string fName, lName;
            std::cout << "First name: ";
            std::cin >> fName;
            std::cout << "Last name: ";
            std::cin >> lName;
            students[i] = Person(fName, lName, 0.0);
            students[i].generateRandom(5); // 5 random homework scores
        } else {
            std::cin >> students[i];
        }
    }

    std::cout << "\nChoose calculation method - (a)verage or (m)edian: ";
    char choice;
    std::cin >> choice;

    std::string methodLabel;
    for (Person& p : students) {
        if (choice == 'm' || choice == 'M') {
            p.calculateFinalMedian();
            methodLabel = "Final_Point(Med.)";
        } else {
            p.calculateFinalAverage();
            methodLabel = "Final_Point(Aver.)";
        }
    }

    std::cout << "\n" << std::left << std::setw(12) << "Name"
               << std::setw(14) << "Surname"
               << std::right << std::setw(10) << methodLabel << "\n";
    std::cout << std::string(43, '-') << "\n";

    for (const Person& p : students) {
        std::cout << p << "\n";
    }

    return 0;
}
