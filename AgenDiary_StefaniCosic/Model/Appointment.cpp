#include "Appointment.h"

Appointment::Appointment(const std::string& title, const std::string& description, bool completed,
                         const QDateTime& start, const QDateTime& end,
                         const std::string& loc, const std::string& pers, bool conf)
    : Activity(title, description, completed),
      startTime(start), endTime(end), location(loc), person(pers), isConfirmed(conf) {}

Activity* Appointment::clone() const {
    return new Appointment(*this);
}

bool Appointment::isUrgent() const {
    if (isCompleted()) return false;
    QDateTime now = QDateTime::currentDateTime();
    // Urgente se l'appuntamento inizia nelle prossime 24 ore o è già iniziato
    return startTime.isValid() && startTime <= now.addDays(1);
}

bool Appointment::isValid() const {
    return !getTitle().empty() && startTime.isValid() && endTime.isValid() && startTime <= endTime;
}

QString Appointment::getSummary() const {
    QString startStr = startTime.isValid() ? startTime.toString("dd/MM HH:mm") : "-";
    return QString("Inizio: %1 | Luogo: %2").arg(startStr, QString::fromStdString(location));
}

QDateTime Appointment::getStartTime() const {
    return startTime;
}

QDateTime Appointment::getEndTime() const {
    return endTime;
}

std::string Appointment::getLocation() const {
    return location;
}

std::string Appointment::getPerson() const {
    return person;
}

bool Appointment::isConfirmedAppointment() const {
    return isConfirmed;
}

void Appointment::setStartTime(const QDateTime& s) {
    startTime = s;
}

void Appointment::setEndTime(const QDateTime& e) {
    endTime = e;
}

void Appointment::setLocation(const std::string& l) {
    location = l;
}

void Appointment::setPerson(const std::string& p) {
    person = p;
}

void Appointment::setConfirmed(bool conf) {
    isConfirmed = conf;
}

void Appointment::request(IActivityVisitor* visitor) {
    visitor->handle(this);
}

void Appointment::request(IConstActivityVisitor* visitor) const {
    visitor->handle(this);
}