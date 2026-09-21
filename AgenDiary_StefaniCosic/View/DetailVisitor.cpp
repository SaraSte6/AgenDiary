#include "DetailVisitor.h"
#include "Model/Task.h"
#include "Model/Appointment.h"
#include "Model/Deadline.h"
#include "Model/Reminder.h"
#include "Model/Event.h"

void DetailVisitor::handle(const Task* task) {
    result = QString("Priorità: %1")
             .arg(QString::fromStdString(task->getPriority()));
}

void DetailVisitor::handle(const Appointment* appt) {
    result = QString("Inizio: %1\nFine: %2\nLuogo: %3\nCon: %4\nConfermato: %5")
             .arg(appt->getStartTime().toString("dd/MM/yyyy HH:mm"))
             .arg(appt->getEndTime().toString("dd/MM/yyyy HH:mm"))
             .arg(QString::fromStdString(appt->getLocation()))
             .arg(appt->getPerson().empty() ? "-" : QString::fromStdString(appt->getPerson()))
             .arg(appt->isConfirmedAppointment() ? "Sì" : "No");
}

void DetailVisitor::handle(const Deadline* deadline) {
    result = QString("Data e Ora Scadenza: %1")
             .arg(deadline->getDueDate().toString("dd/MM/yyyy HH:mm"));
}

void DetailVisitor::handle(const Reminder* reminder) {
    result = QString("Data e Ora Sveglia: %1\nFrequenza: %2\nNotifica: %3\nNumero Rinvii: %4")
             .arg(reminder->getReminderTime().toString("dd/MM/yyyy HH:mm"))
             .arg(QString::fromStdString(reminder->getFrequency()))
             .arg(QString::fromStdString(reminder->getNotificationType()))
             .arg(reminder->getSnoozeCount());
}

void DetailVisitor::handle(const Event* event) {
    result = QString("Data Evento: %1\nCategoria: %2\nOrganizzatore: %3\nCosto: %4\nPartecipanti: %5")
             .arg(event->getEventDate().toString("dd/MM/yyyy"))
             .arg(QString::fromStdString(event->getEventType()))
             .arg(QString::fromStdString(event->getOrganizer()))
             .arg(event->isFreeEntry() ? "Gratuito" : QString::number(event->getTicketCost(), 'f', 2) + " €")
             .arg(event->getGuestCount());
}

QString DetailVisitor::getResult() const {
    return result;
}
