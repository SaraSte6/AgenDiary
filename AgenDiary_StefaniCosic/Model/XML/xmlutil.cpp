#include "xmlutil.h"
#include <QDebug>
#include <QDateTime>
#include <QDate>

XMLutil::XMLutil(const QString& p) : writer(nullptr), path(p) {}

void XMLutil::setPath(const QString& p) {
    path = p;
}

QString XMLutil::getPath() const {
    return path;
}

void XMLutil::handle(const Task* task) {
    if (!writer) return;
    writer->writeStartElement("Task");
    writer->writeTextElement("title", QString::fromStdString(task->getTitle()));
    writer->writeTextElement("description", QString::fromStdString(task->getDescription()));
    writer->writeTextElement("completed", task->isCompleted() ? "true" : "false");
    writer->writeTextElement("priority", QString::fromStdString(task->getPriority()));
    writer->writeEndElement();
}

void XMLutil::handle(const Appointment* appt) {
    if (!writer) return;
    writer->writeStartElement("Appointment");
    writer->writeTextElement("title", QString::fromStdString(appt->getTitle()));
    writer->writeTextElement("description", QString::fromStdString(appt->getDescription()));
    writer->writeTextElement("completed", appt->isCompleted() ? "true" : "false");
    writer->writeTextElement("startTime", appt->getStartTime().toString(Qt::ISODate));
    writer->writeTextElement("endTime", appt->getEndTime().toString(Qt::ISODate));
    writer->writeTextElement("location", QString::fromStdString(appt->getLocation()));
    writer->writeTextElement("person", QString::fromStdString(appt->getPerson()));
    writer->writeTextElement("isConfirmed", appt->isConfirmedAppointment() ? "true" : "false");
    writer->writeEndElement();
}

void XMLutil::handle(const Deadline* deadline) {
    if (!writer) return;
    writer->writeStartElement("Deadline");
    writer->writeTextElement("title", QString::fromStdString(deadline->getTitle()));
    writer->writeTextElement("description", QString::fromStdString(deadline->getDescription()));
    writer->writeTextElement("completed", deadline->isCompleted() ? "true" : "false");
    writer->writeTextElement("dueDate", deadline->getDueDate().toString(Qt::ISODate));
    writer->writeEndElement();
}

void XMLutil::handle(const Reminder* reminder) {
    if (!writer) return;
    writer->writeStartElement("Reminder");
    writer->writeTextElement("title", QString::fromStdString(reminder->getTitle()));
    writer->writeTextElement("description", QString::fromStdString(reminder->getDescription()));
    writer->writeTextElement("completed", reminder->isCompleted() ? "true" : "false");
    writer->writeTextElement("reminderTime", reminder->getReminderTime().toString(Qt::ISODate));
    writer->writeTextElement("frequency", QString::fromStdString(reminder->getFrequency()));
    writer->writeTextElement("notificationType", QString::fromStdString(reminder->getNotificationType()));
    writer->writeTextElement("snoozeCount", QString::number(reminder->getSnoozeCount()));
    writer->writeEndElement();
}

void XMLutil::handle(const Event* event) {
    if (!writer) return;
    writer->writeStartElement("Event");
    writer->writeTextElement("title", QString::fromStdString(event->getTitle()));
    writer->writeTextElement("description", QString::fromStdString(event->getDescription()));
    writer->writeTextElement("completed", event->isCompleted() ? "true" : "false");
    writer->writeTextElement("eventDate", event->getEventDate().toString(Qt::ISODate));
    writer->writeTextElement("eventType", QString::fromStdString(event->getEventType()));
    writer->writeTextElement("organizer", QString::fromStdString(event->getOrganizer()));
    writer->writeTextElement("ticketCost", QString::number(event->getTicketCost()));
    writer->writeTextElement("guestCount", QString::number(event->getGuestCount()));
    writer->writeEndElement();
}

void XMLutil::saveActivitiesToXml(const std::vector<Activity*>& activities) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Impossibile aprire il file per la scrittura:" << path;
        return;
    }

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("Activities");

    writer = &stream;
    for (const auto* activity : activities) {
        if (activity) activity->request(this);
    }
    writer = nullptr;

    stream.writeEndElement(); 
    stream.writeEndDocument();
    file.close();
}

