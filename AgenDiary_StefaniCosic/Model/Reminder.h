#ifndef REMINDER_H
#define REMINDER_H

#include "Activity.h"
#include <QDateTime>

class Reminder : public Activity {
private:
    QDateTime reminderTime;
    std::string frequency;
    std::string notificationType;
    int snoozeCount;

public:
    Reminder(const std::string& title, const std::string& description, bool completed,
             const QDateTime& reminderTime, const std::string& frequency = "Una tantum",
             const std::string& notificationType = "Popup", int snoozeCount = 0);
    virtual ~Reminder() override = default;

    virtual Activity* clone() const override;
    virtual bool isUrgent() const override;
    virtual bool isValid() const override;
    virtual QString getSummary() const override;

    QDateTime getReminderTime() const;
    std::string getFrequency() const;
    std::string getNotificationType() const;
    int getSnoozeCount() const;

    void setReminderTime(const QDateTime& time);
    void setFrequency(const std::string& freq);
    void setNotificationType(const std::string& notif);
    void setSnoozeCount(int count);

    void snooze(int minutes = 10);
    bool isRecurring() const;

    virtual void request(IActivityVisitor* visitor) override;
    virtual void request(IConstActivityVisitor* visitor) const override;
};

#endif // REMINDER_H