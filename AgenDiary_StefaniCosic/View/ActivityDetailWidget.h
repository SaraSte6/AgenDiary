#ifndef ACTIVITYDETAILWIDGET_H
#define ACTIVITYDETAILWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "Model/Activity.h"

class ActivityDetailWidget : public QWidget {
    Q_OBJECT

private:
    QLabel* m_iconLabel;
    QLabel* m_titleLabel;
    QLabel* m_typeLabel;
    QLabel* m_badgeLabel;
    QLabel* m_descriptionLabel;
    QLabel* m_extraInfoLabel;

    QPushButton* m_toggleButton;
    QPushButton* m_editButton;
    QPushButton* m_deleteButton;

    void setupUi();

public:
    explicit ActivityDetailWidget(QWidget* parent = nullptr);
    virtual ~ActivityDetailWidget() override = default;

    void displayActivity(const Activity* activity);
    void clearDisplay();

signals:
    void toggleCompletedRequested();
    void editRequested();
    void deleteRequested();
};

#endif // ACTIVITYDETAILWIDGET_H
