#include "UndergraduateStudent.h"
#include <iostream>

using namespace std;

UndergraduateStudent::UndergraduateStudent(const string& name, int id, float gpa, const string& major)
    : Student(name, id, gpa), major(major) {}

void UndergraduateStudent::display() const {
    cout << "ID: " << id << " | Name: " << name << " | GPA: " << gpa 
         << " | Filière: " << major << " | Boursier: " << computeScholarship() << endl;
}

double UndergraduateStudent::computeScholarship() const {
    return (gpa >= 3.5) ? 500.0 : 0.0;
}

string UndergraduateStudent::getType() const {
    return "Licence";
}
