#include "ActivityEditor.h"
#include "EditVisitor.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QGroupBox>

#include "Model/Task.h"
#include "Model/Appointment.h"
#include "Model/Deadline.h"
#include "Model/Reminder.h"
#include "Model/Event.h"

ActivityEditor::ActivityEditor(QWidget* parent)
    : QWidget(parent), editIndex(-1) {
    setupUi();
}

void ActivityEditor::setupUi() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 25, 30, 25);
    mainLayout->setSpacing(15);

    QLabel* header = new QLabel("Dettagli Attività", this);
    header->setStyleSheet("font-size: 20px; font-weight: bold; color: #1E3A8A;");
    mainLayout->addWidget(header);

    QGroupBox* commonBox = new QGroupBox("Informazioni Generali", this);
    QFormLayout* commonLayout = new QFormLayout(commonBox);

    typeCombo = new QComboBox(this);
    typeCombo->addItems({"Task", "Appuntamento", "Scadenza", "Promemoria", "Evento"});

    titleEdit = new QLineEdit(this);
    titleEdit->setPlaceholderText("Inserisci il titolo...");

    descEdit = new QTextEdit(this);
    descEdit->setMaximumHeight(70);
    descEdit->setPlaceholderText("Inserisci una descrizione...");

    completedCheck = new QCheckBox("Completata", this);

    commonLayout->addRow("Tipo:", typeCombo);
    commonLayout->addRow("Titolo:", titleEdit);
    commonLayout->addRow("Descrizione:", descEdit);
    commonLayout->addRow("", completedCheck);
    mainLayout->addWidget(commonBox);

    specificStack = new QStackedWidget(this);
    specificStack->addWidget(createTaskPage());
    specificStack->addWidget(createAppointmentPage());
    specificStack->addWidget(createDeadlinePage());
    specificStack->addWidget(createReminderPage());
    specificStack->addWidget(createEventPage());
    mainLayout->addWidget(specificStack);

    connect(typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            specificStack, &QStackedWidget::setCurrentIndex);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    cancelBtn = new QPushButton("Annulla", this);
    cancelBtn->setStyleSheet("background-color: #9CA3AF; color: white; padding: 8px 16px; border-radius: 4px; font-weight: 500;");

    saveBtn = new QPushButton("Salva Attività", this);
    saveBtn->setStyleSheet("background-color: #2563EB; color: white; font-weight: bold; padding: 8px 16px; border-radius: 4px;");

    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(saveBtn);
    mainLayout->addLayout(btnLayout);

    connect(saveBtn, &QPushButton::clicked, this, &ActivityEditor::saveRequested);
    connect(cancelBtn, &QPushButton::clicked, this, &ActivityEditor::cancelRequested);
}

QWidget* ActivityEditor::createTaskPage() {
    QGroupBox* box = new QGroupBox("Dettagli Task", this);
    QFormLayout* layout = new QFormLayout(box);
    priorityCombo = new QComboBox(this);
    priorityCombo->addItems({"Bassa", "Media", "Alta"});
    layout->addRow("Priorità:", priorityCombo);
    return box;
}

QWidget* ActivityEditor::createAppointmentPage() {
    QGroupBox* box = new QGroupBox("Dettagli Appuntamento", this);
    QFormLayout* layout = new QFormLayout(box);
    startEdit = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    endEdit = new QDateTimeEdit(QDateTime::currentDateTime().addSecs(3600), this);
    startEdit->setCalendarPopup(true);
    endEdit->setCalendarPopup(true);
    locationEdit = new QLineEdit(this);
    personEdit = new QLineEdit(this);
    confirmedCheck = new QCheckBox("Appuntamento Confermato", this);
    confirmedCheck->setChecked(true);

    layout->addRow("Data e Ora Inizio:", startEdit);
    layout->addRow("Data e Ora Fine:", endEdit);
    layout->addRow("Luogo:", locationEdit);
    layout->addRow("Con chi:", personEdit);
    layout->addRow("", confirmedCheck);
    return box;
}

QWidget* ActivityEditor::createDeadlinePage() {
    QGroupBox* box = new QGroupBox("Dettagli Scadenza", this);
    QFormLayout* layout = new QFormLayout(box);
    dueEdit = new QDateTimeEdit(QDateTime::currentDateTime().addDays(1), this);
    dueEdit->setCalendarPopup(true);
    layout->addRow("Data e Ora Scadenza:", dueEdit);
    return box;
}

