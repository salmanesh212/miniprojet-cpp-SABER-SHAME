#ifndef PHDSTUDENT_H
#define PHDSTUDENT_H

#include "Student.h"
#include <string>

class PhDStudent : public Student {
private:
    std::string supervisor;
    int year;

public:
    PhDStudent(const std::string& name, int id, float gpa, const std::string& supervisor, int year);
    void display() const override;
    double computeScholarship() const override;
    std::string getType() const override;
    std::string getSupervisor() const { return supervisor; }
    int getYearOfStudy() const { return year; }
};

#endif // PHDSTUDENT_H
