#include "ActivityListModel.h"
#include <QIcon>
#include "Controller/Controller.h"
#include "View/IconVisitor.h"

ActivityListModel::ActivityListModel(Controller* controller, QObject *parent)
    : QAbstractListModel(parent), m_controller(controller) {
    if (m_controller) {
        connect(m_controller, &Controller::dataChanged, this, &ActivityListModel::refresh);
    }
}

ActivityListModel::~ActivityListModel() {}

int ActivityListModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid() || !m_controller) return 0;
    return static_cast<int>(m_controller->getActivities().size());
}

QVariant ActivityListModel::data(const QModelIndex &index, int role) const {
    if (!m_controller) return QVariant();

    const std::vector<Activity*>& activities = m_controller->getActivities();
    if (!index.isValid() || index.row() >= static_cast<int>(activities.size()))
        return QVariant();

    Activity* activity = activities[index.row()];
    if (!activity) return QVariant();

    if (role == Qt::DisplayRole) {
        QString title = QString::fromStdString(activity->getTitle());
        if (activity->isCompleted()) {
            title += " FATTO";
        } else if (activity->isUrgent()) {
            title += " !!!";
        }
        return title;
    }
    
    if (role == Qt::DecorationRole) {
        IconVisitor iconVisitor;
        activity->request(&iconVisitor);
        return iconVisitor.getIcon();
    }

    if (role == Qt::ToolTipRole) {
        return activity->getSummary();
    }
    
    return QVariant();
}

void ActivityListModel::refresh() {
    beginResetModel();
    endResetModel();
}
