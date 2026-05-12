// src/gui/StudentDialog.cpp
#include "StudentDialog.h"
#include "../exceptions/StudentExceptions.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QFrame>

// ============================================================
// Constructeur
// ============================================================
StudentDialog::StudentDialog(QWidget *parent)
    : QDialog(parent)
    , comboType(nullptr)
    , editName(nullptr)
    , spinId(nullptr)
    , spinGpa(nullptr)
    , stackedWidget(nullptr)
    , editMajor(nullptr)
    , editResearchTopic(nullptr)
    , editSupervisor(nullptr)
    , spinYear(nullptr)
    , labelDynamic(nullptr)
{
    setupUi();
}

// ============================================================
// Mise en place de l'interface du dialogue
// ============================================================
void StudentDialog::setupUi() {
    setWindowTitle("Details de l'Etudiant");
    setMinimumWidth(400);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QFormLayout* formLayout = new QFormLayout();

    // ── Type (en premier pour que le changement dynamique soit visible) ──
    comboType = new QComboBox(this);
    comboType->addItems({"Licence", "Master", "Doctorat"});
    formLayout->addRow("Type :", comboType);

    // ── Nom ──
    editName = new QLineEdit(this);
    editName->setPlaceholderText("Nom complet de l'etudiant");
    formLayout->addRow("Nom :", editName);

    // ── ID ──
    spinId = new QSpinBox(this);
    spinId->setRange(1000, 999999);
    spinId->setValue(1000);
    formLayout->addRow("ID :", spinId);

    // ── GPA ──
    spinGpa = new QDoubleSpinBox(this);
    spinGpa->setRange(0.0, 4.0);
    spinGpa->setDecimals(2);
    spinGpa->setSingleStep(0.1);
    spinGpa->setValue(0.0);
    formLayout->addRow("GPA :", spinGpa);

    mainLayout->addLayout(formLayout);

    // ── Séparateur visuel ──
    QFrame* line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(line);

    // ── Label dynamique ──
    labelDynamic = new QLabel("Champs specifiques :", this);
    labelDynamic->setStyleSheet("font-weight: bold;");
    mainLayout->addWidget(labelDynamic);

    // ── Champs dynamiques via QStackedWidget ─────────────────
    stackedWidget = new QStackedWidget(this);

    // Page 0 : Licence → Filière
    QWidget* pageLicence = new QWidget(this);
    QFormLayout* layoutLicence = new QFormLayout(pageLicence);
    editMajor = new QLineEdit(this);
    editMajor->setPlaceholderText("Ex: Informatique, Mathematiques...");
    layoutLicence->addRow("Filiere :", editMajor);
    stackedWidget->addWidget(pageLicence);

    // Page 1 : Master → Sujet de recherche
    QWidget* pageMaster = new QWidget(this);
    QFormLayout* layoutMaster = new QFormLayout(pageMaster);
    editResearchTopic = new QLineEdit(this);
    editResearchTopic->setPlaceholderText("Ex: Intelligence Artificielle...");
    layoutMaster->addRow("Sujet :", editResearchTopic);
    stackedWidget->addWidget(pageMaster);

    // Page 2 : Doctorat → Encadrant + Année
    QWidget* pageDoctorat = new QWidget(this);
    QFormLayout* layoutDoctorat = new QFormLayout(pageDoctorat);
    editSupervisor = new QLineEdit(this);
    editSupervisor->setPlaceholderText("Nom du directeur de these");
    spinYear = new QSpinBox(this);
    spinYear->setRange(1, 10);
    spinYear->setValue(1);
    layoutDoctorat->addRow("Encadrant :", editSupervisor);
    layoutDoctorat->addRow("Annee :", spinYear);
    stackedWidget->addWidget(pageDoctorat);

    // Index 0 par défaut (Licence)
    stackedWidget->setCurrentIndex(0);

    mainLayout->addWidget(stackedWidget);

    // ── Boutons Valider / Annuler ────────────────────────────
    mainLayout->addSpacing(10);
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* btnOk     = new QPushButton("Valider", this);
    QPushButton* btnCancel = new QPushButton("Annuler", this);
    btnLayout->addStretch();
    btnLayout->addWidget(btnOk);
    btnLayout->addWidget(btnCancel);
    mainLayout->addLayout(btnLayout);

    // ── Connexions ───────────────────────────────────────────
    connect(comboType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StudentDialog::onTypeChanged);
    connect(btnOk,     &QPushButton::clicked, this, &StudentDialog::validateAndAccept);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

// ============================================================
// SLOT : changement de type → basculer le QStackedWidget
// ============================================================
void StudentDialog::onTypeChanged(int index) {
    if (index < 0 || index >= stackedWidget->count()) return;
    stackedWidget->setCurrentIndex(index);
}

// ============================================================
// Pré-remplissage pour modification
// ============================================================
void StudentDialog::setStudent(std::shared_ptr<Student> student) {
    if (!student) return;

    editName->setText(QString::fromStdString(student->getName()));
    spinId->setValue(student->getId());
    spinId->setEnabled(false); // ID non modifiable en édition
    spinGpa->setValue(static_cast<double>(student->getGpa()));

    QString type = QString::fromStdString(student->getType());

    if (type == "Licence") {
        comboType->setCurrentIndex(0);
        auto u = std::dynamic_pointer_cast<UndergraduateStudent>(student);
        if (u) {
            editMajor->setText(QString::fromStdString(u->getMajor()));
        }
    } else if (type == "Master") {
        comboType->setCurrentIndex(1);
        auto g = std::dynamic_pointer_cast<GraduateStudent>(student);
        if (g) {
            editResearchTopic->setText(QString::fromStdString(g->getResearchTopic()));
        }
    } else if (type == "Doctorat") {
        comboType->setCurrentIndex(2);
        auto p = std::dynamic_pointer_cast<PhDStudent>(student);
        if (p) {
            editSupervisor->setText(QString::fromStdString(p->getSupervisor()));
            spinYear->setValue(p->getYearOfStudy());
        }
    }

    // Bloquer le changement de type en mode édition
    comboType->setEnabled(false);
}

// ============================================================
// Validation stricte avant acceptation
// ============================================================
void StudentDialog::validateAndAccept() {
    // 1. Nom non vide
    QString name = editName->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie",
            "Le nom ne peut pas etre vide.");
        editName->setFocus();
        return;
    }

    // 2. ID >= 1000 (déjà contraint par spinbox, mais double vérif)
    int id = spinId->value();
    if (id < 1000) {
        QMessageBox::warning(this, "Erreur de saisie",
            "L'ID doit etre superieur ou egal a 1000.");
        spinId->setFocus();
        return;
    }

    // 3. GPA entre 0 et 4 (déjà contraint, mais double vérif)
    double gpa = spinGpa->value();
    if (gpa < 0.0 || gpa > 4.0) {
        QMessageBox::warning(this, "Erreur de saisie",
            "Le GPA doit etre entre 0.0 et 4.0.");
        spinGpa->setFocus();
        return;
    }

    // 4. Champs spécifiques selon le type
    int typeIndex = comboType->currentIndex();
    if (typeIndex == 0) { // Licence
        if (editMajor->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Erreur de saisie",
                "La filiere ne peut pas etre vide.");
            editMajor->setFocus();
            return;
        }
    } else if (typeIndex == 1) { // Master
        if (editResearchTopic->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Erreur de saisie",
                "Le sujet de recherche ne peut pas etre vide.");
            editResearchTopic->setFocus();
            return;
        }
    } else if (typeIndex == 2) { // Doctorat
        if (editSupervisor->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Erreur de saisie",
                "Le nom de l'encadrant ne peut pas etre vide.");
            editSupervisor->setFocus();
            return;
        }
    }

    accept(); // Ferme le dialogue avec QDialog::Accepted
}

