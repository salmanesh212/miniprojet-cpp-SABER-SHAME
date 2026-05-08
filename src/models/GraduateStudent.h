#ifndef GRADUATESTUDENT_H
#define GRADUATESTUDENT_H

#include "Student.h"
#include <string>

class GraduateStudent : public Student {
private:
    std::string researchTopic;

public:
    GraduateStudent(const std::string& name, int id, float gpa, const std::string& researchTopic);
    void display() const override;
    double computeScholarship() const override;
    std::string getType() const override;
    std::string getResearchTopic() const { return researchTopic; }
};

#endif // GRADUATESTUDENT_H
