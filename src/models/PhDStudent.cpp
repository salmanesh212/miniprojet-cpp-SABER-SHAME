#include "PhDStudent.h"
#include <iostream>

using namespace std;

PhDStudent::PhDStudent(const string& name, int id, float gpa, const string& supervisor, int year)
    : Student(name, id, gpa), supervisor(supervisor), year(year) {}

void PhDStudent::display() const {
    cout << "ID: " << id << " | Name: " << name << " | GPA: " << gpa 
         << " | Superviseur: " << supervisor << " | Année: " << year 
         << " | Boursier: " << computeScholarship() << endl;
}

double PhDStudent::computeScholarship() const {
    return 1500.0;
}

string PhDStudent::getType() const {
    return "Doctorat";
}
