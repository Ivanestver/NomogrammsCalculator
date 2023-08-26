#pragma once

#include <QDialog>
#include "ui_main_window.h"
#include <memory>
#include "xml/xml.h"
#include <QMessageBox>
#include "db_objs_state.h"

class MainWindow : public QDialog
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow() override = default;

private:
	void initTree();
	void addToolBar();
	QMessageBox::StandardButton showWarning(const QString& message);

private Q_SLOTS:
	void onAddItem();
	void onAddMethodology();
	void onRemoveItem();

private:
	Ui::MainWindow ui;
	std::map<QUuid, std::shared_ptr<IStateCreator>> factoryMap;
	std::map<QUuid, QUuid> mappingRules;
};