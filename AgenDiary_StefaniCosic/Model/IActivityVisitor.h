#ifndef IACTIVITYVISITOR_H
#define IACTIVITYVISITOR_H

class Task;
class Appointment;
class Deadline;
class Reminder;
class Event;

class IActivityVisitor {
public:
    virtual ~IActivityVisitor() = default;
    virtual void handle(Task* task) = 0;
    virtual void handle(Appointment* appt) = 0;
    virtual void handle(Deadline* deadline) = 0;
    virtual void handle(Reminder* reminder) = 0;
    virtual void handle(Event* event) = 0;
};

#endif // IACTIVITYVISITOR_H