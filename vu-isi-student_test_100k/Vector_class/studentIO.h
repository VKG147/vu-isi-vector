#include "randomGenerator.h"
#include "student.h"
#include "Vector.h"
#include <string>

#ifndef STUDENTIO_H
#define STUDENTIO_H

using std::string;

void getInput(Vector<Student>& students, RandomGenerator* generator);
int getInputFromFile(Vector<Student>& students, string path);
int writeStudentsToFile(const Vector<Student> students, string path, bool append = false);

void handleInput(string prompt_text, int& input, bool isGrade = false);
bool promptChoice(string prompt_text);
void printStudents(const Vector<Student> students);

#endif
