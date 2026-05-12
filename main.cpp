#include <iostream>
#include <memory>
#include "src/models/Student.h"
#include "src/models/UndergraduateStudent.h"
#include "src/models/GraduateStudent.h"
#include "src/models/PhDStudent.h"
#include "src/services/StudentManager.h"
#include "src/persistence/PersistenceManager.h"
#include "src/exceptions/StudentExceptions.hpp"

using namespace std;

int main() {
    StudentManager mgr;
    string dataFile = "data/students.json";
    
    cout << "=====================================" << endl;
    cout << " GESTION DES ÉTUDIANTS - PARTIE 3 & 4" << endl;
    cout << "=====================================" << endl << endl;

    // ============================================
    // PART 4: Charger les données au démarrage
    // ============================================
    cout << "[PARTIE 4] Chargement des données..." << endl;
    PersistenceManager::load(mgr, dataFile);
    cout << "✓ Données chargées (" << mgr.count() << " étudiants)" << endl << endl;

    // ============================================
    // TEST 1 : InvalidIDException - ID < 1000
    // ============================================
    cout << "[TEST 1] InvalidIDException - ID invalide (< 1000)" << endl;
    try {
        auto student = make_shared<UndergraduateStudent>("Test Invalid ID", 42, 3.5, "Informatique");
        mgr.add(student);
    } catch (const InvalidIDException& e) {
        cerr << "[ERREUR ID] " << e.what() << endl;
    } catch (const exception& e) {
        cerr << "[ERREUR] " << e.what() << endl;
    }
    cout << endl;

    // ============================================
    // TEST 2 : InvalidGradeException - GPA > 4.0
    // ============================================
    cout << "[TEST 2] InvalidGradeException - GPA hors plage (5.5)" << endl;
    try {
        auto student = make_shared<UndergraduateStudent>("Test Invalid GPA", 1001, 5.5, "Informatique");
        mgr.add(student);
    } catch (const InvalidGradeException& e) {
        cerr << "[ERREUR GPA] " << e.what() << endl;
    } catch (const InvalidIDException& e) {
        cerr << "[ERREUR ID] " << e.what() << endl;
    } catch (const exception& e) {
        cerr << "[ERREUR] " << e.what() << endl;
    }
    cout << endl;

    // ============================================
    // TEST 3 : InvalidGradeException - GPA < 0.0
    // ============================================
    cout << "[TEST 3] InvalidGradeException - GPA négatif (-1.0)" << endl;
    try {
        auto student = make_shared<UndergraduateStudent>("Test Negative GPA", 1002, -1.0, "Informatique");
        mgr.add(student);
    } catch (const InvalidGradeException& e) {
        cerr << "[ERREUR GPA] " << e.what() << endl;
    } catch (const exception& e) {
        cerr << "[ERREUR] " << e.what() << endl;
    }
    cout << endl;

    // ============================================
    // TEST 4 : Création d'étudiants valides
    // ============================================
    cout << "[TEST 4] Création d'étudiants VALIDES" << endl;
    try {
        auto ug = make_shared<UndergraduateStudent>("Benali Amira", 2001, 3.8, "Informatique");
        mgr.add(ug);
        cout << "✓ UndergraduateStudent ajouté avec succès (ID: 2001)" << endl;
        
        auto grad = make_shared<GraduateStudent>("Cherif Omar", 2002, 3.5, "Intelligence Artificielle");
        mgr.add(grad);
        cout << "✓ GraduateStudent ajouté avec succès (ID: 2002)" << endl;
        
        auto phd = make_shared<PhDStudent>("Meziane Riad", 2003, 3.9, "Dr. Hadj", 2);
        mgr.add(phd);
        cout << "✓ PhDStudent ajouté avec succès (ID: 2003)" << endl;
    } catch (const exception& e) {
        cerr << "[ERREUR] " << e.what() << endl;
    }
    cout << endl;

    // ============================================
    // TEST 5 : DuplicateIDException - ID déjà utilisé
    // ============================================
    cout << "[TEST 5] DuplicateIDException - ID déjà utilisé (2001)" << endl;
    try {
        auto duplicate = make_shared<UndergraduateStudent>("Duplicate Student", 2001, 3.0, "Mathematiques");
        mgr.add(duplicate);
    } catch (const DuplicateIDException& e) {
        cerr << "[DOUBLON ID] " << e.what() << endl;
    } catch (const exception& e) {
        cerr << "[ERREUR] " << e.what() << endl;
    }
    cout << endl;

    // ============================================
    // TEST 6 : StudentNotFoundException - findById
    // ============================================
    cout << "[TEST 6] StudentNotFoundException - Étudiant inexistant (ID: 9999)" << endl;
    try {
        auto student = mgr.findById(9999);
    } catch (const StudentNotFoundException& e) {
        cerr << "[ETUDIANT INTROUVABLE] " << e.what() << endl;
    } catch (const exception& e) {
        cerr << "[ERREUR] " << e.what() << endl;
    }
    cout << endl;

    // ============================================
    // TEST 7 : StudentNotFoundException - remove
    // ============================================
    cout << "[TEST 7] StudentNotFoundException - Suppression étudiant inexistant (ID: 5555)" << endl;
    try {
        mgr.remove(5555);
    } catch (const StudentNotFoundException& e) {
        cerr << "[ETUDIANT INTROUVABLE] " << e.what() << endl;
    } catch (const exception& e) {
        cerr << "[ERREUR] " << e.what() << endl;
    }
    cout << endl;

    // ============================================
    // TEST 8 : findById avec ID valide
    // ============================================
    cout << "[TEST 8] findById - Recherche d'un étudiant valide (ID: 2001)" << endl;
    try {
        auto student = mgr.findById(2001);
        cout << "✓ Étudiant trouvé: " << student->getName() << " (" << student->getType() << ")" << endl;
    } catch (const exception& e) {
        cerr << "[ERREUR] " << e.what() << endl;
    }
    cout << endl;

    // ============================================
    // TEST 9 : setGpa() avec valeur invalide
    // ============================================
    cout << "[TEST 9] InvalidGradeException - setGpa() avec valeur invalide (6.0)" << endl;
    try {
        auto student = mgr.findById(2001);
        student->setGpa(6.0);
    } catch (const InvalidGradeException& e) {
        cerr << "[ERREUR GPA] " << e.what() << endl;
    } catch (const exception& e) {
        cerr << "[ERREUR] " << e.what() << endl;
    }
    cout << endl;

    // ============================================
    // TEST 10 : setGpa() avec valeur valide
    // ============================================
    cout << "[TEST 10] setGpa() - Modification du GPA avec valeur valide (3.9)" << endl;
    try {
        auto student = mgr.findById(2001);
        student->setGpa(3.9);
        cout << "✓ GPA modifié avec succès: " << student->getGpa() << endl;
    } catch (const exception& e) {
        cerr << "[ERREUR] " << e.what() << endl;
    }
    cout << endl;

    // ============================================
    // SAUVEGARDER LES DONNÉES
    // ============================================
    cout << "=====================================" << endl;
    cout << " Sauvegarde des données..." << endl;
    PersistenceManager::save(mgr, dataFile);
    cout << "✓ Données sauvegardées" << endl;
    cout << "Nombre d'étudiants en base: " << mgr.count() << endl;
    cout << "Moyenne GPA: " << mgr.averageGpa() << endl;
    cout << "=====================================" << endl;

    return 0;
}
