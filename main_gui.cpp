// main_gui.cpp - Point d'entree pour l'interface graphique Qt
#include <QApplication>
#include "src/gui/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
