#ifndef EXECUTION
#define EXECUTION

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

extern bool errorExists;

extern void report(int line, std::string where, std::string message);
extern void error(int line, std::string message);
extern void run(std::string allFileData);
extern void promptCode();
extern void runFile(const std::string &file);

#endif