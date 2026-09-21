#ifndef ACTIVITYEDITOR_H
#define ACTIVITYEDITOR_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QDateTimeEdit>
#include <QDateEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QStackedWidget>
#include <QPushButton>

#include "Model/Activity.h"

class EditVisitor;

class ActivityEditor : public QWidget {
    Q_OBJECT
    friend class EditVisitor;

private:
    QComboBox* typeCombo;
    QLineEdit* titleEdit;
    QTextEdit* descEdit;
    QCheckBox* completedCheck;

    QStackedWidget* specificStack;

    QComboBox* priorityCombo;

    QDateTimeEdit* startEdit;
    QDateTimeEdit* endEdit;
    QLineEdit* locationEdit;
    QLineEdit* personEdit;
    QCheckBox* confirmedCheck;

    QDateTimeEdit* dueEdit;

    QDateTimeEdit* reminderTimeEdit;
    QComboBox* frequencyCombo;
    QComboBox* notifTypeCombo;
    QSpinBox* snoozeSpin;

    QDateEdit* eventDateEdit;
    QLineEdit* eventTypeEdit;
    QLineEdit* organizerEdit;
    QDoubleSpinBox* ticketSpin;
    QSpinBox* guestSpin;

    QPushButton* saveBtn;
    QPushButton* cancelBtn;

    int editIndex;

    void setupUi();
    QWidget* createTaskPage();
    QWidget* createAppointmentPage();
    QWidget* createDeadlinePage();
    QWidget* createReminderPage();
    QWidget* createEventPage();

public:
    explicit ActivityEditor(QWidget* parent = nullptr);
    virtual ~ActivityEditor() override = default;

    void prepareForNew();
    void prepareForEdit(const Activity* activity, int index);
    Activity* createActivityFromForm() const;
    int getEditIndex() const;

signals:
    void saveRequested();
    void cancelRequested();
};

#endif //ACTIVITYEDITOR_H