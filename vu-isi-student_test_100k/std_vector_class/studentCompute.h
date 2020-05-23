#include "student.h"
#include "randomGenerator.h"
#include <vector>
#include <string>

#ifndef STUDENTCOMPUTE_H
#define STUDENTCOMPUTE_H

using std::string; using std::vector;

void genRandomStudents(vector<Student>& students, int student_count, int grade_count, RandomGenerator* generator, int start_index = 1);

#endif
