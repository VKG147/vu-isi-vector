#include "student.h"

Student::Student(std::istream& is) {
	readStudent(is);
}

Student::Student(const Student& s) {
	this->name_ = s.name_;
	this->surname_ = s.surname_;
	this->hwGrades_ = s.hwGrades_;
	this->examGrade_ = s.examGrade_;
	this->finalGrade_ = s.finalGrade_;
}

Student::~Student() {
	hwGrades_.clear();
}

Student& Student::operator=(const Student& s) {
	if (&s == this) return *this;
	*this = Student(s);
	return *this;
}

Student::operator bool() const {
	return (this->finalGrade_ >= 4);
}
double Student::operator+(const Student& s) {
	return this->finalGrade_ + s.finalGrade_;
}
double Student::operator-(const Student& s) {
	return this->finalGrade_ - s.finalGrade_;
}
double Student::operator*(const Student& s) {
	return this->finalGrade_ * s.finalGrade_;
}

void Student::computeFinalGrade(double (*getAverage)(vector<int>)) {
	finalGrade_ = 0.4 * getAverage(hwGrades_) + 0.6 * this->examGrade_;
}

std::istream& Student::readStudent(std::istream& is) {
	try
	{
		is >> name_ >> surname_;

		while (!is.eof())
		{
			int grade;
			is >> grade;

			this->hwGrades_.push_back(grade);
		}

		this->examGrade_ = this->hwGrades_.back(); // Last grade - exam
		this->hwGrades_.pop_back();
	}
	catch (std::exception& e)
	{
		std::cout << "Studento skaitymo klaida" << std::endl;
	}

	return is;
}


bool compareByName(const Student& a, const Student& b) {
	return (a.name() < b.name());
}

bool compareByFinal(const Student& a, const Student& b) {
	return (a.finalGrade() > b.finalGrade());
}