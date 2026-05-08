#include "GraduateStudent.h"
#include <iostream>

using namespace std;

GraduateStudent::GraduateStudent(const string& name, int id, float gpa, const string& researchTopic)
    : Student(name, id, gpa), researchTopic(researchTopic) {}

void GraduateStudent::display() const {
    cout << "ID: " << id << " | Name: " << name << " | GPA: " << gpa 
         << " | Sujet de recherche: " << researchTopic << " | Boursier: " << computeScholarship() << endl;
}

double GraduateStudent::computeScholarship() const {
    return (gpa >= 3.0) ? 1000.0 : 0.0;
}

string GraduateStudent::getType() const {
    return "Master";
}
