#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <string>
#include <vector>

class Person {
public:
    Person();
    Person(const std::string& name, const std::string& surname,
           const std::vector<int>& homework, int exam);

    // Rule of three
    Person(const Person& other);
    Person& operator=(const Person& other);
    ~Person();

    // Move (makes moving 10 000 000 students fast)
    Person(Person&& other) noexcept;
    Person& operator=(Person&& other) noexcept;

    const std::string& getName() const { return name; }
    const std::string& getSurname() const { return surname; }
    double getFinalAvg() const { return finalAvg; }
    double getFinalMed() const { return finalMed; }

    friend std::istream& operator>>(std::istream& in, Person& p);
    friend std::ostream& operator<<(std::ostream& out, const Person& p);

private:
    std::string name;
    std::string surname;
    std::vector<int> homework;
    int exam;
    double finalAvg;
    double finalMed;

    void calculateFinal();
};

#endif
