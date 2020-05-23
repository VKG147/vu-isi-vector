#include "randomGenerator.h"
#include "student.h"
#include <vector>
#include <string>

#ifndef STUDENTIO_H
#define STUDENTIO_H

using std::string;  using std::vector;

void getInput(vector<Student>& students, RandomGenerator* generator);
int getInputFromFile(vector<Student>& students, string path);
int writeStudentsToFile(const vector<Student> students, string path, bool append = false);

void handleInput(string prompt_text, int& input, bool isGrade = false);
bool promptChoice(string prompt_text);
void printStudents(const vector<Student> students);

#endif
