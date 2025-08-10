#ifndef EXECUTION
#define EXECUTION

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "scanner.h"

extern bool errorExists;

extern void report(const int& line, const std::string& where, const std::string& message);
extern void error(const int& line, const std::string& message);
extern void error(Token token, const std::string& message);
extern void run(const std::string& allFileData);
extern void promptCode();
extern void runFile(const std::string &file);

#endif