#include "student.h"
#include "studentIO.h"
#include "studentCompute.h"
#include "randomGenerator.h"
#include <string>
#include <vector>
#include <chrono>
#include <iostream>
#include <algorithm>

using std::string; using std::vector;

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
		vector<Student> students;

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
	vector<int> testSizes = { 100000 };

	std::chrono::duration<double> diff;

	std::cout << "===std::vector testavimas===\n";
	for (auto it_size = testSizes.begin(); it_size != testSizes.end(); ++it_size)
	{ // One test
		
		// Generating
		vector<Student> students;
		students.reserve(*it_size);
		if (*it_size == 10000000)
		{
			for (int i = 0; i < 10; ++i)
			{
				genRandomStudents(students, 1000000, grade_count, generator, i * 1000);
				writeStudentsToFile(students, testPath, true);
			}
		}
		else
		{
			genRandomStudents(students, *it_size, grade_count, generator);
			writeStudentsToFile(students, testPath);
		}
		students.clear();
		
		// Reading
		auto t1 = std::chrono::high_resolution_clock::now();
		getInputFromFile(students, testPath);
		auto t2 = std::chrono::high_resolution_clock::now();
		diff = t2 - t1;
		std::cout << "Laikas " << *it_size << " studentu failo nuskaitymui: " << diff.count() << "s\n";

		// Computing finals
		for (auto it = students.begin(); it != students.end(); ++it) {
			it->computeFinalGrade();
		}

		// Sorting
		t1 = std::chrono::high_resolution_clock::now();
		std::sort(students.begin(), students.end(), compareByFinal);
		t2 = std::chrono::high_resolution_clock::now();
		diff = t2 - t1;
		std::cout << "Laikas " << *it_size << " studentu rusiavimui: " << diff.count() << "s\n";

		// Seperating into two groups
		t1 = std::chrono::high_resolution_clock::now();
		vector<Student> studentsA;
		auto it_s = students.end() - 1;
		while (it_s->finalGrade() < gradeBound) {
			studentsA.push_back(*it_s);
			it_s--;
			students.pop_back();

			if (it_s == students.begin()) {
				break;
			}
		}
		t2 = std::chrono::high_resolution_clock::now();
		diff = t2 - t1;
		std::cout << "Laikas " << *it_size << " studentu paskirstymo i atskirus std::vector: " << diff.count() << "s\n\n";
	}
}

void generateTestData(string path, int student_count, int grade_count, RandomGenerator* generator)
{
	vector<Student> students;
	genRandomStudents(students, student_count, grade_count, generator);
	writeStudentsToFile(students, path);
}