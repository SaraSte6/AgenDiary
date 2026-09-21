#include "EditVisitor.h"
#include "ActivityEditor.h"
#include "Model/Task.h"
#include "Model/Appointment.h"
#include "Model/Deadline.h"
#include "Model/Reminder.h"
#include "Model/Event.h"

EditVisitor::EditVisitor(ActivityEditor* editor) : m_editor(editor) {}

void EditVisitor::handle(const Task* t) {
    m_editor->typeCombo->setCurrentIndex(0);
    m_editor->priorityCombo->setCurrentText(QString::fromStdString(t->getPriority()));
}

void EditVisitor::handle(const Appointment* a) {
    m_editor->typeCombo->setCurrentIndex(1);
    m_editor->startEdit->setDateTime(a->getStartTime());
    m_editor->endEdit->setDateTime(a->getEndTime());
    m_editor->locationEdit->setText(QString::fromStdString(a->getLocation()));
    m_editor->personEdit->setText(QString::fromStdString(a->getPerson()));
    m_editor->confirmedCheck->setChecked(a->isConfirmedAppointment());
}

void EditVisitor::handle(const Deadline* d) {
    m_editor->typeCombo->setCurrentIndex(2);
    m_editor->dueEdit->setDateTime(d->getDueDate());
}

void EditVisitor::handle(const Reminder* r) {
    m_editor->typeCombo->setCurrentIndex(3);
    m_editor->reminderTimeEdit->setDateTime(r->getReminderTime());
    m_editor->frequencyCombo->setCurrentText(QString::fromStdString(r->getFrequency()));
    m_editor->notifTypeCombo->setCurrentText(QString::fromStdString(r->getNotificationType()));
    m_editor->snoozeSpin->setValue(r->getSnoozeCount());
}

void EditVisitor::handle(const Event* e) {
    m_editor->typeCombo->setCurrentIndex(4);
    m_editor->eventDateEdit->setDate(e->getEventDate());
    m_editor->eventTypeEdit->setText(QString::fromStdString(e->getEventType()));
    m_editor->organizerEdit->setText(QString::fromStdString(e->getOrganizer()));
    m_editor->ticketSpin->setValue(e->getTicketCost());
    m_editor->guestSpin->setValue(e->getGuestCount());
}