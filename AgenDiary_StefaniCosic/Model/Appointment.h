#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include "Activity.h"
#include <QDateTime>

class Appointment : public Activity {
private:
    QDateTime startTime;
    QDateTime endTime;
    std::string location;
    std::string person;
    bool isConfirmed;

public:
    Appointment(const std::string& title, const std::string& description, bool completed,
                const QDateTime& startTime, const QDateTime& endTime,
                const std::string& location, const std::string& person = "", bool confirmed = true);
    virtual ~Appointment() override = default;

    virtual Activity* clone() const override;
    virtual bool isUrgent() const override;
    virtual bool isValid() const override;
    virtual QString getSummary() const override;

    QDateTime getStartTime() const;
    QDateTime getEndTime() const;
    std::string getLocation() const;
    std::string getPerson() const;
    bool isConfirmedAppointment() const;

    void setStartTime(const QDateTime& start);
    void setEndTime(const QDateTime& end);
    void setLocation(const std::string& loc);
    void setPerson(const std::string& pers);
    void setConfirmed(bool conf);

    virtual void request(IActivityVisitor* visitor) override;
    virtual void request(IConstActivityVisitor* visitor) const override;
};

#endif // APPOINTMENT_H