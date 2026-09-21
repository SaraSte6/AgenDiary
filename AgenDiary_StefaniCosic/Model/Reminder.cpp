#include "Reminder.h"

Reminder::Reminder(const std::string& title, const std::string& description, bool completed,
                   const QDateTime& time, const std::string& freq, const std::string& notif, int snooze)
    : Activity(title, description, completed),
      reminderTime(time), frequency(freq), notificationType(notif), snoozeCount(snooze) {}

Activity* Reminder::clone() const {
    return new Reminder(*this);
}

bool Reminder::isUrgent() const {
    if (isCompleted()) return false;
    QDateTime now = QDateTime::currentDateTime();
    // Urgente se il promemoria è scaduto o scade entro un'ora
    return reminderTime.isValid() && reminderTime <= now.addSecs(3600);
}

bool Reminder::isValid() const {
    return !getTitle().empty() && reminderTime.isValid() && snoozeCount >= 0;
}

QString Reminder::getSummary() const {
    QString timeStr = reminderTime.isValid() ? reminderTime.toString("dd/MM HH:mm") : "-";
    return QString("Ora: %1 | Frequenza: %2").arg(timeStr, QString::fromStdString(frequency));
}

QDateTime Reminder::getReminderTime() const {
    return reminderTime;
}

std::string Reminder::getFrequency() const {
    return frequency;
}

std::string Reminder::getNotificationType() const {
    return notificationType;
}

int Reminder::getSnoozeCount() const {
    return snoozeCount;
}

void Reminder::setReminderTime(const QDateTime& time) {
    reminderTime = time;
}

void Reminder::setFrequency(const std::string& freq) {
    frequency = freq;
}

void Reminder::setNotificationType(const std::string& notif) {
    notificationType = notif;
}

void Reminder::setSnoozeCount(int count) {
    snoozeCount = count;
}

void Reminder::snooze(int minutes) {
    reminderTime = reminderTime.addSecs(minutes * 60);
    snoozeCount++;
}

bool Reminder::isRecurring() const {
    return !frequency.empty() && frequency != "Una tantum";
}

void Reminder::request(IActivityVisitor* visitor) {
    visitor->handle(this);
}

void Reminder::request(IConstActivityVisitor* visitor) const {
    visitor->handle(this);
}