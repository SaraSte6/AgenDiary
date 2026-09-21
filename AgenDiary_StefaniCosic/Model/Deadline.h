#ifndef DEADLINE_H
#define DEADLINE_H

#include "Activity.h"
#include <QDateTime>

class Deadline : public Activity {
private:
    QDateTime dueDate;

public:
    Deadline(const std::string& title, const std::string& description, bool completed, const QDateTime& dueDate);
    virtual ~Deadline() override = default;

    virtual Activity* clone() const override;
    virtual bool isUrgent() const override;
    virtual bool isValid() const override;
    virtual QString getSummary() const override;

    QDateTime getDueDate() const;
    void setDueDate(const QDateTime& due);

    virtual void request(IActivityVisitor* visitor) override;
    virtual void request(IConstActivityVisitor* visitor) const override;
};

#endif // DEADLINE_H