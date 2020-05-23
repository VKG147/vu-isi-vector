#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include "average.h"
#include "person.h"

#ifndef STUDENT_H
#define STUDENT_H

using std::string; using std::vector;

class Student : public Person {
private:
	string name_;
	string surname_;
	vector<int> hwGrades_;
	int examGrade_;
	double finalGrade_;
public:
	Student() : examGrade_(0) {};
	Student(const string& name, const string& surname, const vector<int>& hwGrades, const int& examGrade) :
		name_(name), surname_(surname), hwGrades_(hwGrades), examGrade_(examGrade) { }
	Student(std::istream& is);
	~Student();

	Student(const Student&);
	Student& operator=(const Student&);
	Student& operator=(Student&&) = default;

	operator bool() const;
	double operator+(const Student&);
	double operator-(const Student&);
	double operator*(const Student&);

	inline string name() const { return name_; }
	inline string surname() const { return surname_; }
	inline vector<int> hwGrades() const { return hwGrades_; }
	inline int examGrade() const { return examGrade_; }
	inline double finalGrade() const { return finalGrade_; }
	void computeFinalGrade(double (*getAverage)(vector<int>) = getMean);

	std::istream& readStudent(std::istream&);
};

bool compareByName(const Student&, const Student&);
bool compareByFinal(const Student&, const Student&);

#endif