#include "Student.h"
#include "../exceptions/StudentExceptions.hpp"
#include <iostream>

using namespace std;

Student::Student(const string& name, int id, float gpa)
    : name(name), id(id) {
    if (id < 1000) {
        throw InvalidIDException(id);
    }
    setGpa(gpa);
}

Student::~Student() {}

void Student::setName(const string& n) {
    name = n;
}

void Student::setGpa(float g) {
    if (g < 0.0f || g > 4.0f) {
        throw InvalidGradeException(g);
    }
    gpa = g;
}
