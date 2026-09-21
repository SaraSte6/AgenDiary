#ifndef ICONVISITOR_H
#define ICONVISITOR_H

#include "Model/IConstActivityVisitor.h"
#include <QString>
#include <QIcon>
#include <QColor>

class IconVisitor : public IConstActivityVisitor {
private:
    QString typeName;
    QIcon icon;
    QColor typeColor;

public:
    IconVisitor() = default;
    virtual ~IconVisitor() override = default;

    QString getTypeName() const;
    QIcon getIcon() const;
    QColor getTypeColor() const;

    virtual void handle(const Task* task) override;
    virtual void handle(const Appointment* appt) override;
    virtual void handle(const Deadline* deadline) override;
    virtual void handle(const Reminder* reminder) override;
    virtual void handle(const Event* event) override;
};

#endif // ICONVISITOR_H
