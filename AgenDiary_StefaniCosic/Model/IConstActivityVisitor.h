#ifndef ICONSTACTIVITYVISITOR_H
#define ICONSTACTIVITYVISITOR_H

class Task;
class Appointment;
class Deadline;
class Reminder;
class Event;

class IConstActivityVisitor {
public:
    virtual ~IConstActivityVisitor() = default;
    virtual void handle(const Task* task) = 0;
    virtual void handle(const Appointment* appt) = 0;
    virtual void handle(const Deadline* deadline) = 0;
    virtual void handle(const Reminder* reminder) = 0;
    virtual void handle(const Event* event) = 0;
};

#endif // ICONSTACTIVITYVISITOR_H