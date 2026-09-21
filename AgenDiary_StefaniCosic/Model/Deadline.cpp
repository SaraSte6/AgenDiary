#include "Deadline.h"

Deadline::Deadline(const std::string& title, const std::string& description, bool completed, const QDateTime& due)
    : Activity(title, description, completed), dueDate(due) {}

Activity* Deadline::clone() const {
    return new Deadline(*this);
}

bool Deadline::isUrgent() const {
    if (isCompleted()) return false;
    QDateTime now = QDateTime::currentDateTime();
    // Urgente se scade entro 48 ore o se è già scaduta
    return dueDate.isValid() && dueDate <= now.addDays(2);
}

bool Deadline::isValid() const {
    return !getTitle().empty() && dueDate.isValid();
}

QString Deadline::getSummary() const {
    QString dueStr = dueDate.isValid() ? dueDate.toString("dd/MM/yyyy HH:mm") : "-";
    return QString("Scadenza: %1").arg(dueStr);
}

QDateTime Deadline::getDueDate() const {
    return dueDate;
}

void Deadline::setDueDate(const QDateTime& due) {
    dueDate = due;
}

void Deadline::request(IActivityVisitor* visitor) {
    visitor->handle(this);
}

void Deadline::request(IConstActivityVisitor* visitor) const {
    visitor->handle(this);
}