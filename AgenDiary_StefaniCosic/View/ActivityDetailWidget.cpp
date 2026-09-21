#include "ActivityDetailWidget.h"
#include "DetailVisitor.h"
#include "IconVisitor.h"

ActivityDetailWidget::ActivityDetailWidget(QWidget* parent)
    : QWidget(parent) {
    setupUi();
}

void ActivityDetailWidget::setupUi() {
    setObjectName("detailContainer");
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(15);

    
    QHBoxLayout* headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(12);

    m_iconLabel = new QLabel(this);
    m_iconLabel->setFixedSize(48, 48);
    m_iconLabel->setScaledContents(true);

    QVBoxLayout* titleLayout = new QVBoxLayout();
    titleLayout->setSpacing(4);

    m_titleLabel = new QLabel("Seleziona un'attività", this);
    m_titleLabel->setObjectName("titleLabel");
    m_titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #3273dc;");
    m_titleLabel->setWordWrap(true);

    m_typeLabel = new QLabel(this);
    m_typeLabel->setObjectName("typeLabel");
    m_typeLabel->setStyleSheet("font-size: 13px; font-weight: 600; color: #64748B;");

    titleLayout->addWidget(m_titleLabel);
    titleLayout->addWidget(m_typeLabel);

    m_badgeLabel = new QLabel(this);
    m_badgeLabel->setObjectName("badgeLabel");
    m_badgeLabel->setAlignment(Qt::AlignCenter);
    m_badgeLabel->setContentsMargins(8, 4, 8, 4);
    m_badgeLabel->setVisible(false);

    headerLayout->addWidget(m_iconLabel);
    headerLayout->addLayout(titleLayout, 1);
    headerLayout->addWidget(m_badgeLabel);

    layout->addLayout(headerLayout);

    
    QLabel* descHeader = new QLabel("Descrizione:", this);
    descHeader->setStyleSheet("font-size: 16px; font-weight: bold; color: #ea570d; margin-top: 10px;");
    layout->addWidget(descHeader);

    m_descriptionLabel = new QLabel(this);
    m_descriptionLabel->setWordWrap(true);
    m_descriptionLabel->setStyleSheet("font-size:15px; color: #334155; line-height: 1.4;");
    layout->addWidget(m_descriptionLabel);

   
    QLabel* detailsHeader = new QLabel("Dettagli Specifici:", this);
    detailsHeader->setStyleSheet("font-size: 16px; font-weight: bold; color: #ea570d; margin-top: 10px;");
    layout->addWidget(detailsHeader);

    m_extraInfoLabel = new QLabel(this);
    m_extraInfoLabel->setStyleSheet("font-size: 15px; color: #1E293B; background: transparent; border: none; line-height: 1.5;");
    m_extraInfoLabel->setWordWrap(true);
    layout->addWidget(m_extraInfoLabel);

    layout->addStretch();

    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(10);

    m_toggleButton = new QPushButton("Completata", this);
    m_toggleButton->setStyleSheet("background-color: #10B981; color: white; padding: 8px 14px; font-weight: bold; border-radius: 4px;");

    m_editButton = new QPushButton(QIcon(":/Resources/edit.png"), "Modifica", this);
    m_editButton->setStyleSheet("background-color: #3B82F6; color: white; padding: 8px 14px; border-radius: 4px;");

    m_deleteButton = new QPushButton(QIcon(":/Resources/delete.png"), "Elimina", this);
    m_deleteButton->setStyleSheet("background-color: #EF4444; color: white; padding: 8px 14px; border-radius: 4px;");

    m_toggleButton->setEnabled(false);
    m_editButton->setEnabled(false);
    m_deleteButton->setEnabled(false);

    connect(m_toggleButton, &QPushButton::clicked, this, &ActivityDetailWidget::toggleCompletedRequested);
    connect(m_editButton, &QPushButton::clicked, this, &ActivityDetailWidget::editRequested);
    connect(m_deleteButton, &QPushButton::clicked, this, &ActivityDetailWidget::deleteRequested);

    btnLayout->addWidget(m_toggleButton);
    btnLayout->addWidget(m_editButton);
    btnLayout->addWidget(m_deleteButton);
    btnLayout->addStretch();

    layout->addLayout(btnLayout);
}

void ActivityDetailWidget::displayActivity(const Activity* activity) {
    if (!activity) {
        clearDisplay();
        return;
    }

    m_titleLabel->setText(QString::fromStdString(activity->getTitle()));

    IconVisitor iconVisitor;
    activity->request(&iconVisitor);
    m_iconLabel->setPixmap(iconVisitor.getIcon().pixmap(48, 48));
    m_typeLabel->setText(iconVisitor.getTypeName());

    
    QString desc = QString::fromStdString(activity->getDescription());
    m_descriptionLabel->setText(desc.isEmpty() ? "<i>Nessuna descrizione</i>" : desc);


    DetailVisitor detailVisitor;
    activity->request(&detailVisitor);
    m_extraInfoLabel->setText(detailVisitor.getResult());

    
    if (activity->isCompleted()) {
        m_badgeLabel->setText("COMPLETATA");
        m_badgeLabel->setStyleSheet("background-color: #D1FAE5; color: #065F46; font-weight: bold; border-radius: 12px; padding: 4px 10px; font-size: 11px;");
        m_badgeLabel->setVisible(true);
        m_toggleButton->setText("Segna come Da Fare");
    } else if (activity->isUrgent()) {
        m_badgeLabel->setText("URGENTE");
        m_badgeLabel->setStyleSheet("background-color: #FEE2E2; color: #991B1B; font-weight: bold; border-radius: 12px; padding: 4px 10px; font-size: 11px;");
        m_badgeLabel->setVisible(true);
        m_toggleButton->setText("Segna come Completata");
    } else {
        m_badgeLabel->setText("IN CORSO");
        m_badgeLabel->setStyleSheet("background-color: #E0E7FF; color: #3730A3; font-weight: bold; border-radius: 12px; padding: 4px 10px; font-size: 11px;");
        m_badgeLabel->setVisible(true);
        m_toggleButton->setText("Segna come Completata");
    }

    m_toggleButton->setEnabled(true);
    m_editButton->setEnabled(true);
    m_deleteButton->setEnabled(true);
}

void ActivityDetailWidget::clearDisplay() {
    m_iconLabel->clear();
    m_titleLabel->setText("Seleziona un'attività");
    m_typeLabel->clear();
    m_badgeLabel->setVisible(false);
    m_descriptionLabel->clear();
    m_extraInfoLabel->clear();

    m_toggleButton->setEnabled(false);
    m_editButton->setEnabled(false);
    m_deleteButton->setEnabled(false);
}
