#ifndef ACTIVITYLISTMODEL_H
#define ACTIVITYLISTMODEL_H

#include <QAbstractListModel>
#include "Controller/Controller.h"

class ActivityListModel : public QAbstractListModel {
    Q_OBJECT

private:
    Controller* m_controller;

public:
    explicit ActivityListModel(Controller* controller, QObject *parent = nullptr);
    virtual ~ActivityListModel() override;

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

public slots:
    void refresh();
};

#endif // ACTIVITYLISTMODEL_H