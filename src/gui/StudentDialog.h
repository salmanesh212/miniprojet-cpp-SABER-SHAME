// src/gui/StudentDialog.h
#ifndef STUDENTDIALOG_H
#define STUDENTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QComboBox>
#include <QStackedWidget>
#include <QLabel>
#include <memory>
#include "../models/Student.h"
#include "../models/UndergraduateStudent.h"
#include "../models/GraduateStudent.h"
#include "../models/PhDStudent.h"

class StudentDialog : public QDialog {
    Q_OBJECT

public:
    explicit StudentDialog(QWidget *parent = nullptr);

    // Crée et retourne un shared_ptr<Student> selon le type sélectionné.
    // Retourne nullptr si la création échoue (exception interne).
    std::shared_ptr<Student> getStudent() const;

    // Pré-remplit le dialogue avec les données d'un étudiant existant.
    void setStudent(std::shared_ptr<Student> student);

private slots:
    void onTypeChanged(int index);
    void validateAndAccept();

private:
    // Champs communs
    QComboBox*      comboType;
    QLineEdit*      editName;
    QSpinBox*       spinId;
    QDoubleSpinBox* spinGpa;

    // Champs dynamiques (via QStackedWidget)
    QStackedWidget* stackedWidget;
    QLineEdit*      editMajor;          // Licence  → Filière
    QLineEdit*      editResearchTopic;  // Master   → Sujet de recherche
    QLineEdit*      editSupervisor;     // Doctorat → Encadrant
    QSpinBox*       spinYear;           // Doctorat → Année d'étude

    // Labels dynamiques (pour clarté)
    QLabel* labelDynamic;

    void setupUi();
};

#endif // STUDENTDIALOG_H
