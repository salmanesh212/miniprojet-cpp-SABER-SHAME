// src/gui/MainWindow.cpp
#include "MainWindow.h"
#include "StudentDialog.h"
#include "StudentDetailsDialog.h"
#include "StatsDialog.h"
#include "CsvExporter.h"
#include "../persistence/PersistenceManager.h"
#include "../exceptions/StudentExceptions.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QHeaderView>
#include <QStatusBar>
#include <QMenuBar>
#include <QFileDialog>
#include <QGroupBox>
#include <QFormLayout>
#include <QApplication>
#include <QMenu>
#include <QTableWidgetItem>
#include <QColor>
#include <QList>
#include <algorithm>

using namespace std;

// ═══════════════════════════════════════════════════════════════
// Feuilles de style
// ═══════════════════════════════════════════════════════════════
static const char* STYLE_LIGHT = R"(
    QMainWindow { background: #f5f5f5; }
    QTableWidget { background: white; alternate-background-color: #f0f4ff;
                   gridline-color: #ddd; font-size: 13px; }
    QTableWidget::item:selected { background: #3b82f6; color: white; }
    QHeaderView::section { background: #e2e8f0; padding: 6px;
                           font-weight: bold; border: 1px solid #ccc; }
    QPushButton { background: #3b82f6; color: white; border: none;
                  padding: 7px 16px; border-radius: 4px; font-weight: bold; }
    QPushButton:hover { background: #2563eb; }
    QPushButton:pressed { background: #1d4ed8; }
    QPushButton#btnRemove { background: #ef4444; }
    QPushButton#btnRemove:hover { background: #dc2626; }
    QLineEdit, QComboBox, QDoubleSpinBox, QSpinBox {
        padding: 5px; border: 1px solid #cbd5e1; border-radius: 4px; }
    QGroupBox { font-weight: bold; border: 1px solid #ccc;
                border-radius: 6px; margin-top: 8px; padding-top: 16px; }
    QGroupBox::title { subcontrol-origin: margin; left: 10px; }
    QStatusBar { background: #e2e8f0; }
)";

static const char* STYLE_DARK = R"(
    QMainWindow { background: #1e1e2e; color: #cdd6f4; }
    QTableWidget { background: #313244; alternate-background-color: #45475a;
                   color: #cdd6f4; gridline-color: #585b70; font-size: 13px; }
    QTableWidget::item:selected { background: #89b4fa; color: #1e1e2e; }
    QHeaderView::section { background: #45475a; color: #cdd6f4; padding: 6px;
                           font-weight: bold; border: 1px solid #585b70; }
    QPushButton { background: #89b4fa; color: #1e1e2e; border: none;
                  padding: 7px 16px; border-radius: 4px; font-weight: bold; }
    QPushButton:hover { background: #74c7ec; }
    QPushButton:pressed { background: #b4befe; }
    QPushButton#btnRemove { background: #f38ba8; }
    QPushButton#btnRemove:hover { background: #eba0ac; }
    QLineEdit, QComboBox, QDoubleSpinBox, QSpinBox {
        background: #313244; color: #cdd6f4;
        padding: 5px; border: 1px solid #585b70; border-radius: 4px; }
    QGroupBox { color: #cdd6f4; font-weight: bold; border: 1px solid #585b70;
                border-radius: 6px; margin-top: 8px; padding-top: 16px; }
    QGroupBox::title { subcontrol-origin: margin; left: 10px; color: #89b4fa; }
    QStatusBar { background: #313244; color: #cdd6f4; }
    QMenuBar { background: #313244; color: #cdd6f4; }
    QMenuBar::item:selected { background: #45475a; }
    QMenu { background: #313244; color: #cdd6f4; border: 1px solid #585b70; }
    QMenu::item:selected { background: #89b4fa; color: #1e1e2e; }
    QLabel { color: #cdd6f4; }
    QDialog { background: #1e1e2e; color: #cdd6f4; }
    QMessageBox { background: #1e1e2e; color: #cdd6f4; }
    QProgressBar { background: #45475a; border: 1px solid #585b70; border-radius: 4px;
                   text-align: center; color: #cdd6f4; }
    QProgressBar::chunk { background: #89b4fa; border-radius: 3px; }
)";

// ═══════════════════════════════════════════════════════════════
// Constructeur / Destructeur
// ═══════════════════════════════════════════════════════════════
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , manager(std::make_shared<StudentManager>())
    , darkMode(false)
    , table(nullptr)
    , btnAdd(nullptr), btnRemove(nullptr), btnUpdate(nullptr), btnDetails(nullptr), btnRefresh(nullptr)
    , statusLabel(nullptr)
    , searchBox(nullptr), filterType(nullptr)
    , filterGpaMin(nullptr), filterGpaMax(nullptr)
    , actionDarkMode(nullptr)
{
    setupUi();
    setupMenuBar();
    applyStyleSheet();
    loadData();
    updateTable();
}

MainWindow::~MainWindow() {}

// ═══════════════════════════════════════════════════════════════
// Barre de menu
// ═══════════════════════════════════════════════════════════════
void MainWindow::setupMenuBar() {
    // Menu Fichier
    QMenu* menuFile = menuBar()->addMenu("Fichier");

    QAction* actionExport = menuFile->addAction("Exporter CSV...");
    connect(actionExport, &QAction::triggered, this, &MainWindow::onExportCsv);

    menuFile->addSeparator();
    QAction* actionQuit = menuFile->addAction("Quitter");
    connect(actionQuit, &QAction::triggered, this, &QMainWindow::close);

    // Menu Affichage
    QMenu* menuView = menuBar()->addMenu("Affichage");

    QAction* actionStats = menuView->addAction("Statistiques...");
    connect(actionStats, &QAction::triggered, this, &MainWindow::onShowStats);

    menuView->addSeparator();
    actionDarkMode = menuView->addAction("Mode sombre");
    actionDarkMode->setCheckable(true);
    actionDarkMode->setChecked(false);
    connect(actionDarkMode, &QAction::toggled, this, &MainWindow::onToggleDarkMode);
}

// ═══════════════════════════════════════════════════════════════
// Configuration de l'interface
// ═══════════════════════════════════════════════════════════════
void MainWindow::setupUi() {
    setWindowTitle("Gestion des Etudiants - IMS");
    setMinimumSize(900, 600);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(8);
    mainLayout->setContentsMargins(12, 12, 12, 12);

    // ── Boutons CRUD ─────────────────────────────────────────
    QHBoxLayout* crudBar = new QHBoxLayout();
    crudBar->setSpacing(6);

    btnAdd     = new QPushButton("Ajouter",    this);
    btnUpdate  = new QPushButton("Modifier",   this);
    btnRemove  = new QPushButton("Supprimer",  this);
    btnDetails = new QPushButton("Détails",    this);
    btnRefresh = new QPushButton("Rafraichir", this);

    btnAdd->setObjectName("btnAdd");
    btnUpdate->setObjectName("btnUpdate");
    btnRemove->setObjectName("btnRemove");
    btnDetails->setObjectName("btnDetails");
    btnRefresh->setObjectName("btnRefresh");

    crudBar->addWidget(btnAdd);
    crudBar->addWidget(btnUpdate);
    crudBar->addWidget(btnRemove);
    crudBar->addWidget(btnDetails);
    crudBar->addWidget(btnRefresh);
    crudBar->addStretch();
    mainLayout->addLayout(crudBar);

    // ── Filtres avancés ──────────────────────────────────────
    QGroupBox* filterGroup = new QGroupBox("Filtres", this);
    QHBoxLayout* filterLayout = new QHBoxLayout(filterGroup);
    filterLayout->setSpacing(10);

    // Recherche par nom
    QLabel* lblSearch = new QLabel("Nom :", this);
    searchBox = new QLineEdit(this);
    searchBox->setPlaceholderText("Rechercher...");
    searchBox->setClearButtonEnabled(true);
    filterLayout->addWidget(lblSearch);
    filterLayout->addWidget(searchBox, 2);

    // Filtre par type
    QLabel* lblType = new QLabel("Type :", this);
    filterType = new QComboBox(this);
    filterType->addItems({"Tous", "Licence", "Master", "Doctorat"});
    filterLayout->addWidget(lblType);
    filterLayout->addWidget(filterType);

    // Filtre GPA min / max
    QLabel* lblGpa = new QLabel("GPA :", this);
    filterGpaMin = new QDoubleSpinBox(this);
    filterGpaMin->setRange(0.0, 4.0);
    filterGpaMin->setDecimals(1);
    filterGpaMin->setSingleStep(0.5);
    filterGpaMin->setValue(0.0);
    filterGpaMin->setPrefix("Min ");

    filterGpaMax = new QDoubleSpinBox(this);
    filterGpaMax->setRange(0.0, 4.0);
    filterGpaMax->setDecimals(1);
    filterGpaMax->setSingleStep(0.5);
    filterGpaMax->setValue(4.0);
    filterGpaMax->setPrefix("Max ");

    filterLayout->addWidget(lblGpa);
    filterLayout->addWidget(filterGpaMin);
    filterLayout->addWidget(filterGpaMax);

    mainLayout->addWidget(filterGroup);

    // ── Tableau ──────────────────────────────────────────────
    table = new QTableWidget(0, 5, this);
    table->setHorizontalHeaderLabels({"ID", "Nom", "Type", "GPA", "Bourse"});
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);
    mainLayout->addWidget(table);

    // ── Barre de statut ──────────────────────────────────────
    statusLabel = new QLabel(this);
    statusBar()->addPermanentWidget(statusLabel);

    // ── Connexions ───────────────────────────────────────────
    connect(btnAdd, &QPushButton::clicked, this, [this]() {
        onAddClicked();
    });
    connect(btnRemove, &QPushButton::clicked, this, [this]() {
        onRemoveClicked();
    });
    connect(btnUpdate, &QPushButton::clicked, this, [this]() {
        onUpdateClicked();
    });
    connect(btnDetails, &QPushButton::clicked, this, [this]() {
        onViewDetails();
    });
    connect(btnRefresh, &QPushButton::clicked, this, [this]() {
        updateTable();
        statusBar()->showMessage("Liste rafraichie.", 2000);
    });

    // Menu contextuel sur le tableau
    table->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(table, &QTableWidget::customContextMenuRequested, this, &MainWindow::onTableContextMenu);

    // Filtres temps réel
    connect(searchBox,    &QLineEdit::textChanged,    this, &MainWindow::applyFilters);
    connect(filterType,   QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::applyFilters);
    connect(filterGpaMin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::applyFilters);
    connect(filterGpaMax, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::applyFilters);
}

// ═══════════════════════════════════════════════════════════════
// Filtrage avancé combiné
// ═══════════════════════════════════════════════════════════════
vector<shared_ptr<Student>> MainWindow::getFilteredStudents() const {
    vector<shared_ptr<Student>> result;
    const auto& all = manager->getAll();

    QString keyword = searchBox->text().trimmed().toLower();
    QString typeFilter = filterType->currentText();
    double gpaMin = filterGpaMin->value();
    double gpaMax = filterGpaMax->value();

    for (const auto& pair : all) {
        const auto& s = pair.second;
        if (!s) continue;

        // Filtre par type
        if (typeFilter != "Tous" &&
            QString::fromStdString(s->getType()) != typeFilter) continue;

        // Filtre par GPA
        float gpa = s->getGpa();
        if (gpa < gpaMin || gpa > gpaMax) continue;

        // Filtre par nom (insensible à la casse, partiel)
        if (!keyword.isEmpty()) {
            QString name = QString::fromStdString(s->getName()).toLower();
            if (!name.contains(keyword)) continue;
        }

        result.push_back(s);
    }

    // Tri par GPA décroissant
    std::sort(result.begin(), result.end(),
        [](const shared_ptr<Student>& a, const shared_ptr<Student>& b) {
            return a->getGpa() > b->getGpa();
        });

    return result;
}

void MainWindow::applyFilters() {
    auto filtered = getFilteredStudents();
    populateTable(filtered);

    int shown = static_cast<int>(filtered.size());
    int total = manager->count();
    statusLabel->setText(
        QString("Affiches : %1 / %2  |  Moyenne GPA : %3")
            .arg(shown).arg(total)
            .arg(manager->averageGpa(), 0, 'f', 2));
}

// ═══════════════════════════════════════════════════════════════
// Peuplement du tableau
// ═══════════════════════════════════════════════════════════════
void MainWindow::populateTable(const vector<shared_ptr<Student>>& students) {
    table->setRowCount(0);
    for (const auto& s : students) {
        if (!s) continue;
        int row = table->rowCount();
        table->insertRow(row);

        QString type = QString::fromStdString(s->getType());
        QColor background(255, 255, 255);
        QColor foreground(17, 24, 39);

        if (type == "Licence") {
            background = QColor(219, 234, 254);
            foreground = QColor(30, 64, 175);
        } else if (type == "Master") {
            background = QColor(220, 252, 231);
            foreground = QColor(22, 101, 52);
        } else if (type == "Doctorat") {
            background = QColor(255, 237, 213);
            foreground = QColor(154, 52, 18);
        }

        QList<QTableWidgetItem*> items;
        items << new QTableWidgetItem(QString::number(s->getId()))
              << new QTableWidgetItem(QString::fromStdString(s->getName()))
              << new QTableWidgetItem(type)
              << new QTableWidgetItem(QString::number(s->getGpa(), 'f', 2))
              << new QTableWidgetItem(QString::number(s->computeScholarship(), 'f', 0) + " EUR");

        for (int col = 0; col < items.size(); ++col) {
            items[col]->setBackground(background);
            items[col]->setForeground(foreground);
            table->setItem(row, col, items[col]);
        }
    }
}

// ═══════════════════════════════════════════════════════════════
// Mise à jour complète
// ═══════════════════════════════════════════════════════════════
void MainWindow::updateTable() {
    // Reset filtres
    searchBox->blockSignals(true);
    filterType->blockSignals(true);
    filterGpaMin->blockSignals(true);
    filterGpaMax->blockSignals(true);

    searchBox->clear();
    filterType->setCurrentIndex(0);
    filterGpaMin->setValue(0.0);
    filterGpaMax->setValue(4.0);

    searchBox->blockSignals(false);
    filterType->blockSignals(false);
    filterGpaMin->blockSignals(false);
    filterGpaMax->blockSignals(false);

    auto students = manager->sortByGpa();
    populateTable(students);
    updateStatusBar();
}

void MainWindow::updateStatusBar() {
    statusLabel->setText(
        QString("Etudiants : %1  |  Moyenne GPA : %2")
            .arg(manager->count())
            .arg(manager->averageGpa(), 0, 'f', 2));
}

int MainWindow::getSelectedStudentId() const {
    int row = table->currentRow();
    if (row < 0 || row >= table->rowCount()) return -1;
    QTableWidgetItem* item = table->item(row, 0);
    if (!item) return -1;
    bool ok = false;
    int id = item->text().toInt(&ok);
    return ok ? id : -1;
}

// ═══════════════════════════════════════════════════════════════
// CRUD Slots
// ═══════════════════════════════════════════════════════════════
void MainWindow::onAddClicked() {
    StudentDialog dialog(this);
    dialog.setWindowTitle("Ajouter un etudiant");
    if (dialog.exec() == QDialog::Accepted) {
        try {
            auto student = dialog.getStudent();
            if (!student) {
                QMessageBox::critical(this, "Erreur",
                    "Impossible de creer l'etudiant.");
                return;
            }
            manager->add(student);
            updateTable();
            saveData();
            statusBar()->showMessage("Etudiant ajoute avec succes.", 3000);
        } catch (const DuplicateIDException& e) {
            QMessageBox::critical(this, "Erreur - ID duplique",
                QString::fromLocal8Bit(e.what()));
        } catch (const InvalidGradeException& e) {
            QMessageBox::warning(this, "Erreur - GPA invalide",
                QString::fromLocal8Bit(e.what()));
        } catch (const InvalidIDException& e) {
            QMessageBox::warning(this, "Erreur - ID invalide",
                QString::fromLocal8Bit(e.what()));
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Erreur", QString::fromLocal8Bit(e.what()));
        } catch (...) {
            QMessageBox::critical(this, "Erreur critique",
                "Une erreur inconnue est survenue.");
        }
    }
}

void MainWindow::onRemoveClicked() {
    int id = getSelectedStudentId();
    if (id < 0) {
        QMessageBox::information(this, "Selection",
            "Veuillez selectionner un etudiant a supprimer.");
        return;
    }

    try {
        auto student = manager->findById(id);
        QString name = QString::fromStdString(student->getName());

        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Confirmation",
            QString("Supprimer %1 (ID: %2) ?").arg(name).arg(id),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (reply != QMessageBox::Yes) return;

        manager->remove(id);
        updateTable();
        saveData();
        statusBar()->showMessage("Etudiant supprime.", 3000);
    } catch (const StudentNotFoundException& e) {
        QMessageBox::warning(this, "Erreur", QString::fromLocal8Bit(e.what()));
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Erreur", QString::fromLocal8Bit(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "Erreur critique",
            "Une erreur inconnue est survenue.");
    }
}

void MainWindow::onUpdateClicked() {
    int id = getSelectedStudentId();
    if (id < 0) {
        QMessageBox::information(this, "Selection",
            "Veuillez selectionner un etudiant a modifier.");
        return;
    }
    try {
        auto student = manager->findById(id);
        if (!student) {
            QMessageBox::critical(this, "Erreur", "Etudiant introuvable.");
            return;
        }
        StudentDialog dialog(this);
        dialog.setWindowTitle("Modifier l'etudiant");
        dialog.setStudent(student);
        if (dialog.exec() == QDialog::Accepted) {
            auto updated = dialog.getStudent();
            if (!updated) {
                QMessageBox::critical(this, "Erreur",
                    "Impossible de creer l'etudiant modifie.");
                return;
            }
            manager->update(id, updated);
            updateTable();
            saveData();
            statusBar()->showMessage("Etudiant modifie.", 3000);
        }
    } catch (const DuplicateIDException& e) {
        QMessageBox::critical(this, "Erreur - ID duplique",
            QString::fromLocal8Bit(e.what()));
    } catch (const InvalidGradeException& e) {
        QMessageBox::warning(this, "Erreur - GPA invalide",
            QString::fromLocal8Bit(e.what()));
    } catch (const StudentNotFoundException& e) {
        QMessageBox::warning(this, "Erreur", QString::fromLocal8Bit(e.what()));
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Erreur", QString::fromLocal8Bit(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "Erreur critique",
            "Une erreur inconnue est survenue.");
    }
}

// ═══════════════════════════════════════════════════════════════
// BONUS : Export CSV
// ═══════════════════════════════════════════════════════════════
void MainWindow::onExportCsv() {
    QString path = QFileDialog::getSaveFileName(
        this, "Exporter en CSV", "etudiants.csv",
        "Fichiers CSV (*.csv);;Tous les fichiers (*)");

    if (path.isEmpty()) return;

    bool ok = CsvExporter::exportToFile(*manager, path.toStdString());
    if (ok) {
        QMessageBox::information(this, "Export CSV",
            QString("Export reussi : %1 etudiants exportes.")
                .arg(manager->count()));
    } else {
        QMessageBox::critical(this, "Erreur",
            "Impossible d'ecrire le fichier CSV.");
    }
}

// ═══════════════════════════════════════════════════════════════
// BONUS : Statistiques
// ═══════════════════════════════════════════════════════════════
void MainWindow::onShowStats() {
    StatsDialog dialog(manager, this);
    dialog.exec();
}

// ═══════════════════════════════════════════════════════════════
// BONUS : Dark Mode
// ═══════════════════════════════════════════════════════════════
void MainWindow::onToggleDarkMode() {
    darkMode = actionDarkMode->isChecked();
    applyStyleSheet();
}

void MainWindow::applyStyleSheet() {
    qApp->setStyleSheet(darkMode ? STYLE_DARK : STYLE_LIGHT);
}

// ═══════════════════════════════════════════════════════════════
// Persistence
// ═══════════════════════════════════════════════════════════════
void MainWindow::loadData() {
    try {
        PersistenceManager::load(*manager, "data/students.json");
    } catch (...) {
        // Fichier absent au premier lancement
    }
}

void MainWindow::saveData() {
    try {
        PersistenceManager::save(*manager, "data/students.json");
    } catch (...) {
        QMessageBox::warning(this, "Sauvegarde",
            "Impossible de sauvegarder les donnees.");
    }
}

// ═══════════════════════════════════════════════════════════════
// Vue Détails de l'Étudiant
// ═══════════════════════════════════════════════════════════════
void MainWindow::onViewDetails() {
    int id = getSelectedStudentId();
    if (id < 0) {
        QMessageBox::information(this, "Selection",
            "Veuillez selectionner un etudiant.");
        return;
    }

    try {
        auto student = manager->findById(id);
        if (!student) {
            QMessageBox::critical(this, "Erreur", "Etudiant introuvable.");
            return;
        }

        StudentDetailsDialog detailsDialog(student, this);
        detailsDialog.exec();
    } catch (const StudentNotFoundException& e) {
        QMessageBox::warning(this, "Erreur", QString::fromLocal8Bit(e.what()));
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Erreur", QString::fromLocal8Bit(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "Erreur critique",
            "Une erreur inconnue est survenue.");
    }
}

// ═══════════════════════════════════════════════════════════════
// Menu Contextuel du Tableau
// ═══════════════════════════════════════════════════════════════
void MainWindow::onTableContextMenu(const QPoint& pos) {
    int row = table->rowAt(pos.y());
    if (row < 0) return;

    // Selectionner la ligne
    table->selectRow(row);

    // Créer le menu contextuel
    QMenu contextMenu;

    QAction* actionView = contextMenu.addAction("Voir les détails");
    QAction* actionEdit = contextMenu.addAction("Modifier");
    QAction* actionDelete = contextMenu.addAction("Supprimer");

    QAction* selectedAction = contextMenu.exec(table->mapToGlobal(pos));

    if (selectedAction == actionView) {
        onViewDetails();
    } else if (selectedAction == actionEdit) {
        onUpdateClicked();
    } else if (selectedAction == actionDelete) {
        onRemoveClicked();
    }
}
