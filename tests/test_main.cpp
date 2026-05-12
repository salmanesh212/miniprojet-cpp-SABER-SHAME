// tests/test_main.cpp - Suite de tests complète pour le projet IMS
#include <iostream>
#include <memory>
#include <string>
#include <cassert>
#include <cstdio>
#include "../src/models/Student.h"
#include "../src/models/UndergraduateStudent.h"
#include "../src/models/GraduateStudent.h"
#include "../src/models/PhDStudent.h"
#include "../src/services/StudentManager.h"
#include "../src/persistence/PersistenceManager.h"
#include "../src/exceptions/StudentExceptions.hpp"

using namespace std;

static int passed = 0, failed = 0;

void report(const string& name, bool ok) {
    cout << "[" << name << "] -> " << (ok ? "OK" : "FAIL") << endl;
    ok ? ++passed : ++failed;
}

// ============================================================
// 1. Tests des types d'étudiants
// ============================================================
void testStudents() {
    cout << "\n========== TESTS TYPES ETUDIANTS ==========\n";

    // UndergraduateStudent
    try {
        auto ug = make_shared<UndergraduateStudent>("Ali Ben", 1001, 3.8f, "Informatique");
        report("UG getType", ug->getType() == "Licence");
        report("UG scholarship (GPA>=3.5)", ug->computeScholarship() == 500.0);
        report("UG getName", ug->getName() == "Ali Ben");
        report("UG getId", ug->getId() == 1001);
        report("UG getGpa", ug->getGpa() >= 3.79f && ug->getGpa() <= 3.81f);
        report("UG getMajor", ug->getMajor() == "Informatique");
    } catch (...) { report("UG creation", false); }

    // UndergraduateStudent low GPA -> no scholarship
    try {
        auto ug2 = make_shared<UndergraduateStudent>("Sara K", 1002, 2.5f, "Maths");
        report("UG scholarship (GPA<3.5)", ug2->computeScholarship() == 0.0);
    } catch (...) { report("UG low GPA creation", false); }

    // GraduateStudent
    try {
        auto gr = make_shared<GraduateStudent>("Omar C", 1003, 3.5f, "IA");
        report("GRAD getType", gr->getType() == "Master");
        report("GRAD scholarship", gr->computeScholarship() == 3.5 * 300.0);
        report("GRAD getResearchTopic", gr->getResearchTopic() == "IA");
    } catch (...) { report("GRAD creation", false); }

    // PhDStudent
    try {
        auto phd = make_shared<PhDStudent>("Riad M", 1004, 3.9f, "Dr. Hadj", 2);
        report("PHD getType", phd->getType() == "Doctorat");
        report("PHD scholarship", phd->computeScholarship() == 1200.0);
        report("PHD getSupervisor", phd->getSupervisor() == "Dr. Hadj");
        report("PHD getYearOfStudy", phd->getYearOfStudy() == 2);
    } catch (...) { report("PHD creation", false); }

    // Test GPA boundary: GPA = 0.0 (valide)
    try {
        auto ug0 = make_shared<UndergraduateStudent>("Zero GPA", 1005, 0.0f, "Info");
        report("GPA boundary 0.0", ug0->getGpa() >= -0.01f && ug0->getGpa() <= 0.01f);
        report("GPA 0.0 scholarship", ug0->computeScholarship() == 0.0);
    } catch (...) { report("GPA boundary 0.0", false); }

    // Test GPA boundary: GPA = 4.0 (valide)
    try {
        auto ug4 = make_shared<UndergraduateStudent>("Perfect GPA", 1006, 4.0f, "Maths");
        report("GPA boundary 4.0", ug4->getGpa() >= 3.99f && ug4->getGpa() <= 4.01f);
        report("GPA 4.0 scholarship", ug4->computeScholarship() == 500.0);
    } catch (...) { report("GPA boundary 4.0", false); }

    // display() - just verify no crash
    try {
        auto ug = make_shared<UndergraduateStudent>("Test", 1010, 3.0f, "Info");
        auto gr = make_shared<GraduateStudent>("Test2", 1011, 3.0f, "Sujet");
        auto phd = make_shared<PhDStudent>("Test3", 1012, 3.0f, "Dr. X", 1);
        ug->display(); gr->display(); phd->display();
        report("DISPLAY no crash", true);
    } catch (...) { report("DISPLAY no crash", false); }
}

