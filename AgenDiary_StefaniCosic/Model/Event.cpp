#include "Event.h"

Event::Event(const std::string& title, const std::string& description, bool completed,
             const QDate& date, const std::string& type, const std::string& org, double cost, int guests)
    : Activity(title, description, completed),
      eventDate(date), eventType(type), organizer(org),
      ticketCost(cost >= 0.0 ? cost : 0.0), guestCount(guests > 0 ? guests : 1) {}

Activity* Event::clone() const {
    return new Event(*this);
}

bool Event::isUrgent() const {
    if (isCompleted()) return false;
    QDate today = QDate::currentDate();
    // Urgente se l'evento è entro i prossimi 3 giorni
    return eventDate.isValid() && eventDate <= today.addDays(3);
}

bool Event::isValid() const {
    return !getTitle().empty() && eventDate.isValid() && ticketCost >= 0.0 && guestCount > 0;
}

QString Event::getSummary() const {
    QString dateStr = eventDate.isValid() ? eventDate.toString("dd/MM/yyyy") : "-";
    return QString("Data: %1 | Organizzatore: %2").arg(dateStr, QString::fromStdString(organizer));
}

QDate Event::getEventDate() const {
    return eventDate;
}

std::string Event::getEventType() const {
    return eventType;
}

std::string Event::getOrganizer() const {
    return organizer;
}

double Event::getTicketCost() const {
    return ticketCost;
}

int Event::getGuestCount() const {
    return guestCount;
}

void Event::setEventDate(const QDate& date) {
    eventDate = date;
}

void Event::setEventType(const std::string& type) {
    eventType = type;
}

void Event::setOrganizer(const std::string& org) {
    organizer = org;
}

void Event::setTicketCost(double cost) {
    ticketCost = (cost >= 0.0 ? cost : 0.0);
}

void Event::setGuestCount(int guests) {
    guestCount = (guests > 0 ? guests : 1);
}

bool Event::isFreeEntry() const {
    return ticketCost == 0.0;
}

void Event::request(IActivityVisitor* visitor) {
    visitor->handle(this);
}

void Event::request(IConstActivityVisitor* visitor) const {
    visitor->handle(this);
}