#ifndef STUDENTIO_H
#define STUDENTIO_H

#include <string>

bool fileExists(const std::string& fileName);
void generateFile(const std::string& fileName, int count, int hwCount = 7);

#endif
