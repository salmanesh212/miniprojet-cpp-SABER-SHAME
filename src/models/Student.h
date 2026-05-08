#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <stdexcept>
#include "../exceptions/StudentExceptions.hpp"

class Student {
protected:
    std::string name;
    int id;
    float gpa;

public:
    Student(const std::string& name, int id, float gpa);
    virtual ~Student();

    virtual void display() const = 0;
    virtual std::string getType() const = 0;
    virtual double computeScholarship() const = 0;

    std::string getName() const { 
        if (name.empty()) throw std::invalid_argument("Name cannot be empty");
        return name; 
    }
    int getId() const { 
        if (id < 1000) throw InvalidIDException(id);
        return id; 
    }
    float getGpa() const { 
        if (gpa < 0.0f || gpa > 4.0f) throw InvalidGradeException(gpa);
        return gpa; 
    }

    void setName(const std::string& n);
    void setGpa(float g);
};

#endif // STUDENT_H
