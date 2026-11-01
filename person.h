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
    Person(const Person& other);              // rule of three
    Person& operator=(const Person& other);
    ~Person();

    const std::string& getName() const { return name; }
    const std::string& getSurname() const { return surname; }
    double getFinal() const { return finalGrade; }

    friend std::istream& operator>>(std::istream& in, Person& p);
    friend std::ostream& operator<<(std::ostream& out, const Person& p);

private:
    std::string name, surname;
    std::vector<int> homework;
    int exam;
    double finalGrade;
    void calculateFinal();
};

#endif
