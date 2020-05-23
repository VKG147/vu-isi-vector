#include "student.h"
#include "randomGenerator.h"
#include "Vector.h"
#include <string>

#ifndef STUDENTCOMPUTE_H
#define STUDENTCOMPUTE_H

using std::string;

void genRandomStudents(Vector<Student>& students, int student_count, int grade_count, RandomGenerator* generator, int start_index = 1);

#endif
