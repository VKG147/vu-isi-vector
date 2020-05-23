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
	vector<int> testSizes = { 1000000 };

	std::chrono::duration<double> diff;

	std::cout << "===std::vector testavimas===\n";
    for (auto it_size = testSizes.begin(); it_size != testSizes.end(); ++it_size)
    { // One test

        int realloc_count = 0;

        // Generating
        vector<Student> students;
        auto t1 = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 10; ++i)
        {
            genRandomStudents(students, 1000000, grade_count, generator, realloc_count, i * 1000);
        }
        std::cout << realloc_count << std::endl;

        auto t2 = std::chrono::high_resolution_clock::now();
        diff = t2 - t1;
        std::cout << "Laikas " << *it_size << " studentu generavimui su std::vector: " << diff.count() << "s\n";
        students.clear();
    }
}
