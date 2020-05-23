#include "studentCompute.h"
#include <algorithm>
#include <string>

void genRandomStudents(vector<Student>& students, int student_count, int grade_count, RandomGenerator* generator, int start_index)
{
	students.clear();
	
	string dName = "Vardas";
	string dSurname = "Pavarde";

	for (int i = 0; i < student_count; ++i)
	{
		vector<int> hwGrades;
		hwGrades.reserve(grade_count);

		string suffix = std::to_string(start_index+i);

		for (int j = 0; j < grade_count; ++j)
		{
			int r_grade = generator->getRandom(1, 10);
			hwGrades.push_back(r_grade);
		}

		int examGrade = generator->getRandom(1, 10);

		Student student(dName + suffix, dSurname + suffix, hwGrades, examGrade);

		students.push_back(student);
	}
}