// ============================================================
// 2. Tests CRUD
// ============================================================
void testCRUD() {
    cout << "\n========== TESTS CRUD ==========\n";
    StudentManager mgr;

    // ADD
    try {
        auto s1 = make_shared<UndergraduateStudent>("Alice", 2001, 3.5f, "Info");
        mgr.add(s1);
        report("ADD student", mgr.count() == 1);
    } catch (...) { report("ADD student", false); }

    try {
        auto s2 = make_shared<GraduateStudent>("Bob", 2002, 3.2f, "ML");
        auto s3 = make_shared<PhDStudent>("Charlie", 2003, 3.9f, "Dr. Y", 3);
        mgr.add(s2);
        mgr.add(s3);
        report("ADD multiple", mgr.count() == 3);
    } catch (...) { report("ADD multiple", false); }

    // FIND BY ID
    try {
        auto found = mgr.findById(2001);
        report("FIND BY ID valid", found && found->getName() == "Alice");
    } catch (...) { report("FIND BY ID valid", false); }

    // FIND BY NAME (partial, case-insensitive)
    try {
        auto results = mgr.findByName("ali");
        report("FIND BY NAME partial", results.size() >= 1);
        bool foundAlice = false;
        for (const auto& s : results) {
            if (s->getName() == "Alice") foundAlice = true;
        }
        report("FIND BY NAME correct", foundAlice);
    } catch (...) { report("FIND BY NAME", false); }

    // FIND BY NAME empty result
    try {
        auto results = mgr.findByName("ZZZZZ");
        report("FIND BY NAME no match", results.empty());
    } catch (...) { report("FIND BY NAME no match", false); }

    // UPDATE
    try {
        auto updated = make_shared<UndergraduateStudent>("Alice Updated", 2001, 3.7f, "Maths");
        mgr.update(2001, updated);
        auto check = mgr.findById(2001);
        report("UPDATE student", check->getName() == "Alice Updated");
        report("UPDATE GPA", check->getGpa() >= 3.69f && check->getGpa() <= 3.71f);
    } catch (...) { report("UPDATE student", false); }

    // REMOVE
    try {
        mgr.remove(2003);
        report("REMOVE student", mgr.count() == 2);
    } catch (...) { report("REMOVE student", false); }

    // REMOVE verify gone
    try {
        mgr.findById(2003);
        report("REMOVE verify gone", false);
    } catch (const StudentNotFoundException&) {
        report("REMOVE verify gone", true);
    } catch (...) { report("REMOVE verify gone", false); }

    // COUNT BY TYPE
    try {
        report("COUNT BY TYPE Licence", mgr.countByType("Licence") == 1);
        report("COUNT BY TYPE Master", mgr.countByType("Master") == 1);
        report("COUNT BY TYPE Doctorat", mgr.countByType("Doctorat") == 0);
    } catch (...) { report("COUNT BY TYPE", false); }

    // AVERAGE GPA
    try {
        double avg = mgr.averageGpa();
        report("AVERAGE GPA", avg > 0.0 && avg <= 4.0);
    } catch (...) { report("AVERAGE GPA", false); }

    // EMPTY MANAGER
    try {
        StudentManager emptyMgr;
        report("EMPTY count", emptyMgr.count() == 0);
        report("EMPTY averageGpa", emptyMgr.averageGpa() == 0.0);
        auto res = emptyMgr.findByName("test");
        report("EMPTY findByName", res.empty());
        auto sorted = emptyMgr.sortByGpa();
        report("EMPTY sortByGpa", sorted.empty());
    } catch (...) { report("EMPTY manager", false); }
}

