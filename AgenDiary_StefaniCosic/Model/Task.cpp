#include "Task.h"

Task::Task(const std::string& title, const std::string& description, bool completed, const std::string& priority)
    : Activity(title, description, completed), priority(priority) {}

Activity* Task::clone() const {
    return new Task(*this);
}

bool Task::isUrgent() const {
    return !isCompleted() && (priority == "Alta" || priority == "alta");
}

bool Task::isValid() const {
    return !getTitle().empty() && !priority.empty();
}

QString Task::getSummary() const {
    return QString("Priorità: %1").arg(QString::fromStdString(priority));
}

std::string Task::getPriority() const {
    return priority;
}

void Task::setPriority(const std::string& p) {
    priority = p;
}

void Task::request(IActivityVisitor* visitor) {
    visitor->handle(this);
}

void Task::request(IConstActivityVisitor* visitor) const {
    visitor->handle(this);
}
