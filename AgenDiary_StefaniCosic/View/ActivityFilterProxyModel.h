#ifndef ACTIVITYFILTERPROXYMODEL_H
#define ACTIVITYFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "SidebarWidget.h"
#include "Controller/Controller.h"

class ActivityFilterProxyModel : public QSortFilterProxyModel {
    Q_OBJECT

private:
    Controller* m_controller;
    FilterCategory m_category;

public:
    explicit ActivityFilterProxyModel(Controller* controller, QObject* parent = nullptr)
        : QSortFilterProxyModel(parent), m_controller(controller), m_category(FilterCategory::All) {
        setFilterCaseSensitivity(Qt::CaseInsensitive);
    }

    void setFilterCategory(FilterCategory cat) {
        m_category = cat;
        invalidate();
    }

    FilterCategory getFilterCategory() const {
        return m_category;
    }

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override {
        Q_UNUSED(source_parent);
        if (!m_controller) return false;

        const std::vector<Activity*>& activities = m_controller->getActivities();
        if (source_row < 0 || source_row >= static_cast<int>(activities.size())) return false;

        const Activity* act = activities[source_row];
        if (!act) return false;

        // Filtro per categoria
        if (m_category == FilterCategory::Urgent && !act->isUrgent()) return false;
        if (m_category == FilterCategory::Pending && act->isCompleted()) return false;
        if (m_category == FilterCategory::Completed && !act->isCompleted()) return false;

        // Ricerca
        QString filter = filterRegularExpression().pattern();
        if (filter.isEmpty()) return true;

        QString title = QString::fromStdString(act->getTitle());
        QString desc = QString::fromStdString(act->getDescription());
        return title.contains(filter, Qt::CaseInsensitive) || desc.contains(filter, Qt::CaseInsensitive);
    }
};

#endif // ACTIVITYFILTERPROXYMODEL_H
