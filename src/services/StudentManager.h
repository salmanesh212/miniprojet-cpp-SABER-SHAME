#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H

#include <algorithm>
#include <cctype>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "../models/Student.h"
#include "../exceptions/StudentExceptions.hpp"

class StudentManager {
private:
    std::map<int, std::shared_ptr<Student>> students;

public:
    void add(std::shared_ptr<Student> s) {
        if (students.find(s->getId()) != students.end()) {
            throw DuplicateIDException(s->getId());
        }
        students[s->getId()] = s;
    }

    void remove(int id) {
        auto it = students.find(id);
        if (it == students.end()) {
            throw StudentNotFoundException(id);
        }
        students.erase(it);
    }

    void update(int id, std::shared_ptr<Student> updated) {
        auto it = students.find(id);
        if (it == students.end()) {
            throw StudentNotFoundException(id);
        }
        if (updated->getId() != id) {
            throw InvalidIDException(updated->getId());
        }
        students[id] = updated;
    }

    std::shared_ptr<Student> findById(int id) const {
        auto it = students.find(id);
        if (it == students.end()) {
            throw StudentNotFoundException(id);
        }
        return it->second;
    }

    std::vector<std::shared_ptr<Student>> findByName(const std::string& n) const {
        std::vector<std::shared_ptr<Student>> result;
        auto it = students.begin();

        while ((it = std::find_if(it, students.end(), [&n](const auto& pair) {
            std::string fullName = pair.second->getName();
            std::string searchTerm = n;

            std::transform(fullName.begin(), fullName.end(), fullName.begin(), ::tolower);
            std::transform(searchTerm.begin(), searchTerm.end(), searchTerm.begin(), ::tolower);

            return fullName.find(searchTerm) != std::string::npos;
        })) != students.end()) {
            result.push_back(it->second);
            ++it;
        }
        return result;
    }

    std::vector<std::shared_ptr<Student>> sortByGpa() const {
        std::vector<std::shared_ptr<Student>> result;
        for (const auto& pair : students) {
            result.push_back(pair.second);
        }
        std::sort(result.begin(), result.end(), [](const std::shared_ptr<Student>& a, const std::shared_ptr<Student>& b) {
            return a->getGpa() > b->getGpa();
        });
        return result;
    }

    std::vector<std::shared_ptr<Student>> sortByName() const {
        std::vector<std::shared_ptr<Student>> result;
        for (const auto& pair : students) {
            result.push_back(pair.second);
        }
        std::sort(result.begin(), result.end(), [](const std::shared_ptr<Student>& a, const std::shared_ptr<Student>& b) {
            std::string nameA = a->getName();
            std::string nameB = b->getName();
            std::transform(nameA.begin(), nameA.end(), nameA.begin(), ::tolower);
            std::transform(nameB.begin(), nameB.end(), nameB.begin(), ::tolower);
            return nameA < nameB;
        });
        return result;
    }

    double averageGpa() const {
        if (students.empty()) return 0.0;
        double sum = 0.0;
        for (const auto& pair : students) {
            sum += pair.second->getGpa();
        }
        return sum / students.size();
    }

    int count() const { return static_cast<int>(students.size()); }

    int countByType(const std::string& type) const {
        return static_cast<int>(std::count_if(students.begin(), students.end(), [&type](const auto& pair) {
            return pair.second->getType() == type;
        }));
    }

    const std::map<int, std::shared_ptr<Student>>& getAll() const { return students; }
};

#endif // STUDENTMANAGER_H
