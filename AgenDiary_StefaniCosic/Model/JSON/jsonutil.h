#ifndef JSONUTIL_H
#define JSONUTIL_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QString>
#include <vector>

#include "IConstActivityVisitor.h"
#include "Activity.h"
#include "Task.h"
#include "Appointment.h"
#include "Deadline.h"
#include "Reminder.h"
#include "Event.h"

class JSONutil : public IConstActivityVisitor {
private:
    QJsonDocument* document;
    QString path;

public:
    explicit JSONutil(const QString& path = "");
    virtual ~JSONutil() override;

    void setPath(const QString& path);
    QString getPath() const;

    virtual void handle(const Task* task) override;
    virtual void handle(const Appointment* appt) override;
    virtual void handle(const Deadline* deadline) override;
    virtual void handle(const Reminder* reminder) override;
    virtual void handle(const Event* event) override;

    std::vector<Activity*> getActivitiesFromJson() const;
    void saveActivitiesToJson(const std::vector<Activity*>& activities);
};

#endif // JSONUTIL_H