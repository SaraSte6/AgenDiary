#ifndef TASK_H
#define TASK_H

#include "Activity.h"

class Task : public Activity {
private:
    std::string priority;

public:
    Task(const std::string& title, const std::string& description, bool completed, const std::string& priority = "Media");
    virtual ~Task() override = default;

    virtual Activity* clone() const override;
    virtual bool isUrgent() const override;
    virtual bool isValid() const override;
    virtual QString getSummary() const override;

    std::string getPriority() const;
    void setPriority(const std::string& priority);

    virtual void request(IActivityVisitor* visitor) override;
    virtual void request(IConstActivityVisitor* visitor) const override;
};

#endif // TASK_H