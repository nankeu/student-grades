#ifndef STUDENTIO_H
#define STUDENTIO_H

#include <string>
#include <vector>
#include "Person.h"

void generateFile(const std::string& fileName, int count, int hwCount = 5);
void readStudents(const std::string& fileName, std::vector<Person>& students);
void sortStudents(std::vector<Person>& students);
void splitStudents(const std::vector<Person>& all,
                   std::vector<Person>& failed, std::vector<Person>& passed);
void writeStudents(const std::string& fileName, const std::vector<Person>& students);

#endif
