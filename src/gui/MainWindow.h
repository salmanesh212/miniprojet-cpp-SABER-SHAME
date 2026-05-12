// src/gui/MainWindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QAction>
#include <memory>
#include "../services/StudentManager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    // CRUD
    void updateTable();
    void onAddClicked();
    void onRemoveClicked();
    void onUpdateClicked();
    void onViewDetails();

    // Recherche & Filtres
    void applyFilters();

    // Bonus
    void onExportCsv();
    void onShowStats();
    void onToggleDarkMode();
    void onTableContextMenu(const QPoint& pos);

private:
    std::shared_ptr<StudentManager> manager;
    bool darkMode;

    // Widgets principaux
    QTableWidget* table;
    QPushButton*  btnAdd;
    QPushButton*  btnRemove;
    QPushButton*  btnUpdate;
    QPushButton*  btnDetails;
    QPushButton*  btnRefresh;
    QLabel*       statusLabel;

    // Filtres avancés
    QLineEdit*      searchBox;
    QComboBox*      filterType;
    QDoubleSpinBox* filterGpaMin;
    QDoubleSpinBox* filterGpaMax;

    // Actions menu
    QAction* actionDarkMode;

    void setupUi();
    void setupMenuBar();
    void loadData();
    void saveData();
    void updateStatusBar();
    void populateTable(const std::vector<std::shared_ptr<Student>>& students);
    int  getSelectedStudentId() const;
    void applyStyleSheet();
    std::vector<std::shared_ptr<Student>> getFilteredStudents() const;
};

#endif // MAINWINDOW_H
