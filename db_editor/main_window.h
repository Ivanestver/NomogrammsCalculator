#pragma once

#include <QDialog>
#include "ui_main_window.h"
#include <memory>
#include "xml/xml.h"
#include <QMessageBox>
#include <QPoint>
#include <QMenu>
#include "db_objs_state.h"

class MainWindow : public QDialog
{
	Q_OBJECT;

	using FactoryMap = std::map<QUuid, std::shared_ptr<IStateCreator>>;

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow() override = default;

private:
	void initTree();
	void addToolBar();
	void addContextMenuToTree();
	QMessageBox::StandardButton showWarning(const QString& message);

private Q_SLOTS:
	void onAddItem();
	void onAddMethodology();
	void onRemoveItem();
	void onCustomMenuRequested(const QPoint& point);

private:
	Ui::MainWindow ui;
	FactoryMap factoryMap;
	FactoryMap factoryMapIndependent;
	std::map<QUuid, QUuid> mappingRules;
	QMenu* treeContextMenu = nullptr;
};