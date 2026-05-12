// src/gui/StudentDetailsDialog.cpp
#include "StudentDetailsDialog.h"
#include "../models/UndergraduateStudent.h"
#include "../models/GraduateStudent.h"
#include "../models/PhDStudent.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QGroupBox>
#include <QFormLayout>
#include <sstream>
#include <iostream>

StudentDetailsDialog::StudentDetailsDialog(std::shared_ptr<Student> student, QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Détails de l'étudiant");
    setMinimumSize(500, 400);
    setupUI(student);
}

void StudentDetailsDialog::setupUI(std::shared_ptr<Student> student) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Informations communes
    QGroupBox *groupCommon = new QGroupBox("Informations Communes");
    QFormLayout *formCommon = new QFormLayout(groupCommon);

    formCommon->addRow("ID:", new QLabel(QString::number(student->getId())));
    formCommon->addRow("Nom:", new QLabel(QString::fromStdString(student->getName())));
    formCommon->addRow("GPA:", new QLabel(QString::number(student->getGpa(), 'f', 2)));
    formCommon->addRow("Type:", new QLabel(QString::fromStdString(student->getType())));
    formCommon->addRow("Bourse:", new QLabel(QString::number(student->computeScholarship(), 'f', 2) + " EUR"));

    mainLayout->addWidget(groupCommon);

    // Informations spécifiques selon le type
    QGroupBox *groupSpecific = new QGroupBox("Informations Spécifiques");
    auto formLayout = new QFormLayout();
    groupSpecific->setLayout(formLayout);

    if (student->getType() == "Licence") {
        if (auto ug = dynamic_cast<UndergraduateStudent*>(student.get())) {
            QLabel *labelMajor = new QLabel("Filière :", groupSpecific);
            QLabel *valueMajor = new QLabel(QString::fromStdString(ug->getMajor()), groupSpecific);
            valueMajor->setStyleSheet("font-weight: bold;");
            formLayout->addRow(labelMajor, valueMajor);
        }
    } else if (student->getType() == "Master") {
        if (auto grad = dynamic_cast<GraduateStudent*>(student.get())) {
            QLabel *labelTopic = new QLabel("Sujet de Recherche :", groupSpecific);
            QLabel *valueTopic = new QLabel(QString::fromStdString(grad->getResearchTopic()), groupSpecific);
            valueTopic->setStyleSheet("font-weight: bold;");
            formLayout->addRow(labelTopic, valueTopic);
        }
    } else if (student->getType() == "Doctorat") {
        if (auto phd = dynamic_cast<PhDStudent*>(student.get())) {
            QLabel *labelSupv = new QLabel("Encadrant :", groupSpecific);
            QLabel *valueSupv = new QLabel(QString::fromStdString(phd->getSupervisor()), groupSpecific);
            valueSupv->setStyleSheet("font-weight: bold;");
            formLayout->addRow(labelSupv, valueSupv);

            QLabel *labelYear = new QLabel("Année d'Étude :", groupSpecific);
            QLabel *valueYear = new QLabel(QString::number(phd->getYearOfStudy()), groupSpecific);
            valueYear->setStyleSheet("font-weight: bold;");
            formLayout->addRow(labelYear, valueYear);
        }
    }

    mainLayout->addWidget(groupSpecific);

    // Affichage polymorph
    QGroupBox *groupDisplay = new QGroupBox("Affichage Formaté");
    QLabel *displayText = new QLabel();
    displayText->setTextFormat(Qt::PlainText);
    displayText->setWordWrap(true);
    displayText->setAlignment(Qt::AlignTop);
    
    std::stringstream ss;
    ss << "[" << student->getType() << "] "
       << "ID: " << student->getId() << " | "
       << "Nom: " << student->getName() << " | "
       << "GPA: " << student->getGpa() << " | "
       << "Bourse: " << student->computeScholarship() << " EUR";
    
    displayText->setText(QString::fromStdString(ss.str()));
    
    QVBoxLayout *displayLayout = new QVBoxLayout(groupDisplay);
    displayLayout->addWidget(displayText);
    mainLayout->addWidget(groupDisplay);

    mainLayout->addStretch();
}


