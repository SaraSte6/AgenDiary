#include "Activity.h"

Activity::Activity(const std::string& t, const std::string& d, bool c)
    : title(t), description(d), completed(c) {}

std::string Activity::getTitle() const {
    return title;
}

std::string Activity::getDescription() const {
    return description;
}

bool Activity::isCompleted() const {
    return completed;
}

void Activity::setTitle(const std::string& t) {
    title = t;
}

void Activity::setDescription(const std::string& d) {
    description = d;
}

void Activity::setCompleted(bool c) {
    completed = c;
}