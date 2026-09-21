TEMPLATE = app
TARGET = AgenDiary
QT += core gui widgets xml
RESOURCES += resources.qrc
INCLUDEPATH += . Model View Controller
CONFIG += c++17

SOURCES += \
    main.cpp \
    Model/Activity.cpp \
    Model/Task.cpp \
    Model/Appointment.cpp \
    Model/Deadline.cpp \
    Model/Reminder.cpp \
    Model/Event.cpp \
    Model/JSON/jsonutil.cpp \
    Model/XML/xmlutil.cpp \
    Controller/Controller.cpp \
    View/ActivityListModel.cpp \
    View/DetailVisitor.cpp \
    View/IconVisitor.cpp \
    View/EditVisitor.cpp \
    View/ActivityDetailWidget.cpp \
    View/SidebarWidget.cpp \
    View/ActivityEditor.cpp \
    View/MainWindow.cpp

HEADERS += \
    Model/IActivityVisitor.h \
    Model/IConstActivityVisitor.h \
    Model/Activity.h \
    Model/Task.h \
    Model/Appointment.h \
    Model/Deadline.h \
    Model/Reminder.h \
    Model/Event.h \
    Model/JSON/jsonutil.h \
    Model/XML/xmlutil.h \
    Controller/Controller.h \
    View/ActivityListModel.h \
    View/DetailVisitor.h \
    View/IconVisitor.h \
    View/EditVisitor.h \
    View/ActivityDetailWidget.h \
    View/SidebarWidget.h \
    View/ActivityFilterProxyModel.h \
    View/ActivityEditor.h \
    View/MainWindow.h