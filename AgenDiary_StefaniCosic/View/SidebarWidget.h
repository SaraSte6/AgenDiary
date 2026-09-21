#ifndef SIDEBARWIDGET_H
#define SIDEBARWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QButtonGroup>

enum class FilterCategory {
    All,
    Urgent,
    Pending,
    Completed
};

class SidebarWidget : public QWidget {
    Q_OBJECT

private:
    QButtonGroup* m_buttonGroup;
    QPushButton* m_allBtn;
    QPushButton* m_urgentBtn;
    QPushButton* m_pendingBtn;
    QPushButton* m_completedBtn;

    void setupUi();

public:
    explicit SidebarWidget(QWidget* parent = nullptr);
    virtual ~SidebarWidget() override = default;

    FilterCategory getCurrentFilter() const;

signals:
    void filterChanged(FilterCategory category);
};

#endif // SIDEBARWIDGET_H
