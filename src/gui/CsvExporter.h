// src/gui/CsvExporter.h - Export CSV des étudiants
#ifndef CSVEXPORTER_H
#define CSVEXPORTER_H

#include <string>
#include <fstream>
#include <memory>
#include "../services/StudentManager.h"

class CsvExporter {
public:
    // Exporte tous les étudiants au format CSV compatible Excel.
    // Retourne true si succès, false si erreur d'écriture.
    static bool exportToFile(const StudentManager& mgr, const std::string& path) {
        std::ofstream file(path);
        if (!file.is_open()) return false;

        // BOM UTF-8 pour compatibilité Excel
        file << "\xEF\xBB\xBF";

        // En-têtes
        file << "ID;Nom;Type;GPA;Bourse (EUR)\n";

        // Données
        const auto& all = mgr.getAll();
        for (const auto& pair : all) {
            const auto& s = pair.second;
            if (!s) continue;
            file << s->getId() << ";"
                 << s->getName() << ";"
                 << s->getType() << ";"
                 << s->getGpa() << ";"
                 << s->computeScholarship() << "\n";
        }

        file.close();
        return file.good() || !file.fail();
    }
};

#endif // CSVEXPORTER_H
