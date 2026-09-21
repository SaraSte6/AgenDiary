#ifndef EVENT_H
#define EVENT_H

#include "Activity.h"
#include <QDate>

class Event : public Activity {
private:
    QDate eventDate;
    std::string eventType;
    std::string organizer;
    double ticketCost;
    int guestCount;

public:
    Event(const std::string& title, const std::string& description, bool completed,
          const QDate& eventDate, const std::string& eventType,
          const std::string& organizer, double ticketCost = 0.0, int guestCount = 1);
    virtual ~Event() override = default;

    virtual Activity* clone() const override;
    virtual bool isUrgent() const override;
    virtual bool isValid() const override;
    virtual QString getSummary() const override;

    QDate getEventDate() const;
    std::string getEventType() const;
    std::string getOrganizer() const;
    double getTicketCost() const;
    int getGuestCount() const;

    void setEventDate(const QDate& date);
    void setEventType(const std::string& type);
    void setOrganizer(const std::string& org);
    void setTicketCost(double cost);
    void setGuestCount(int guests);

    bool isFreeEntry() const;

    virtual void request(IActivityVisitor* visitor) override;
    virtual void request(IConstActivityVisitor* visitor) const override;
};

#endif // EVENT_H