// ============================================================
// 3. Tests Tri
// ============================================================
void testSorting() {
    cout << "\n========== TESTS TRI ==========\n";
    StudentManager mgr;

    try {
        mgr.add(make_shared<UndergraduateStudent>("Charlie", 3001, 2.5f, "Info"));
        mgr.add(make_shared<GraduateStudent>("Alice", 3002, 3.8f, "IA"));
        mgr.add(make_shared<PhDStudent>("bob", 3003, 3.0f, "Dr. Z", 1));
        mgr.add(make_shared<UndergraduateStudent>("Diana", 3004, 3.9f, "Maths"));
    } catch (...) { report("SORT setup", false); return; }

    // SORT BY GPA descending
    try {
        auto byGpa = mgr.sortByGpa();
        report("SORT GPA count", (int)byGpa.size() == 4);
        bool descending = true;
        for (size_t i = 1; i < byGpa.size(); ++i) {
            if (byGpa[i]->getGpa() > byGpa[i-1]->getGpa()) {
                descending = false;
                break;
            }
        }
        report("SORT GPA descending", descending);
        report("SORT GPA first", byGpa[0]->getGpa() >= 3.89f);
        report("SORT GPA last", byGpa[3]->getGpa() <= 2.51f);
    } catch (...) { report("SORT GPA", false); }

    // SORT BY NAME alphabetical case-insensitive
    try {
        auto byName = mgr.sortByName();
        report("SORT NAME count", (int)byName.size() == 4);
        bool alphabetical = true;
        for (size_t i = 1; i < byName.size(); ++i) {
            string a = byName[i-1]->getName();
            string b = byName[i]->getName();
            transform(a.begin(), a.end(), a.begin(), ::tolower);
            transform(b.begin(), b.end(), b.begin(), ::tolower);
            if (a > b) { alphabetical = false; break; }
        }
        report("SORT NAME alphabetical", alphabetical);
        // First should be Alice (or bob), last should be Diana
        string firstName = byName[0]->getName();
        transform(firstName.begin(), firstName.end(), firstName.begin(), ::tolower);
        report("SORT NAME first is 'alice'", firstName == "alice");
    } catch (...) { report("SORT NAME", false); }
}

// ============================================================
// 4. Tests Exceptions
// ============================================================
void testExceptions() {
    cout << "\n========== TESTS EXCEPTIONS ==========\n";

    // InvalidIDException (ID < 1000)
    try {
        auto s = make_shared<UndergraduateStudent>("Test", 42, 3.0f, "Info");
        report("INVALID ID EXCEPTION", false);
    } catch (const InvalidIDException& e) {
        report("INVALID ID EXCEPTION", true);
        cout << "  -> Message: " << e.what() << endl;
    } catch (...) { report("INVALID ID EXCEPTION", false); }

    // InvalidGradeException (GPA > 4.0)
    try {
        auto s = make_shared<UndergraduateStudent>("Test", 4001, 5.5f, "Info");
        report("INVALID GPA >4 EXCEPTION", false);
    } catch (const InvalidGradeException& e) {
        report("INVALID GPA >4 EXCEPTION", true);
        cout << "  -> Message: " << e.what() << endl;
    } catch (...) { report("INVALID GPA >4 EXCEPTION", false); }

    // InvalidGradeException (GPA < 0)
    try {
        auto s = make_shared<UndergraduateStudent>("Test", 4002, -1.0f, "Info");
        report("INVALID GPA <0 EXCEPTION", false);
    } catch (const InvalidGradeException& e) {
        report("INVALID GPA <0 EXCEPTION", true);
        cout << "  -> Message: " << e.what() << endl;
    } catch (...) { report("INVALID GPA <0 EXCEPTION", false); }

    // InvalidGradeException via setGpa()
    try {
        auto s = make_shared<UndergraduateStudent>("Test", 4003, 3.0f, "Info");
        s->setGpa(6.0f);
        report("INVALID GPA setGpa EXCEPTION", false);
    } catch (const InvalidGradeException& e) {
        report("INVALID GPA setGpa EXCEPTION", true);
    } catch (...) { report("INVALID GPA setGpa EXCEPTION", false); }

    // DuplicateIDException
    try {
        StudentManager mgr;
        mgr.add(make_shared<UndergraduateStudent>("A", 5001, 3.0f, "Info"));
        mgr.add(make_shared<UndergraduateStudent>("B", 5001, 3.5f, "Maths"));
        report("DUPLICATE ID EXCEPTION", false);
    } catch (const DuplicateIDException& e) {
        report("DUPLICATE ID EXCEPTION", true);
        cout << "  -> Message: " << e.what() << endl;
    } catch (...) { report("DUPLICATE ID EXCEPTION", false); }

    // StudentNotFoundException - findById
    try {
        StudentManager mgr;
        mgr.findById(9999);
        report("STUDENT NOT FOUND findById", false);
    } catch (const StudentNotFoundException& e) {
        report("STUDENT NOT FOUND findById", true);
        cout << "  -> Message: " << e.what() << endl;
    } catch (...) { report("STUDENT NOT FOUND findById", false); }

    // StudentNotFoundException - remove
    try {
        StudentManager mgr;
        mgr.remove(9999);
        report("STUDENT NOT FOUND remove", false);
    } catch (const StudentNotFoundException& e) {
        report("STUDENT NOT FOUND remove", true);
    } catch (...) { report("STUDENT NOT FOUND remove", false); }

    // StudentNotFoundException - update
    try {
        StudentManager mgr;
        auto s = make_shared<UndergraduateStudent>("X", 9999, 3.0f, "Y");
        mgr.update(9999, s);
        report("STUDENT NOT FOUND update", false);
    } catch (const StudentNotFoundException& e) {
        report("STUDENT NOT FOUND update", true);
    } catch (...) { report("STUDENT NOT FOUND update", false); }
}

