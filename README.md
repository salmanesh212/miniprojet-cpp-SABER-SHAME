# 🎓 StudentManager — Système de Gestion des Étudiants en C++

> **Mini-Projet C++ · IMS 1ère Année · ESITH · Année Universitaire 2025/2026**

---

## 👥 Équipe

| Membre | Rôle principal |
|---|---|
| **SABER Salma**  |
| **SHAME Mohamed Salmane**  |

---

## 🔗 Liens importants

| Ressource | Lien |
|---|---|
| 📁 Dépôt GitHub | [miniprojet-cpp-SABER-SHAME](https://github.com/salmanesh212/miniprojet-cpp-SABER-SHAME) |
| 🎬 Vidéo de démonstration | *(à compléter)* |
| 📄 Rapport PDF | `docs/rapport.pdf` |

---

## 📌 Description du projet

**StudentManager** est un système complet de gestion des étudiants développé en **C++17 orienté objet**. Il couvre l'ensemble des aspects d'un logiciel professionnel réel :

- Hiérarchie de classes avec **héritage et polymorphisme**
- Gestion mémoire sécurisée via **smart pointers** (`std::shared_ptr` — aucun pointeur brut)
- Stockage et manipulation de données avec les **conteneurs et algorithmes STL**
- **Exceptions personnalisées** pour une robustesse totale
- **Persistance JSON** via Qt (`QJsonDocument`) avec fallback fichier texte
- **Interface graphique Qt 6** complète avec filtrage, statistiques et mode sombre
- Workflow collaboratif avec **Git/GitHub** (branches, commits, pull requests)

---

## 🗂️ Structure du projet

```
miniprojet-cpp-SABER-SHAME/
├── src/
│   ├── models/                         # Hiérarchie de classes étudiants
│   │   ├── Student.h / .cpp            # Classe abstraite de base
│   │   ├── UndergraduateStudent.h/.cpp # Étudiant Licence
│   │   ├── GraduateStudent.h/.cpp      # Étudiant Master
│   │   └── PhDStudent.h/.cpp          # Doctorant
│   ├── services/
│   │   └── StudentManager.h           # CRUD + tri + recherche (STL, header-only)
│   ├── exceptions/
│   │   └── StudentExceptions.hpp      # 4 exceptions personnalisées
│   ├── persistence/
│   │   └── PersistenceManager.h       # Sauvegarde/chargement JSON (header-only)
│   └── gui/
│       ├── MainWindow.h/.cpp          # Fenêtre principale Qt
│       ├── StudentDialog.h/.cpp       # Formulaire ajout/modification
│       ├── StudentDetailsDialog.h/.cpp# Fiche détaillée d'un étudiant
│       ├── StatsDialog.h/.cpp         # Statistiques + histogramme QChart
│       └── CsvExporter.h              # Export CSV (header-only)
├── data/
│   ├── students.json                  # Données persistantes (format principal)
│   └── students.txt                   # Format texte alternatif (séparateur |)
├── tests/
│   └── test_main.cpp                  # Suite de tests complets (assert + rapport)
├── main.cpp                           # Point d'entrée console (tests exceptions)
├── main_gui.cpp                       # Point d'entrée interface graphique
├── StudentManagementApp.pro           # Fichier projet Qt (qmake)
└── README.md
```

---

## ⚙️ Prérequis

| Outil | Version minimale | Notes |
|---|---|---|
| Compilateur C++ | GCC ≥ 11 ou MSVC 2019+ | Support C++17 requis |
| Qt | Qt 6.x | Modules : `Widgets`, `Charts` |
| qmake / Qt Creator | Qt Creator ≥ 9 | Inclus dans Qt SDK |
| Git | ≥ 2.30 | Pour cloner et contribuer |

> **Windows** : utilisez [Qt Online Installer](https://www.qt.io/download-qt-installer).  
> **Linux** : `sudo apt install qt6-base-dev qt6-charts-dev`  
> **macOS** : `brew install qt6`

---

## 🚀 Compilation et exécution

### Option 1 — Qt Creator (recommandé)

```bash
# 1. Cloner le dépôt
git clone https://github.com/salmanesh212/miniprojet-cpp-SABER-SHAME.git
cd miniprojet-cpp-SABER-SHAME

# 2. Ouvrir StudentManagementApp.pro dans Qt Creator
#    File → Open File or Project → StudentManagementApp.pro

# 3. Cliquer sur le marteau (Build) puis la flèche verte (Run)
```

### Option 2 — Ligne de commande (qmake)

```bash
git clone https://github.com/salmanesh212/miniprojet-cpp-SABER-SHAME.git
cd miniprojet-cpp-SABER-SHAME

mkdir build && cd build
qmake ../StudentManagementApp.pro
make -j$(nproc)          # Linux/macOS
# ou : mingw32-make      # Windows avec MinGW

./StudentManagementApp   # Lancer la GUI
```

### Compilation des tests (mode console)

```bash
# Depuis la racine du projet
g++ -std=c++17 -o tests_runner tests/test_main.cpp -I.
./tests_runner
```

> Le binaire `tests/test.exe` précompilé (Windows x64) est également disponible pour une exécution rapide sans recompilation.

---

## 🧩 Architecture OOP — Hiérarchie de classes

```
Student  (classe abstraite — src/models/Student.h)
│   Attributs protégés : name, id (≥1000), gpa ([0.0–4.0])
│   Méthodes pures :
│       ├── virtual void display() const = 0
│       ├── virtual std::string getType() const = 0
│       └── virtual double computeScholarship() const = 0
│   Mutateurs avec validation :
│       ├── setName()  — lève std::invalid_argument si vide
│       └── setGpa()   — lève InvalidGradeException si hors [0,4]
│
├── UndergraduateStudent  [Licence]
│       Attribut : std::string major
│       computeScholarship() → 500 € si GPA ≥ 3.5, sinon 0 €
│       getType() → "Licence"
│
├── GraduateStudent  [Master]
│       Attribut : std::string researchTopic
│       computeScholarship() → GPA × 300 €
│       getType() → "Master"
│
└── PhDStudent  [Doctorat]
        Attributs : std::string supervisor, int yearOfStudy
        computeScholarship() → 1 200 € (fixe/mois)
        getType() → "Doctorat"
```

### Exemple d'affichage `display()`

```
[Licence]  ID: 1001 | Nom: Benali Amira  | GPA: 3.8 | Filière: Informatique           | Bourse: 500 EUR
[Master]   ID: 1002 | Nom: Cherif Omar   | GPA: 3.5 | Sujet: Intelligence Artificielle | Bourse: 1050 EUR
[Doctorat] ID: 1003 | Nom: Meziane Riad  | GPA: 3.9 | Directeur: Dr. Hadj | Année: 2  | Bourse: 1200 EUR
```

---

## 📦 Fonctionnalités implémentées

### ✅ Partie 1 — Modélisation OOP

- Classe abstraite `Student` avec 3 méthodes virtuelles pures
- 3 classes concrètes avec leurs attributs et règles de bourse spécifiques
- Constructeurs validant `id` (≥ 1000) et `gpa` ([0.0–4.0]) dès la création
- Polymorphisme complet : `display()` et `computeScholarship()` sur `shared_ptr<Student>`

### ✅ Partie 2 — STL & Gestion des données

| Fonctionnalité | Implémentation STL |
|---|---|
| Stockage des étudiants | `std::map<int, shared_ptr<Student>>` |
| Tri décroissant par GPA | `std::sort` + lambda comparateur |
| Tri alphabétique (insensible casse) | `std::sort` + `std::transform` + `tolower` |
| Recherche partielle par nom | `std::find_if` en boucle |
| Moyenne GPA | boucle range-based sur la map |
| Comptage par type | `std::count_if` |

### ✅ Partie 3 — Exceptions personnalisées

| Exception | Condition de déclenchement | Lieu |
|---|---|---|
| `InvalidIDException` | `id < 1000` | Constructeur `Student` |
| `InvalidGradeException` | `gpa < 0.0` ou `gpa > 4.0` | `Student::setGpa()` et constructeur |
| `DuplicateIDException` | ID déjà présent dans la map | `StudentManager::add()` |
| `StudentNotFoundException` | ID absent de la map | `StudentManager::findById()`, `remove()` |

Toutes héritent de `std::exception` et exposent un message clair via `what()`.

### ✅ Partie 4 — Persistance des données

- **Format principal** : `data/students.json` (via `QJsonDocument` / `QJsonArray`)
- **Format alternatif** : `data/students.txt` avec séparateur `|`
- Chargement automatique au démarrage si le fichier existe
- Sauvegarde immédiate après chaque opération CRUD
- Les lignes/entrées corrompues sont ignorées silencieusement (`try/catch` par entrée)

#### Structure JSON

```json
{
  "students": [
    { "id": 1001, "name": "Benali Amira", "gpa": 3.8, "type": "Licence", "major": "Informatique" },
    { "id": 1002, "name": "Cherif Omar",  "gpa": 3.5, "type": "Master",  "researchTopic": "Intelligence Artificielle" },
    { "id": 1003, "name": "Meziane Riad", "gpa": 3.9, "type": "Doctorat","supervisor": "Dr. Hadj", "yearOfStudy": 2 }
  ]
}
```

#### Format texte alternatif

```
Licence|1001|Benali Amira|3.8|Informatique
Master|1002|Cherif Omar|3.5|Intelligence Artificielle
Doctorat|1003|Meziane Riad|3.9|Dr. Hadj|2
```

### ✅ Partie 5 — Interface graphique Qt 6

#### `MainWindow`
- `QTableWidget` avec colonnes : ID · Nom · Type · GPA · Bourse
- Boutons : **+ Ajouter**, **✎ Modifier**, **🗑 Supprimer**, **🔍 Détails**, **↻ Rafraîchir**
- Champ de recherche `QLineEdit` avec filtrage **en temps réel** (`textChanged`)
- `QComboBox` filtre par type (Tous / Licence / Master / Doctorat)
- `QDoubleSpinBox` filtre par plage de GPA (min/max)
- Barre de statut : nombre d'étudiants + moyenne GPA
- Menu contextuel (clic droit) sur les lignes du tableau
- Menu **Fichier** : Export CSV, Statistiques
- Menu **Affichage** : basculer mode sombre / mode clair

#### `StudentDialog`
- `QComboBox` pour le type → champs supplémentaires affichés dynamiquement
- `QLineEdit` pour Nom et ID, `QDoubleSpinBox` pour GPA (pas 0.1)
- Validation des entrées avec `QMessageBox` (erreur ID, GPA, doublon)
- Boutons **Valider** et **Annuler**

#### `StudentDetailsDialog`
- Fiche complète d'un étudiant avec tous ses attributs et sa bourse calculée

#### `StatsDialog` *(Bonus)*
- Statistiques globales : total, moyenne GPA, répartition par type
- **Histogramme interactif** des GPA via `QtCharts::QBarSeries`

### ✅ Partie 6 — Tests et validation

La suite de tests dans `tests/test_main.cpp` couvre 5 catégories :

| Groupe | Cas testés |
|---|---|
| Types étudiants | `getType()`, `computeScholarship()`, accesseurs, limites GPA 0.0 et 4.0 |
| CRUD | add, remove, update, findById, findByName (partiel) |
| Tri | GPA décroissant, alphabétique insensible à la casse |
| Exceptions | Déclenchement et capture des 4 exceptions + `setGpa()` invalide |
| Persistance | Sauvegarde → vidage → rechargement → comparaison |

#### Tableau de résultats des tests

| Cas de test | Entrée | Résultat attendu | Statut |
|---|---|---|---|
| Ajout étudiant valide | ID=2001, GPA=3.8 | Succès | ✅ OK |
| GPA invalide (trop élevé) | GPA=5.5 | `InvalidGradeException` | ✅ OK |
| GPA invalide (négatif) | GPA=-1.0 | `InvalidGradeException` | ✅ OK |
| ID invalide | ID=42 | `InvalidIDException` | ✅ OK |
| ID dupliqué | ID=2001 (×2) | `DuplicateIDException` | ✅ OK |
| Étudiant introuvable (findById) | ID=9999 | `StudentNotFoundException` | ✅ OK |
| Étudiant introuvable (remove) | ID=5555 | `StudentNotFoundException` | ✅ OK |
| setGpa() valeur invalide | gpa=6.0 | `InvalidGradeException` | ✅ OK |
| setGpa() valeur valide | gpa=3.9 | GPA mis à jour | ✅ OK |
| Tri GPA décroissant | 5 étudiants | Liste triée | ✅ OK |
| Tri alphabétique | 5 étudiants | Ordre insensible à la casse | ✅ OK |
| Persistance | Save + reload | Données identiques | ✅ OK |
| Fichier absent | Chemin inexistant | Démarrage liste vide, pas de crash | ✅ OK |

---

## 🎁 Fonctionnalités bonus implémentées

| Bonus | Description | Implémentation |
|---|---|---|
| ✅ **Export CSV** | Export de la liste en `.csv` compatible Excel (UTF-8 BOM, séparateur `;`) | `src/gui/CsvExporter.h` |
| ✅ **Filtrage dynamique** | Filtre simultané par texte, type et plage de GPA | `MainWindow::getFilteredStudents()` |
| ✅ **Smart pointers exclusifs** | Aucun pointeur brut dans tout le projet | `shared_ptr<Student>` partout |
| ✅ **Statistiques + histogramme** | `StatsDialog` avec `QChart` / `QBarSeries` | `src/gui/StatsDialog.h/.cpp` |
| ✅ **Mode sombre** | Basculement clair ↔ sombre via `QAction` dans le menu | `MainWindow::onToggleDarkMode()` |

---

## 🌿 Organisation Git

| Branche | Description |
|---|---|
| `main` | Version stable et fonctionnelle |
| `feature/oop` | Hiérarchie de classes Student |
| `feature/stl` | Service `StudentManager` (conteneurs & algorithmes) |
| `feature/exceptions` | Exceptions personnalisées |
| `feature/persistence` | `PersistenceManager` (JSON + texte) |
| `feature/gui` | Interface graphique Qt 6 |
| `feature/tests` | Suite de tests `test_main.cpp` |

> Chaque fonctionnalité a été développée sur sa propre branche puis intégrée via **pull request** dans `main`. L'historique Git comporte un minimum de 10 commits au nom des deux membres.

---

## 🔑 Points techniques clés

### Polymorphisme en action

```cpp
// Stockage hétérogène — tous types d'étudiants dans la même map
std::map<int, std::shared_ptr<Student>> students;

// Appel polymorphique — dispatch dynamique automatique
for (const auto& [id, student] : students) {
    student->display();                        // virtual dispatch
    double bourse = student->computeScholarship();  // virtual dispatch
}
```

### Gestion des exceptions (pattern recommandé)

```cpp
try {
    manager->add(make_shared<UndergraduateStudent>("Test", 42, 5.5, "Info"));
} catch (const InvalidIDException& e) {
    QMessageBox::critical(this, "Erreur ID", e.what());
} catch (const InvalidGradeException& e) {
    QMessageBox::warning(this, "Erreur GPA", e.what());
} catch (const DuplicateIDException& e) {
    QMessageBox::warning(this, "ID dupliqué", e.what());
}
```

### Tri STL avec lambda

```cpp
// Tri décroissant par GPA
std::sort(result.begin(), result.end(),
    [](const shared_ptr<Student>& a, const shared_ptr<Student>& b) {
        return a->getGpa() > b->getGpa();
    });

// Tri alphabétique insensible à la casse
std::sort(result.begin(), result.end(),
    [](const shared_ptr<Student>& a, const shared_ptr<Student>& b) {
        std::string na = a->getName(), nb = b->getName();
        std::transform(na.begin(), na.end(), na.begin(), ::tolower);
        std::transform(nb.begin(), nb.end(), nb.begin(), ::tolower);
        return na < nb;
    });
```

### Persistance JSON

```cpp
// Sauvegarde
PersistenceManager::save(manager, "data/students.json");

// Chargement (silencieux si fichier absent ou corrompu)
PersistenceManager::load(manager, "data/students.json");
```

---

## 📊 Grille d'évaluation (rappel)

| Critère | Points | État |
|---|---|---|
| OOP et Polymorphisme | 20 | ✅ Implémenté |
| STL (conteneurs & algorithmes) | 15 | ✅ Implémenté |
| Exceptions personnalisées | 15 | ✅ Implémenté |
| Persistance fichiers | 10 | ✅ Implémenté (JSON + TXT) |
| Interface graphique | 20 | ✅ Implémenté (Qt 6) |
| Tests et validation | 10 | ✅ Suite complète |
| Qualité du code | 5 | ✅ Commenté, structuré |
| Dépôt GitHub | 5 | ✅ Branches + README |
| **Bonus** | **+10** | ✅ CSV, Filtres, Charts, Dark mode |
| **Total** | **100 + 10** | |

---

## 📚 Références

- [Documentation Qt 6](https://doc.qt.io/qt-6/)
- [cppreference — STL Algorithms](https://en.cppreference.com/w/cpp/algorithm)
- [cppreference — shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr)
- [QtCharts — QBarSeries](https://doc.qt.io/qt-6/qbarseries.html)
- [QJsonDocument](https://doc.qt.io/qt-6/qjsondocument.html)

---

> 📅 Date de rendu : **15 mai 2026** · ESITH — École Supérieure des Industries du Textile et de l'Habillement
