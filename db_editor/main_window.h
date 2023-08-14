#pragma once

#include <QDialog>
#include "ui_main_window.h"
#include <memory>
#include "xml/xml.h"
#include <QMessageBox>

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

private:
	Ui::MainWindow ui;
};