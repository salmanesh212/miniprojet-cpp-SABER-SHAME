#ifndef PERSISTENCEMANAGER_H
#define PERSISTENCEMANAGER_H

#include <string>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <memory>
#include "../services/StudentManager.h"
#include "../models/UndergraduateStudent.h"
#include "../models/GraduateStudent.h"
#include "../models/PhDStudent.h"

class PersistenceManager {
public:
    inline static void saveToJson(const StudentManager& manager, const QString& fileName) {
        QJsonArray studentArray;
        for (const auto& pair : manager.getAll()) {
            auto student = pair.second;
            QJsonObject studentObj;
            studentObj["id"] = student->getId();
            studentObj["name"] = QString::fromStdString(student->getName());
            studentObj["gpa"] = student->getGpa();
            studentObj["type"] = QString::fromStdString(student->getType());

            if (student->getType() == "Licence") {
                if (auto ug = dynamic_cast<UndergraduateStudent*>(student.get())) {
                    studentObj["major"] = QString::fromStdString(ug->getMajor());
                }
            } else if (student->getType() == "Master") {
                if (auto grad = dynamic_cast<GraduateStudent*>(student.get())) {
                    studentObj["researchTopic"] = QString::fromStdString(grad->getResearchTopic());
                }
            } else if (student->getType() == "Doctorat") {
                if (auto phd = dynamic_cast<PhDStudent*>(student.get())) {
                    studentObj["supervisor"] = QString::fromStdString(phd->getSupervisor());
                    studentObj["yearOfStudy"] = phd->getYearOfStudy();
                }
            }

            studentArray.append(studentObj);
        }
        QJsonObject mainObj;
        mainObj["students"] = studentArray;

        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            QJsonDocument doc(mainObj);
            file.write(doc.toJson());
            file.close();
        }
    }

    inline static void loadFromJson(StudentManager& manager, const QString& fileName) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            file.close();

            QJsonDocument doc(QJsonDocument::fromJson(data));
            if (doc.isObject()) {
                QJsonObject mainObj = doc.object();
                QJsonArray studentArray = mainObj["students"].toArray();
                for (int i = 0; i < studentArray.size(); ++i) {
                    QJsonObject obj = studentArray[i].toObject();
                    int id = obj["id"].toInt();
                    std::string name = obj["name"].toString().toStdString();
                    float gpa = obj["gpa"].toDouble();
                    std::string type = obj["type"].toString().toStdString();

                    try {
                        if (type == "Licence") {
                            std::string major = obj["major"].toString("").toStdString();
                            manager.add(std::make_shared<UndergraduateStudent>(name, id, gpa, major));
                        } else if (type == "Master") {
                            std::string researchTopic = obj["researchTopic"].toString("").toStdString();
                            manager.add(std::make_shared<GraduateStudent>(name, id, gpa, researchTopic));
                        } else if (type == "Doctorat") {
                            std::string supervisor = obj["supervisor"].toString("").toStdString();
                            int yearOfStudy = obj["yearOfStudy"].toInt(1);
                            manager.add(std::make_shared<PhDStudent>(name, id, gpa, supervisor, yearOfStudy));
                        }
                    } catch (const std::exception&) {
                        // Ignore exceptions for duplicate entries
                    }
                }
            }
        }
    }

    // Aliases pour compatibilité avec les appels save()/load()
    inline static void save(const StudentManager& manager, const std::string& fileName) {
        saveToJson(manager, QString::fromStdString(fileName));
    }

    inline static void load(StudentManager& manager, const std::string& fileName) {
        loadFromJson(manager, QString::fromStdString(fileName));
    }
};

#endif // PERSISTENCEMANAGER_H
