#include "StatsDialog.h"
#include <QColor>
#include <QPainter>
#include <QStringList>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QValueAxis>
#include <memory>

using namespace std;

StatsDialog::StatsDialog(std::shared_ptr<StudentManager> manager, QWidget *parent) : QDialog(parent) {
    setWindowTitle("Statistiques des Étudiants");
    setMinimumSize(500, 400);
    setupUI(manager);
}

void StatsDialog::setupUI(std::shared_ptr<StudentManager> manager) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    int totalStudents = manager->count();
    double avgGpa = manager->averageGpa();
    
    layout->addWidget(new QLabel("<b>Total Étudiants :</b> " + QString::number(totalStudents)));
    layout->addWidget(new QLabel("<b>GPA Moyen :</b> " + QString::number(avgGpa, 'f', 2)));
    
    layout->addWidget(new QLabel("<b>Répartition par type :</b>"));
    layout->addWidget(new QLabel("- Licence : " + QString::number(manager->countByType("Licence"))));
    layout->addWidget(new QLabel("- Master : " + QString::number(manager->countByType("Master"))));
    layout->addWidget(new QLabel("- Doctorat : " + QString::number(manager->countByType("Doctorat"))));

    setupChart(manager, layout);
}

void StatsDialog::setupChart(std::shared_ptr<StudentManager> manager, QVBoxLayout* layout) {
    int range1 = 0, range2 = 0, range3 = 0, range4 = 0;
    
    for (const auto& pair : manager->getAll()) {
        float gpa = pair.second->getGpa();
        if (gpa >= 0.0f && gpa < 1.0f) range1++;
        else if (gpa >= 1.0f && gpa < 2.0f) range2++;
        else if (gpa >= 2.0f && gpa < 3.0f) range3++;
        else if (gpa >= 3.0f && gpa <= 4.0f) range4++;
    }

    auto *set0 = new QBarSet("Étudiants");
    set0->setColor(QColor(59, 130, 246));
    *set0 << range1 << range2 << range3 << range4;

    auto *series = new QBarSeries();
    series->append(set0);

    auto *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Distribution des GPA");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QStringList categories;
    categories << "[0-1[" << "[1-2[" << "[2-3[" << "[3-4]";
    
    auto *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    auto *axisY = new QValueAxis();
    axisY->setLabelFormat("%d");
    axisY->setMin(0.0);
    axisY->setTitleText("Nombre d'étudiants");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    auto *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumHeight(250);

    layout->addWidget(chartView);
}
