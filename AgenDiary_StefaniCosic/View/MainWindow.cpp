#include "MainWindow.h"
#include <QToolBar>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QPainter>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_controller(new Controller(this)) {
    m_model = new ActivityListModel(m_controller, this);
    m_proxyModel = new ActivityFilterProxyModel(m_controller, this);
    m_proxyModel->setSourceModel(m_model);

    setupUi();

    QFile styleFile(":/Resources/style.qss");
    if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        this->setStyleSheet(styleFile.readAll());
        styleFile.close();
    }
}

QIcon MainWindow::tintedIcon(const QString& path, const QColor& color) {
    QPixmap pixmap(path);
    if (pixmap.isNull()) return QIcon(path);

    QPainter painter(&pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(pixmap.rect(), color);
    painter.end();

    return QIcon(pixmap);
}

void MainWindow::setupUi() {
    QToolBar* toolbar = addToolBar("Barra Strumenti");
    toolbar->setIconSize(QSize(22, 22));
    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolbar->setMovable(false);

    QColor whiteTint("white");
    toolbar->addAction(tintedIcon(":/Resources/new_item.png", whiteTint), "Nuova Attività", this, &MainWindow::onAddActivity);
    toolbar->addAction(tintedIcon(":/Resources/save_file.png", whiteTint), "Salva", this, &MainWindow::onSave);
    toolbar->addAction(tintedIcon(":/Resources/open_file.png", whiteTint), "Carica", this, &MainWindow::onLoad);

    m_stackedWidget = new QStackedWidget(this);
    m_dashboardWidget = createDashboardWidget();
    m_editor = new ActivityEditor(this);

    m_stackedWidget->addWidget(m_dashboardWidget); 
    m_stackedWidget->addWidget(m_editor);          

    setCentralWidget(m_stackedWidget);

    connect(m_editor, &ActivityEditor::saveRequested, this, &MainWindow::onEditorSave);
    connect(m_editor, &ActivityEditor::cancelRequested, this, &MainWindow::onEditorCancel);

    setMinimumSize(950, 680);
    setWindowTitle("AgenDiary - Gestione Attività");
    setWindowIcon(QIcon(":/Resources/logo.png"));
}

QWidget* MainWindow::createDashboardWidget() {
    QWidget* dashboard = new QWidget(this);
    QHBoxLayout* dashLayout = new QHBoxLayout(dashboard);
    dashLayout->setContentsMargins(0, 0, 0, 0);
    dashLayout->setSpacing(0);

    m_sidebar = new SidebarWidget(dashboard);
    connect(m_sidebar, &SidebarWidget::filterChanged, this, &MainWindow::onFilterCategoryChanged);
    dashLayout->addWidget(m_sidebar);

    QWidget* centerContainer = new QWidget(dashboard);
    QVBoxLayout* centerLayout = new QVBoxLayout(centerContainer);
    centerLayout->setContentsMargins(15, 20, 15, 20);
    centerLayout->setSpacing(12);

    m_searchBar = new QLineEdit(centerContainer);
    m_searchBar->setPlaceholderText("Cerca per titolo o descrizione...");
    m_searchBar->setStyleSheet("padding: 8px 12px; border: 1px solid #CBD5E1; border-radius: 6px; font-size: 13px;");
    connect(m_searchBar, &QLineEdit::textChanged, this, &MainWindow::onSearch);

    m_listView = new QListView(centerContainer);
    m_listView->setModel(m_proxyModel);
    m_listView->setIconSize(QSize(28, 28));
    m_listView->setStyleSheet("border: 1px solid #E2E8F0; border-radius: 6px; font-size: 13px;");
    connect(m_listView->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWindow::onSelectionChanged);

    centerLayout->addWidget(m_searchBar);
    centerLayout->addWidget(m_listView);

    m_detailWidget = new ActivityDetailWidget(dashboard);
    connect(m_detailWidget, &ActivityDetailWidget::toggleCompletedRequested, this, &MainWindow::onToggleCompleted);
    connect(m_detailWidget, &ActivityDetailWidget::editRequested, this, &MainWindow::onEditActivity);
    connect(m_detailWidget, &ActivityDetailWidget::deleteRequested, this, &MainWindow::onRemoveActivity);

    QSplitter* splitter = new QSplitter(dashboard);
    splitter->addWidget(centerContainer);
    splitter->addWidget(m_detailWidget);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);
    splitter->setCollapsible(0, false);
    splitter->setCollapsible(1, false);

    dashLayout->addWidget(splitter, 1);
    return dashboard;
}

