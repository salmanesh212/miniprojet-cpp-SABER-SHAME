// src/gui/StudentDetailsDialog.h
#ifndef STUDENTDETAILSDIALOG_H
#define STUDENTDETAILSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <memory>
#include "../models/Student.h"

class StudentDetailsDialog : public QDialog {
    Q_OBJECT

public:
    explicit StudentDetailsDialog(std::shared_ptr<Student> student, QWidget *parent = nullptr);

private:
    void setupUI(std::shared_ptr<Student> student);
};

#endif // STUDENTDETAILSDIALOG_H
