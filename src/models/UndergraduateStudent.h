#ifndef UNDERGRADUATESTUDENT_H
#define UNDERGRADUATESTUDENT_H

#include "Student.h"
#include <string>

class UndergraduateStudent : public Student {
private:
    std::string major;

public:
    UndergraduateStudent(const std::string& name, int id, float gpa, const std::string& major);
    void display() const override;
    double computeScholarship() const override;
    std::string getType() const override;
    std::string getMajor() const { return major; }
};

#endif // UNDERGRADUATESTUDENT_H
