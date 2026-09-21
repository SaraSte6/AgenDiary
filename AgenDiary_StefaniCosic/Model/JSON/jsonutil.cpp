#include "jsonutil.h"
#include <QDebug>
#include <QDate>
#include <QDateTime>

enum ActivityType {
    TaskType = 0,
    AppointmentType = 1,
    DeadlineType = 2,
    ReminderType = 3,
    EventType = 4
};

JSONutil::JSONutil(const QString& p) : document(new QJsonDocument()), path(p) {}

JSONutil::~JSONutil() {
    delete document;
}

void JSONutil::setPath(const QString& p) {
    path = p;
}

QString JSONutil::getPath() const {
    return path;
}

void JSONutil::handle(const Task* task) {
    QJsonObject jTask;
    jTask["type"] = ActivityType::TaskType;
    jTask["title"] = QString::fromStdString(task->getTitle());
    jTask["description"] = QString::fromStdString(task->getDescription());
    jTask["completed"] = task->isCompleted();
    jTask["priority"] = QString::fromStdString(task->getPriority());

    QJsonObject jDocObj = document->object();
    QJsonArray jArr = jDocObj["items"].toArray();
    jArr.append(jTask);
    jDocObj["items"] = jArr;
    document->setObject(jDocObj);
}

void JSONutil::handle(const Appointment* appt) {
    QJsonObject jAppt;
    jAppt["type"] = ActivityType::AppointmentType;
    jAppt["title"] = QString::fromStdString(appt->getTitle());
    jAppt["description"] = QString::fromStdString(appt->getDescription());
    jAppt["completed"] = appt->isCompleted();
    jAppt["startTime"] = appt->getStartTime().toString(Qt::ISODate);
    jAppt["endTime"] = appt->getEndTime().toString(Qt::ISODate);
    jAppt["location"] = QString::fromStdString(appt->getLocation());
    jAppt["person"] = QString::fromStdString(appt->getPerson());
    jAppt["isConfirmed"] = appt->isConfirmedAppointment();

    QJsonObject jDocObj = document->object();
    QJsonArray jArr = jDocObj["items"].toArray();
    jArr.append(jAppt);
    jDocObj["items"] = jArr;
    document->setObject(jDocObj);
}

void JSONutil::handle(const Deadline* deadline) {
    QJsonObject jDeadline;
    jDeadline["type"] = ActivityType::DeadlineType;
    jDeadline["title"] = QString::fromStdString(deadline->getTitle());
    jDeadline["description"] = QString::fromStdString(deadline->getDescription());
    jDeadline["completed"] = deadline->isCompleted();
    jDeadline["dueDate"] = deadline->getDueDate().toString(Qt::ISODate);

    QJsonObject jDocObj = document->object();
    QJsonArray jArr = jDocObj["items"].toArray();
    jArr.append(jDeadline);
    jDocObj["items"] = jArr;
    document->setObject(jDocObj);
}

void JSONutil::handle(const Reminder* reminder) {
    QJsonObject jRem;
    jRem["type"] = ActivityType::ReminderType;
    jRem["title"] = QString::fromStdString(reminder->getTitle());
    jRem["description"] = QString::fromStdString(reminder->getDescription());
    jRem["completed"] = reminder->isCompleted();
    jRem["reminderTime"] = reminder->getReminderTime().toString(Qt::ISODate);
    jRem["frequency"] = QString::fromStdString(reminder->getFrequency());
    jRem["notificationType"] = QString::fromStdString(reminder->getNotificationType());
    jRem["snoozeCount"] = reminder->getSnoozeCount();

    QJsonObject jDocObj = document->object();
    QJsonArray jArr = jDocObj["items"].toArray();
    jArr.append(jRem);
    jDocObj["items"] = jArr;
    document->setObject(jDocObj);
}

void JSONutil::handle(const Event* event) {
    QJsonObject jEv;
    jEv["type"] = ActivityType::EventType;
    jEv["title"] = QString::fromStdString(event->getTitle());
    jEv["description"] = QString::fromStdString(event->getDescription());
    jEv["completed"] = event->isCompleted();
    jEv["eventDate"] = event->getEventDate().toString(Qt::ISODate);
    jEv["eventType"] = QString::fromStdString(event->getEventType());
    jEv["organizer"] = QString::fromStdString(event->getOrganizer());
    jEv["ticketCost"] = event->getTicketCost();
    jEv["guestCount"] = event->getGuestCount();

    QJsonObject jDocObj = document->object();
    QJsonArray jArr = jDocObj["items"].toArray();
    jArr.append(jEv);
    jDocObj["items"] = jArr;
    document->setObject(jDocObj);
}

void JSONutil::saveActivitiesToJson(const std::vector<Activity*>& activities) {
    QJsonObject root;
    QJsonArray items;
    root["items"] = items;
    document->setObject(root);

    for (const auto* activity : activities) {
        if (activity) activity->request(this);
    }

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Impossibile aprire il file per la scrittura:" << path;
        return;
    }
    file.write(document->toJson(QJsonDocument::Indented));
    file.close();
}

std::vector<Activity*> JSONutil::getActivitiesFromJson() const {
    std::vector<Activity*> activities;
    QFile file(path);
    
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Impossibile aprire il file per la lettura:" << path;
        return activities;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument loadDoc(QJsonDocument::fromJson(data));
    if (loadDoc.isNull()) return activities;

    QJsonObject root = loadDoc.object();
    QJsonArray items = root["items"].toArray();

    for (const auto& itemRef : items) {
        QJsonObject item = itemRef.toObject();
        int type = item["type"].toInt();
        
        Activity* activity = nullptr;
        std::string title = item["title"].toString().toStdString();
        std::string desc = item["description"].toString().toStdString();
        bool completed = item["completed"].toBool();

        switch (type) {
            case ActivityType::TaskType:
                activity = new Task(title, desc, completed, item["priority"].toString().toStdString());
                break;
            case ActivityType::AppointmentType:
                activity = new Appointment(
                    title, desc, completed,
                    QDateTime::fromString(item["startTime"].toString(), Qt::ISODate),
                    QDateTime::fromString(item["endTime"].toString(), Qt::ISODate),
                    item["location"].toString().toStdString(),
                    item["person"].toString().toStdString(),
                    item["isConfirmed"].toBool()
                );
                break;
            case ActivityType::DeadlineType:
                activity = new Deadline(
                    title, desc, completed,
                    QDateTime::fromString(item["dueDate"].toString(), Qt::ISODate)
                );
                break;
            case ActivityType::ReminderType:
                activity = new Reminder(
                    title, desc, completed,
                    QDateTime::fromString(item["reminderTime"].toString(), Qt::ISODate),
                    item["frequency"].toString().toStdString(),
                    item["notificationType"].toString().toStdString(),
                    item["snoozeCount"].toInt()
                );
                break;
            case ActivityType::EventType:
                activity = new Event(
                    title, desc, completed,
                    QDate::fromString(item["eventDate"].toString(), Qt::ISODate),
                    item["eventType"].toString().toStdString(),
                    item["organizer"].toString().toStdString(),
                    item["ticketCost"].toDouble(),
                    item["guestCount"].toInt()
                );
                break;
        }

        if (activity) {
            activities.push_back(activity);
        }
    }

    return activities;
}