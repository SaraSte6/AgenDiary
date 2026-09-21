#ifndef XMLUTIL_H
#define XMLUTIL_H

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
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

class XMLutil : public IConstActivityVisitor {
private:
    QXmlStreamWriter* writer;
    QString path;

public:
    explicit XMLutil(const QString& path = "");
    virtual ~XMLutil() override = default;

    void setPath(const QString& path);
    QString getPath() const;

    virtual void handle(const Task* task) override;
    virtual void handle(const Appointment* appt) override;
    virtual void handle(const Deadline* deadline) override;
    virtual void handle(const Reminder* reminder) override;
    virtual void handle(const Event* event) override;

    std::vector<Activity*> getActivitiesFromXml() const;
    void saveActivitiesToXml(const std::vector<Activity*>& activities);
};

#endif // XMLUTIL_H