#include "SidebarWidget.h"

SidebarWidget::SidebarWidget(QWidget* parent)
    : QWidget(parent) {
    setupUi();
}

void SidebarWidget::setupUi() {
    setFixedWidth(200);
    setStyleSheet("background-color: #1E293B; border-right: 1px solid #334155;");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(12, 20, 12, 20);
    layout->setSpacing(8);

    QLabel* brandLabel = new QLabel("AgenDiary", this);
    brandLabel->setStyleSheet("color: #f56213; font-size: 22px; font-weight: bold; background: transparent; border: none; margin-bottom: 15px;");
    layout->addWidget(brandLabel);

    QLabel* sectionLabel = new QLabel("CATEGORIE", this);
    sectionLabel->setStyleSheet("color: #05509c; font-size: 12px; font-weight: bold; background: transparent; border: none; margin-bottom: 5px;");
    layout->addWidget(sectionLabel);

    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->setExclusive(true);

    auto createFilterBtn = [this](const QString& text, int id) -> QPushButton* {
        QPushButton* btn = new QPushButton(text, this);
        btn->setCheckable(true);
        btn->setStyleSheet(
            "QPushButton {"
            "   color: #334155;"
            "   background-color: transparent;"
            "   border: none;"
            "   border-radius: 6px;"
            "   text-align: left;"
            "   padding: 10px 14px;"
            "   font-size: 14px;"
            "   font-weight: 600;"
            "}"
            "QPushButton:hover {"
            "   background-color: #e4e7ea;"
            "   color: #0F172A;"
            "}"
            "QPushButton:checked {"
            "   background-color: #276afa;"
            "   color: #FFFFFF;"
            "   font-weight: bold;"
            "}"
            );
        m_buttonGroup->addButton(btn, id);
        return btn;
    };

    m_allBtn = createFilterBtn("Tutte le Attività", static_cast<int>(FilterCategory::All));
    m_urgentBtn = createFilterBtn("Urgenti", static_cast<int>(FilterCategory::Urgent));
    m_pendingBtn = createFilterBtn("Da Fare", static_cast<int>(FilterCategory::Pending));
    m_completedBtn = createFilterBtn("Completate", static_cast<int>(FilterCategory::Completed));

    m_allBtn->setChecked(true);

    layout->addWidget(m_allBtn);
    layout->addWidget(m_urgentBtn);
    layout->addWidget(m_pendingBtn);
    layout->addWidget(m_completedBtn);

    layout->addStretch();

    connect(m_buttonGroup, &QButtonGroup::idClicked, this, [this](int id) {
        emit filterChanged(static_cast<FilterCategory>(id));
    });
}

FilterCategory SidebarWidget::getCurrentFilter() const {
    return static_cast<FilterCategory>(m_buttonGroup->checkedId());
}
