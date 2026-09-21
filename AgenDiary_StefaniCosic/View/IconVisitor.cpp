#include "IconVisitor.h"
#include "Model/Task.h"
#include "Model/Appointment.h"
#include "Model/Deadline.h"
#include "Model/Reminder.h"
#include "Model/Event.h"

QString IconVisitor::getTypeName() const {
    return typeName;
}

QIcon IconVisitor::getIcon() const {
    return icon;
}

QColor IconVisitor::getTypeColor() const {
    return typeColor;
}

void IconVisitor::handle(const Task* task) {
    typeName = "Task";
    icon = QIcon(":/Resources/task.jpg");
    typeColor = QColor("#3B82F6"); 
}

void IconVisitor::handle(const Appointment* appt) {
    typeName = "Appuntamento";
    icon = QIcon(":/Resources/appointement.jpg");
    typeColor = QColor("#10B981"); 
}

void IconVisitor::handle(const Deadline* deadline) {
    typeName = "Scadenza";
    icon = QIcon(":/Resources/deadline.jpg");
    typeColor = QColor("#EF4444"); 
}

void IconVisitor::handle(const Reminder* reminder) {
    typeName = "Promemoria";
    icon = QIcon(":/Resources/reminder.jpg");
    typeColor = QColor("#F59E0B"); 
}

void IconVisitor::handle(const Event* event) {
    typeName = "Evento";
    icon = QIcon(":/Resources/event.png");
    typeColor = QColor("#8B5CF6");
}