// ============================================================
// Création de l'objet Student selon le type
// ============================================================
std::shared_ptr<Student> StudentDialog::getStudent() const {
    std::string name = editName->text().trimmed().toStdString();
    int id   = spinId->value();
    float gpa = static_cast<float>(spinGpa->value());
    int typeIndex = comboType->currentIndex();

    try {
        switch (typeIndex) {
        case 0: { // Licence
            std::string major = editMajor->text().trimmed().toStdString();
            return std::make_shared<UndergraduateStudent>(name, id, gpa, major);
        }
        case 1: { // Master
            std::string topic = editResearchTopic->text().trimmed().toStdString();
            return std::make_shared<GraduateStudent>(name, id, gpa, topic);
        }
        case 2: { // Doctorat
            std::string supervisor = editSupervisor->text().trimmed().toStdString();
            int year = spinYear->value();
            return std::make_shared<PhDStudent>(name, id, gpa, supervisor, year);
        }
        default:
            return nullptr;
        }
    } catch (const InvalidIDException&) {
        // Ne devrait jamais arriver vu la validation, mais sécurité
        throw; // relancer pour que MainWindow l'attrape
    } catch (const InvalidGradeException&) {
        throw; // relancer pour que MainWindow l'attrape
    } catch (...) {
        return nullptr;
    }
}