std::vector<Activity*> XMLutil::getActivitiesFromXml() const {
    std::vector<Activity*> activities;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Impossibile aprire il file per la lettura:" << path;
        return activities;
    }

    QXmlStreamReader xml(&file);
    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartElement) {
            QString tag = xml.name().toString();
            if (tag == "Activities") continue;

            std::string title, desc;
            bool completed = false;

            if (tag == "Task") {
                std::string priority = "Media";
                while (xml.readNextStartElement()) {
                    if (xml.name() == QLatin1String("title")) title = xml.readElementText().toStdString();
                    else if (xml.name() == QLatin1String("description")) desc = xml.readElementText().toStdString();
                    else if (xml.name() == QLatin1String("completed")) completed = (xml.readElementText() == "true");
                    else if (xml.name() == QLatin1String("priority")) priority = xml.readElementText().toStdString();
                    else xml.skipCurrentElement();
                }
                activities.push_back(new Task(title, desc, completed, priority));
            }
            else if (tag == "Appointment") {
                QDateTime start, end;
                std::string loc, withP;
                bool conf = true;
                while (xml.readNextStartElement()) {
                    if (xml.name() == QLatin1String("title")) title = xml.readElementText().toStdString();
                    else if (xml.name() == QLatin1String("description")) desc = xml.readElementText().toStdString();
                    else if (xml.name() == QLatin1String("completed")) completed = (xml.readElementText() == "true");
                    else if (xml.name() == QLatin1String("startTime")) start = QDateTime::fromString(xml.readElementText(), Qt::ISODate);
                    else if (xml.name() == QLatin1String("endTime")) end = QDateTime::fromString(xml.readElementText(), Qt::ISODate);
                    else if (xml.name() == QLatin1String("location")) loc = xml.readElementText().toStdString();
                    else if (xml.name() == QLatin1String("person")) withP = xml.readElementText().toStdString();
                    else if (xml.name() == QLatin1String("isConfirmed")) conf = (xml.readElementText() == "true");
                    else xml.skipCurrentElement();
                }
                activities.push_back(new Appointment(title, desc, completed, start, end, loc, withP, conf));
            }
            else if (tag == "Deadline") {
                QDateTime due;
                while (xml.readNextStartElement()) {
                    if (xml.name() == QLatin1String("title")) title = xml.readElementText().toStdString();
                    else if (xml.name() == QLatin1String("description")) desc = xml.readElementText().toStdString();
                    else if (xml.name() == QLatin1String("completed")) completed = (xml.readElementText() == "true");
                    else if (xml.name() == QLatin1String("dueDate")) due = QDateTime::fromString(xml.readElementText(), Qt::ISODate);
                    else xml.skipCurrentElement();
                }
                activities.push_back(new Deadline(title, desc, completed, due));
            }
            else if (tag == "Reminder") {
                QDateTime rTime;
                std::string freq = "Una tantum", notif = "Popup";
                int snooze = 0;
                while (xml.readNextStartElement()) {
                    if (xml.name() == QLatin1String("title")) title = xml.readElementText().toStdString();
                    else if (xml.name() == QLatin1String("description")) desc = xml.readElementText().toStdString();
                    else if (xml.name() == QLatin1String("completed")) completed = (xml.readElementText() == "true");
                    else if (xml.name() == QLatin1String("reminderTime")) rTime = QDateTime::fromString(xml.readElementText(), Qt::ISODate);
                    else if (xml.name() == QLatin1String("frequency")) freq = xml.readElementText().toStdString();
                    else if (xml.name() == QLatin1String("notificationType")) notif = xml.readElementText().toStdString();
                    else if (xml.name() == QLatin1String("snoozeCount")) snooze = xml.readElementText().toInt();
                    else xml.skipCurrentElement();
                }
                activities.push_back(new Reminder(title, desc, completed, rTime, freq, notif, snooze));
            }
            else if (tag == "Event") {
                QDate eDate;
                std::string eType, org;
                double cost = 0.0;
                int guests = 1;
                while (xml.readNextStartElement()) {
                    if (xml.name() == QLatin1String("title")) title = xml.readElementText().toStdString();
                    else if (xml.name() == QLatin1String("description")) desc = xml.readElementText().toStdString();
                    else if (xml.name() == QLatin1String("completed")) completed = (xml.readElementText() == "true");
                    else if (xml.name() == QLatin1String("eventDate")) eDate = QDate::fromString(xml.readElementText(), Qt::ISODate);
                    else if (xml.name() == QLatin1String("eventType")) eType = xml.readElementText().toStdString();
                    else if (xml.name() == QLatin1String("organizer")) org = xml.readElementText().toStdString();
                    else if (xml.name() == QLatin1String("ticketCost")) cost = xml.readElementText().toDouble();
                    else if (xml.name() == QLatin1String("guestCount")) guests = xml.readElementText().toInt();
                    else xml.skipCurrentElement();
                }
                activities.push_back(new Event(title, desc, completed, eDate, eType, org, cost, guests));
            }
        }
    }
    file.close();
    return activities;
}