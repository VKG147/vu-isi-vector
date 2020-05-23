#include "student.h"
#include "studentIO.h"
#include "studentCompute.h"
#include "randomGenerator.h"
#include <string>
#include <Vector>
#include <chrono>
#include <iostream>
#include <algorithm>
#include "Vector.h"

using std::string;

RandomGenerator* generator;

void generateTestData(string path, int student_count, int grade_count, RandomGenerator* generator);
void runTests(RandomGenerator* generator);

int main()
{
	generator = new RandomGenerator();

	bool isTest = promptChoice("Paleisti testus? (t/n)\n");

	if (isTest)
	{
		runTests(generator);
	}
	else
	{
		Vector<Student> students;

		bool readFromFile = promptChoice("Skaityti duomenis is failo (kursiokai.txt)? (t/n)\n");

		if (readFromFile)
			getInputFromFile(students, "kursiokai.txt");
		else
			getInput(students, generator);

		std::sort(students.begin(), students.end(), compareByName);

		printStudents(students);
	}

	delete generator;
	return 0;
}

void runTests(RandomGenerator* generator)
{
	const string testPath = "test.txt";
	const double gradeBound = 5.0;
	const int grade_count = 5;
	Vector<int> testSizes = { 10000000 };

	std::chrono::duration<double> diff;

	std::cout << "===Vector testavimas===\n";
	for (auto it_size = testSizes.begin(); it_size != testSizes.end(); ++it_size)
	{ // One test

		// Generating
		Vector<Student> students;
        auto t1 = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1; ++i)
        {
            genRandomStudents(students, 1000000, grade_count, generator, i * 1000000);
        }
        auto t2 = std::chrono::high_resolution_clock::now();
        diff = t2 - t1;
        std::cout << "Laikas " << *it_size << " studentu generavimui su Vector: " << diff.count() << "s\n";
        students.clear();
	}
}

void generateTestData(string path, int student_count, int grade_count, RandomGenerator* generator)
{
	Vector<Student> students;
	genRandomStudents(students, student_count, grade_count, generator);
	writeStudentsToFile(students, path);
}