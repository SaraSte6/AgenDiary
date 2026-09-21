#ifndef DETAILVISITOR_H
#define DETAILVISITOR_H

#include "Model/IConstActivityVisitor.h"
#include <QString>

class DetailVisitor : public IConstActivityVisitor {
private:
    QString result;

public:
    virtual ~DetailVisitor() override = default;

    virtual void handle(const Task* task) override;
    virtual void handle(const Appointment* appt) override;
    virtual void handle(const Deadline* deadline) override;
    virtual void handle(const Reminder* reminder) override;
    virtual void handle(const Event* event) override;

    QString getResult() const;
};

#endif // DETAILVISITOR_H
