#ifndef STUDENTIO_H
#define STUDENTIO_H

#include <string>
#include <vector>
#include "Person.h"

bool fileExists(const std::string& fileName);
void generateFile(const std::string& fileName, int count, int hwCount = 7);
void readStudents(const std::string& fileName, std::vector<Person>& students);
void sortStudents(std::vector<Person>& students);
void splitStudents(std::vector<Person>& students, std::vector<Person>& failed);
void writeStudents(const std::string& fileName, const std::vector<Person>& students);

#endif