// ============================================================
// 5. Tests Persistance
// ============================================================
void testPersistence() {
    cout << "\n========== TESTS PERSISTANCE ==========\n";
    const string testFile = "data/test_students.json";

    // Préparer les données
    StudentManager mgrSave;
    try {
        mgrSave.add(make_shared<UndergraduateStudent>("Amira B", 6001, 3.8f, "Informatique"));
        mgrSave.add(make_shared<GraduateStudent>("Omar C", 6002, 3.5f, "Intelligence Artificielle"));
        mgrSave.add(make_shared<PhDStudent>("Riad M", 6003, 3.9f, "Dr. Hadj", 2));
    } catch (...) { report("PERSIST setup", false); return; }

    int savedCount = mgrSave.count();
    double savedAvg = mgrSave.averageGpa();

    // SAVE
    try {
        PersistenceManager::save(mgrSave, testFile);
        report("SAVE to file", true);
    } catch (...) { report("SAVE to file", false); return; }

    // LOAD into fresh manager
    StudentManager mgrLoad;
    try {
        PersistenceManager::load(mgrLoad, testFile);
        report("LOAD from file", true);
    } catch (...) { report("LOAD from file", false); return; }

    // Compare counts
    report("PERSIST count match", mgrLoad.count() == savedCount);

    // Compare average GPA (within tolerance)
    double loadedAvg = mgrLoad.averageGpa();
    report("PERSIST avg GPA match", abs(loadedAvg - savedAvg) < 0.1);

    // Verify each student exists
    try {
        auto s1 = mgrLoad.findById(6001);
        report("PERSIST student 6001 exists", s1 != nullptr);
        report("PERSIST student 6001 name", s1->getName() == "Amira B");
        report("PERSIST student 6001 type", s1->getType() == "Licence");
    } catch (...) { report("PERSIST student 6001", false); }

    try {
        auto s2 = mgrLoad.findById(6002);
        report("PERSIST student 6002 exists", s2 != nullptr);
        report("PERSIST student 6002 type", s2->getType() == "Master");
    } catch (...) { report("PERSIST student 6002", false); }

    try {
        auto s3 = mgrLoad.findById(6003);
        report("PERSIST student 6003 exists", s3 != nullptr);
        report("PERSIST student 6003 type", s3->getType() == "Doctorat");
    } catch (...) { report("PERSIST student 6003", false); }

    // LOAD from missing file (no crash)
    try {
        StudentManager mgrEmpty;
        PersistenceManager::load(mgrEmpty, "data/NONEXISTENT_FILE.txt");
        report("LOAD missing file no crash", mgrEmpty.count() == 0);
    } catch (...) { report("LOAD missing file no crash", false); }

    // Cleanup
    remove(testFile.c_str());
}

// ============================================================
// MAIN
// ============================================================
int main() {
    cout << "=============================================\n";
    cout << " SUITE DE TESTS - PROJET IMS\n";
    cout << "=============================================\n";

    testStudents();
    testCRUD();
    testSorting();
    testExceptions();
    testPersistence();

    cout << "\n=============================================\n";
    cout << " RESULTATS FINAUX\n";
    cout << "=============================================\n";
    cout << "  Passes  : " << passed << endl;
    cout << "  Echoues : " << failed << endl;
    cout << "  Total   : " << passed + failed << endl;
    cout << "=============================================\n";

    return failed == 0 ? 0 : 1;
}
