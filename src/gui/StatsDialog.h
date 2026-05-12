#ifndef STATSDIALOG_H
#define STATSDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <memory>
#include "../services/StudentManager.h"

class StatsDialog : public QDialog {
    Q_OBJECT

public:
    explicit StatsDialog(std::shared_ptr<StudentManager> manager, QWidget *parent = nullptr);

private:
    void setupUI(std::shared_ptr<StudentManager> manager);
    void setupChart(std::shared_ptr<StudentManager> manager, QVBoxLayout* layout);
};

#endif // STATSDIALOG_H
