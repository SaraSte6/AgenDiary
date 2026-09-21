#ifndef EDITVISITOR_H
#define EDITVISITOR_H

#include "Model/IConstActivityVisitor.h"

class ActivityEditor;

class EditVisitor : public IConstActivityVisitor {
private:
    ActivityEditor* m_editor;

public:
    explicit EditVisitor(ActivityEditor* editor);
    virtual ~EditVisitor() override = default;

    virtual void handle(const Task* task) override;
    virtual void handle(const Appointment* appt) override;
    virtual void handle(const Deadline* deadline) override;
    virtual void handle(const Reminder* reminder) override;
    virtual void handle(const Event* event) override;
};

#endif //EDITVISITOR_H