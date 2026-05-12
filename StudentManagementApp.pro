QT += core gui widgets
QT += charts

TARGET = StudentManagementApp
TEMPLATE = app

CONFIG += c++17

# Source files
SOURCES += \
    main_gui.cpp \
    src/models/Student.cpp \
    src/models/UndergraduateStudent.cpp \
    src/models/GraduateStudent.cpp \
    src/models/PhDStudent.cpp \
    src/gui/MainWindow.cpp \
    src/gui/StudentDialog.cpp \
    src/gui/StatsDialog.cpp \
    src/gui/StudentDetailsDialog.cpp

# Header files (for MOC - Q_OBJECT)
HEADERS += \
    src/gui/MainWindow.h \
    src/gui/StudentDialog.h \
    src/gui/StatsDialog.h \
    src/gui/StudentDetailsDialog.h \
    src/gui/CsvExporter.h \
    src/models/Student.h \
    src/models/UndergraduateStudent.h \
    src/models/GraduateStudent.h \
    src/models/PhDStudent.h \
    src/persistence/PersistenceManager.h \
    src/services/StudentManager.h \
    src/exceptions/StudentExceptions.hpp

# Include path
INCLUDEPATH += .
