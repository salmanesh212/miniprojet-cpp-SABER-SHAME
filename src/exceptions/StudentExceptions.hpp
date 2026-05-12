#ifndef STUDENTEXCEPTIONS_HPP
#define STUDENTEXCEPTIONS_HPP

#include <exception>
#include <string>

class InvalidIDException : public std::exception {
private:
    std::string msg;
public:
    InvalidIDException(int id)
        : msg("ID invalide (doit etre >= 1000) : " + std::to_string(id)) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

class InvalidGradeException : public std::exception {
private:
    std::string msg;
public:
    InvalidGradeException(float gpa)
        : msg("GPA hors plage [0.0 - 4.0] : " + std::to_string(gpa)) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

class StudentNotFoundException : public std::exception {
private:
    std::string msg;
public:
    StudentNotFoundException(int id)
        : msg("Etudiant introuvable avec ID : " + std::to_string(id)) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

class DuplicateIDException : public std::exception {
private:
    std::string msg;
public:
    DuplicateIDException(int id)
        : msg("ID deja utilise : " + std::to_string(id)) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

#endif // STUDENTEXCEPTIONS_HPP
