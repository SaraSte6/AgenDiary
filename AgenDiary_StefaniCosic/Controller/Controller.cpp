#include "Controller.h"

Controller::Controller(QObject* parent) 
    : QObject(parent), jUtil(new JSONutil("")), xUtil(new XMLutil("")) {}

Controller::~Controller() {
    clearItems();
    delete jUtil;
    delete xUtil;
}

void Controller::clearItems() {
    for (Activity* a : items) {
        delete a;
    }
    items.clear();
}

const std::vector<Activity*>& Controller::getActivities() const {
    return items;
}

Activity* Controller::getActivityAt(int index) const {
    if (index >= 0 && index < items.size()) {
        return items[index];
    }
    return nullptr;
}

void Controller::addActivity(Activity* a) {
    if (a) {
        items.push_back(a);
        emit dataChanged();
    }
}

void Controller::removeActivityAt(int index) {
    if (index >= 0 && index < items.size()) {
        delete items[index];
        items.erase(items.begin() + index);
        emit dataChanged();
    }
}

void Controller::updateActivity(int index, Activity* newAct) {
    if (index >= 0 && index < items.size() && newAct) {
        delete items[index];
        items[index] = newAct;
        emit dataChanged();
    }
}

void Controller::toggleActivityCompleted(int index) {
    if (index >= 0 && index < items.size()) {
        items[index]->setCompleted(!items[index]->isCompleted());
        emit dataChanged();
    }
}

std::vector<Activity*> Controller::searchActivities(const std::string& query) const {
    if (query.empty()) return items;

    std::vector<Activity*> results;
    QString q = QString::fromStdString(query).toLower();

    for (Activity* a : items) {
        QString title = QString::fromStdString(a->getTitle()).toLower();
        QString desc = QString::fromStdString(a->getDescription()).toLower();

        if (title.contains(q) || desc.contains(q)) {
            results.push_back(a);
        }
    }
    return results;
}

bool Controller::saveToFile(const QString& path) {
    if (path.endsWith(".json")) {
        jUtil->setPath(path);
        jUtil->saveActivitiesToJson(items);
        return true;
    } else if (path.endsWith(".xml")) {
        xUtil->setPath(path);
        xUtil->saveActivitiesToXml(items);
        return true;
    }
    return false;
}

bool Controller::loadFromFile(const QString& path) {
    clearItems();
    if (path.endsWith(".json")) {
        jUtil->setPath(path);
        items = jUtil->getActivitiesFromJson();
    } else if (path.endsWith(".xml")) {
        xUtil->setPath(path);
        items = xUtil->getActivitiesFromXml();
    } else {
        return false;
    }
    emit dataChanged();
    return true;
}