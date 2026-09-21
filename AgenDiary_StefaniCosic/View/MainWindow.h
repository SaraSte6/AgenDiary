#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListView>
#include <QLineEdit>
#include <QStackedWidget>
#include <QIcon>
#include <QColor>

#include "Controller/Controller.h"
#include "ActivityListModel.h"
#include "ActivityFilterProxyModel.h"
#include "SidebarWidget.h"
#include "ActivityDetailWidget.h"
#include "ActivityEditor.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow() override = default;

private slots:
    void onAddActivity();
    void onEditActivity();
    void onRemoveActivity();
    void onToggleCompleted();
    void onSave();
    void onLoad();
    void onSearch(const QString &text);
    void onFilterCategoryChanged(FilterCategory category);
    void onSelectionChanged(const QModelIndex &current, const QModelIndex &previous);

    void onEditorSave();
    void onEditorCancel();

private:
    void setupUi();
    QWidget* createDashboardWidget();
    QIcon tintedIcon(const QString& path, const QColor& color);

    Controller* m_controller;
    ActivityListModel* m_model;
    ActivityFilterProxyModel* m_proxyModel;

    QStackedWidget* m_stackedWidget;
    QWidget* m_dashboardWidget;
    ActivityEditor* m_editor;

    SidebarWidget* m_sidebar;
    QLineEdit* m_searchBar;
    QListView* m_listView;
    ActivityDetailWidget* m_detailWidget;
};

#endif // MAINWINDOW_H