QWidget* ActivityEditor::createReminderPage() {
    QGroupBox* box = new QGroupBox("Dettagli Promemoria", this);
    QFormLayout* layout = new QFormLayout(box);
    reminderTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime().addSecs(1800), this);
    reminderTimeEdit->setCalendarPopup(true);
    frequencyCombo = new QComboBox(this);
    frequencyCombo->addItems({"Una tantum", "Giornaliero", "Settimanale", "Mensile"});
    notifTypeCombo = new QComboBox(this);
    notifTypeCombo->addItems({"Popup", "Audio", "Email"});
    snoozeSpin = new QSpinBox(this);
    snoozeSpin->setRange(0, 100);

    layout->addRow("Data e Ora Sveglia:", reminderTimeEdit);
    layout->addRow("Frequenza:", frequencyCombo);
    layout->addRow("Tipo Notifica:", notifTypeCombo);
    layout->addRow("Numero Rinvii:", snoozeSpin);
    return box;
}

QWidget* ActivityEditor::createEventPage() {
    QGroupBox* box = new QGroupBox("Dettagli Evento", this);
    QFormLayout* layout = new QFormLayout(box);
    eventDateEdit = new QDateEdit(QDate::currentDate(), this);
    eventDateEdit->setCalendarPopup(true);
    eventTypeEdit = new QLineEdit(this);
    organizerEdit = new QLineEdit(this);
    ticketSpin = new QDoubleSpinBox(this);
    ticketSpin->setRange(0.0, 9999.0);
    ticketSpin->setSuffix(" €");
    guestSpin = new QSpinBox(this);
    guestSpin->setRange(1, 10000);

    layout->addRow("Data Evento:", eventDateEdit);
    layout->addRow("Categoria:", eventTypeEdit);
    layout->addRow("Organizzatore:", organizerEdit);
    layout->addRow("Costo:", ticketSpin);
    layout->addRow("Invitati:", guestSpin);
    return box;
}

void ActivityEditor::prepareForNew() {
    editIndex = -1;
    typeCombo->setEnabled(true);
    typeCombo->setCurrentIndex(0);
    titleEdit->clear();
    descEdit->clear();
    completedCheck->setChecked(false);
    priorityCombo->setCurrentIndex(1);
    startEdit->setDateTime(QDateTime::currentDateTime());
    endEdit->setDateTime(QDateTime::currentDateTime().addSecs(3600));
    locationEdit->clear();
    personEdit->clear();
    confirmedCheck->setChecked(true);
    dueEdit->setDateTime(QDateTime::currentDateTime().addDays(1));
    reminderTimeEdit->setDateTime(QDateTime::currentDateTime().addSecs(1800));
    frequencyCombo->setCurrentIndex(0);
    notifTypeCombo->setCurrentIndex(0);
    snoozeSpin->setValue(0);
    eventDateEdit->setDate(QDate::currentDate());
    eventTypeEdit->clear();
    organizerEdit->clear();
    ticketSpin->setValue(0.0);
    guestSpin->setValue(1);
}

void ActivityEditor::prepareForEdit(const Activity* act, int index) {
    if (!act) return;
    editIndex = index;

    titleEdit->setText(QString::fromStdString(act->getTitle()));
    descEdit->setText(QString::fromStdString(act->getDescription()));
    completedCheck->setChecked(act->isCompleted());

    EditVisitor visitor(this);
    act->request(&visitor);

    typeCombo->setEnabled(false);
}

int ActivityEditor::getEditIndex() const {
    return editIndex;
}

Activity* ActivityEditor::createActivityFromForm() const {
    std::string title = titleEdit->text().toStdString();
    std::string desc = descEdit->toPlainText().toStdString();
    bool completed = completedCheck->isChecked();
    int type = typeCombo->currentIndex();

    switch (type) {
    case 0:
        return new Task(title, desc, completed, priorityCombo->currentText().toStdString());
    case 1:
        return new Appointment(title, desc, completed, startEdit->dateTime(), endEdit->dateTime(),
                               locationEdit->text().toStdString(), personEdit->text().toStdString(), confirmedCheck->isChecked());
    case 2:
        return new Deadline(title, desc, completed, dueEdit->dateTime());
    case 3:
        return new Reminder(title, desc, completed, reminderTimeEdit->dateTime(),
                            frequencyCombo->currentText().toStdString(), notifTypeCombo->currentText().toStdString(), snoozeSpin->value());
    case 4:
        return new Event(title, desc, completed, eventDateEdit->date(), eventTypeEdit->text().toStdString(),
                         organizerEdit->text().toStdString(), ticketSpin->value(), guestSpin->value());
    default:
        return nullptr;
    }
}