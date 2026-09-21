#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <string>
#include <QString>
#include "IActivityVisitor.h"
#include "IConstActivityVisitor.h"

class Activity {
private:
    std::string title;
    std::string description;
    bool completed;

protected:
    Activity(const std::string& title, const std::string& description, bool completed = false);

public:
    virtual ~Activity() = default;

    virtual Activity* clone() const = 0;
    virtual bool isUrgent() const = 0;
    virtual bool isValid() const = 0;
    virtual QString getSummary() const = 0;

    std::string getTitle() const;
    std::string getDescription() const;
    bool isCompleted() const;

    void setTitle(const std::string& title);
    void setDescription(const std::string& description);
    void setCompleted(bool completed);

    virtual void request(IActivityVisitor* visitor) = 0;
    virtual void request(IConstActivityVisitor* visitor) const = 0;
};

#endif // ACTIVITY_H