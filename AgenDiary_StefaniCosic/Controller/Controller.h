#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <vector>
#include <string>
#include <QString>

#include "Activity.h"
#include "JSON/jsonutil.h"
#include "XML/xmlutil.h"

class Controller : public QObject {
    Q_OBJECT

private:
    std::vector<Activity*> items;
    JSONutil* jUtil;
    XMLutil* xUtil;

    void clearItems();

public:
    explicit Controller(QObject* parent = nullptr);
    virtual ~Controller() override;

    const std::vector<Activity*>& getActivities() const;
    Activity* getActivityAt(int index) const;

    void addActivity(Activity* activity);
    void updateActivity(int index, Activity* newActivity);
    void removeActivityAt(int index);
    void toggleActivityCompleted(int index);

    std::vector<Activity*> searchActivities(const std::string& query) const;

    bool saveToFile(const QString& filepath);
    bool loadFromFile(const QString& filepath);

signals:
    void dataChanged();
};

#endif // CONTROLLER_H