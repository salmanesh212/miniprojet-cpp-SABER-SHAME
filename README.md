# 🎓 StudentManager — Système de Gestion des Étudiants en C++

> Mini-Projet C++ · IMS 1ère Année · ESITH · Année Universitaire 2025/2026

---

## 👥 Équipe

|           Membre                    |
|-------------------------------------|
|           **SABER Salma**           |
|           **SHAME Mohamed Salmane** |

---

## 🔗 Liens importants

- 📁 **Dépôt GitHub** : [https://github.com/salmanesh212/miniprojet-cpp-SABER-SHAME](https://github.com/salmanesh212/miniprojet-cpp-SABER-SHAME)
- 🎬 **Vidéo de démonstration** : [https://youtube.com/...](https://youtube.com/...)
- 📄 **Rapport PDF** : disponible dans `/docs/rapport.pdf`

---

## 📌 Description du projet

**StudentManager** est un système complet de gestion des étudiants développé en **C++ orienté objet**. Il couvre l'ensemble du cycle de vie d'un logiciel professionnel :

- Modélisation objet avancée avec **héritage et polymorphisme**
- Gestion mémoire avec **smart pointers** (`shared_ptr`)
- Manipulation de données via les **conteneurs et algorithmes STL**
- **Exceptions personnalisées** pour une robustesse maximale
- **Persistance des données** via fichiers texte
- **Interface graphique interactive** développée avec **Qt 6**
- Travail collaboratif avec **Git/GitHub** (branches, commits, pull requests)

---

## 🗂️ Structure du projet

```
StudentManager/
├── src/
│   ├── models/                  # Hiérarchie de classes étudiants
│   │   ├── Student.h / .cpp         # Classe abstraite de base
│   │   ├── UndergraduateStudent.h / .cpp   # Étudiant Licence
│   │   ├── GraduateStudent.h / .cpp        # Étudiant Master
│   │   └── PhDStudent.h / .cpp             # Doctorant
│   ├── services/                # Logique métier
│   │   └── StudentManager.h / .cpp  # CRUD + tri + recherche (STL)
│   ├── exceptions/              # Exceptions personnalisées
│   │   └── StudentExceptions.h
│   ├── persistence/             # Lecture / écriture fichiers
│   │   └── PersistenceManager.h / .cpp
│   └── gui/                     # Interface graphique Qt
│       ├── MainWindow.h / .cpp
│       ├── MainWindow.ui
│       ├── StudentDialog.h / .cpp
│       └── StudentDialog.ui
├── data/
│   └── students.txt             # Fichier de données persistantes
├── tests/
│   └── test_main.cpp            # Tests unitaires complets
├── docs/
│   └── rapport.pdf              # Rapport de projet
├── StudentManager.pro           # Fichier de projet Qt
├── CMakeLists.txt               # Système de build CMake
└── README.md
```

---

## ⚙️ Prérequis

| Outil | Version recommandée |
|-------|-------------------|
| Compilateur C++ | GCC ≥ 11 ou MSVC ≥ 2019 |
| Standard C++ | C++17 |
| Qt | Qt 6.x (Widgets + Charts) |
| CMake | ≥ 3.16 |
| Git | ≥ 2.30 |

---

## 🚀 Compilation et exécution

### Option 1 — Via Qt Creator (recommandé)

```bash
# 1. Cloner le dépôt
git clone https://github.com/salmanesh212/miniprojet-cpp-SABER-SHAME.git
cd miniprojet-cpp

# 2. Ouvrir StudentManager.pro dans Qt Creator
# 3. Cliquer sur "Build" puis "Run"
```

### Option 2 — Via CMake (ligne de commande)

```bash
# 1. Cloner le dépôt
git clone https://github.com/salmanesh212/miniprojet-cpp-SABER-SHAME.git
cd miniprojet-cpp

# 2. Configurer et compiler
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt6
make -j4

# 3. Lancer l'application
./StudentManager
```

### Compilation des tests

```bash
# Depuis le dossier build/
g++ -std=c++17 -o tests ../tests/test_main.cpp -I../src
./tests
```

---

## 🧩 Architecture OOP — Hiérarchie de classes

```
Student  (classe abstraite)
│   ├── display() = 0
│   ├── getType() = 0
│   └── computeScholarship() = 0
│
├── UndergraduateStudent  [Licence]
│       └── major : string
│       └── Bourse : 500€ si GPA ≥ 3.5, sinon 0€
│
├── GraduateStudent  [Master]
│       └── researchTopic : string
│       └── Bourse : GPA × 300€
│
└── PhDStudent  [Doctorat]
        ├── supervisor : string
        ├── yearOfStudy : int
        └── Bourse fixe : 1 200€/mois
```

---

## 📦 Fonctionnalités

### ✅ Partie 1 — Modélisation OOP
- Classe abstraite `Student` avec méthodes virtuelles pures
- 3 classes dérivées : `UndergraduateStudent`, `GraduateStudent`, `PhDStudent`
- Calcul de bourse polymorphique via `computeScholarship()`
- Affichage formaté via `display()`

### ✅ Partie 2 — STL & Gestion des données
- Stockage via `std::map<int, shared_ptr<Student>>`
- Tri décroissant par GPA (`std::sort` + lambda)
- Tri alphabétique par nom (insensible à la casse)
- Recherche partielle par nom (`std::find_if`)
- Statistiques : moyenne GPA, comptage par type (`std::count_if`)

### ✅ Partie 3 — Exceptions personnalisées

| Exception | Déclenchée quand |
|-----------|-----------------|
| `InvalidIDException` | ID < 1000 |
| `InvalidGradeException` | GPA hors [0.0, 4.0] |
| `DuplicateIDException` | ID déjà utilisé |
| `StudentNotFoundException` | ID introuvable |

### ✅ Partie 4 — Persistance des données
- Format fichier `data/students.txt` avec séparateur `|`
- Chargement automatique au démarrage
- Sauvegarde immédiate après chaque modification
- Gestion silencieuse des lignes corrompues

### ✅ Partie 5 — Interface graphique Qt 6
- **MainWindow** : tableau `QTableWidget`, recherche en temps réel, barre de statut
- **StudentDialog** : formulaire dynamique selon le type, validation avec `QMessageBox`
- Connexion GUI ↔ Exceptions : chaque erreur affichée en boîte de dialogue

### ✅ Partie 6 — Tests et validation

| Cas de test | Entrée | Résultat attendu | Statut |
|-------------|--------|-----------------|--------|
| Ajout valide | ID=1001, GPA=3.8 | Succès | ✅ OK |
| GPA invalide | GPA=5.5 | `InvalidGradeException` | ✅ OK |
| ID invalide | ID=42 | `InvalidIDException` | ✅ OK |
| ID dupliqué | ID=1001 (×2) | `DuplicateIDException` | ✅ OK |
| Étudiant introuvable | `remove(9999)` | `StudentNotFoundException` | ✅ OK |
| Tri GPA décroissant | 5 étudiants | Liste triée | ✅ OK |
| Persistance | Save + reload | Données identiques | ✅ OK |

---

## 🎁 Fonctionnalités bonus implémentées

- [ ] Export CSV de la liste des étudiants
- [ ] Filtrage dynamique (type, plage GPA, filière)
- [ ] Smart pointers exclusifs (aucun pointeur brut)
- [ ] Histogramme GPA via `QChart`
- [ ] Mode sombre / mode clair

---

## 📄 Format du fichier de données

```
Licence|1001|Benali Amira|3.8|Informatique
Master|1002|Cherif Omar|3.5|Intelligence Artificielle
Doctorat|1003|Meziane Riad|3.9|Dr. Hadj|2
```

---

## 🌿 Organisation Git

| Branche | Description |
|---------|-------------|
| `main` | Version stable et fonctionnelle |
| `feature/oop` | Modélisation des classes Student |
| `feature/stl` | Service StudentManager (STL) |
| `feature/exceptions` | Exceptions personnalisées |
| `feature/persistence` | PersistenceManager (fichiers) |
| `feature/gui` | Interface graphique Qt |
| `feature/tests` | Tests unitaires |

---



## 📚 Références

- [Documentation Qt 6](https://doc.qt.io/qt-6/)
- [cppreference — STL Algorithms](https://en.cppreference.com/w/cpp/algorithm)
- [C++ Smart Pointers](https://en.cppreference.com/w/cpp/memory/shared_ptr)

---

> 📅 Date de rendu : **15 mai 2026** · ESITH — École Supérieure des Industries du Textile et de l'Habillement
