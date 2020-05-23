#pragma once

#include <string>

using std::string;

class Person {
public:
	virtual string name() const = 0;
	virtual string surname() const = 0;
};