void MainWindow::onSelectionChanged(const QModelIndex &current, const QModelIndex &previous) {
    Q_UNUSED(previous);
    if (!current.isValid()) {
        m_detailWidget->clearDisplay();
        return;
    }

    QModelIndex sourceIndex = m_proxyModel->mapToSource(current);
    const std::vector<Activity*>& activities = m_controller->getActivities();
    if (sourceIndex.row() >= 0 && sourceIndex.row() < static_cast<int>(activities.size())) {
        Activity* activity = activities[sourceIndex.row()];
        if (activity) {
            m_detailWidget->displayActivity(activity);
        }
    }
}

void MainWindow::onFilterCategoryChanged(FilterCategory category) {
    m_proxyModel->setFilterCategory(category);
    m_detailWidget->clearDisplay();
}

void MainWindow::onSearch(const QString &text) {
    m_proxyModel->setFilterRegularExpression(QRegularExpression(text, QRegularExpression::CaseInsensitiveOption));
}

void MainWindow::onAddActivity() {
    m_editor->prepareForNew();
    m_stackedWidget->setCurrentWidget(m_editor);
}

void MainWindow::onEditActivity() {
    QModelIndex index = m_listView->currentIndex();
    if (!index.isValid()) return;

    QModelIndex sourceIndex = m_proxyModel->mapToSource(index);
    const std::vector<Activity*>& activities = m_controller->getActivities();

    if (sourceIndex.row() < 0 || sourceIndex.row() >= static_cast<int>(activities.size())) return;
    Activity* activity = activities[sourceIndex.row()];
    if (!activity) return;

    m_editor->prepareForEdit(activity, sourceIndex.row());
    m_stackedWidget->setCurrentWidget(m_editor);
}

void MainWindow::onRemoveActivity() {
    QModelIndex index = m_listView->currentIndex();
    if (!index.isValid()) return;

    int ret = QMessageBox::question(this, "Conferma Eliminazione",
                                    "Sei sicuro di voler eliminare questa attività?",
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        QModelIndex sourceIndex = m_proxyModel->mapToSource(index);
        m_controller->removeActivityAt(sourceIndex.row());
        m_detailWidget->clearDisplay();
    }
}

void MainWindow::onToggleCompleted() {
    QModelIndex index = m_listView->currentIndex();
    if (!index.isValid()) return;

    QModelIndex sourceIndex = m_proxyModel->mapToSource(index);
    m_controller->toggleActivityCompleted(sourceIndex.row());

    const std::vector<Activity*>& activities = m_controller->getActivities();
    if (sourceIndex.row() >= 0 && sourceIndex.row() < static_cast<int>(activities.size())) {
        m_detailWidget->displayActivity(activities[sourceIndex.row()]);
    }
}

void MainWindow::onEditorSave() {
    Activity* act = m_editor->createActivityFromForm();
    if (!act) {
        QMessageBox::warning(this, "Errore", "Impossibile creare l'attività dai dati del form.");
        return;
    }

    if (!act->isValid()) {
        QMessageBox::warning(this, "Dati Non Validi",
                             "I dati inseriti non sono validi.\n"
                             "Verifica che il titolo non sia vuoto e che date/orari siano corretti.");
        delete act;
        return;
    }

    int editIndex = m_editor->getEditIndex();
    if (editIndex == -1) {
        m_controller->addActivity(act);
    } else {
        m_controller->updateActivity(editIndex, act);
        m_model->refresh();
        onSelectionChanged(m_listView->currentIndex(), QModelIndex());
    }

    m_stackedWidget->setCurrentWidget(m_dashboardWidget);
}

void MainWindow::onEditorCancel() {
    m_stackedWidget->setCurrentWidget(m_dashboardWidget);
}

void MainWindow::onSave() {
    QString filename = QFileDialog::getSaveFileName(this, "Salva Attività", "", "File JSON (*.json);;File XML (*.xml)");
    if (!filename.isEmpty()) {
        bool ok = m_controller->saveToFile(filename);
        if (ok) {
            QMessageBox::information(this, "Salvataggio Completato", "Attività salvate con successo.");
        } else {
            QMessageBox::critical(this, "Errore Salvataggio", "Impossibile salvare il file specificato.");
        }
    }
}

void MainWindow::onLoad() {
    QString filename = QFileDialog::getOpenFileName(this, "Carica Attività", "", "File JSON (*.json);;File XML (*.xml)");
    if (!filename.isEmpty()) {
        bool ok = m_controller->loadFromFile(filename);
        if (ok) {
            m_detailWidget->clearDisplay();
            QMessageBox::information(this, "Caricamento Completato", "Attività caricate con successo.");
        } else {
            QMessageBox::critical(this, "Errore Caricamento", "Impossibile caricare il file selezionato.");
        }
    }
}