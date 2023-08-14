#pragma once

#include <QDialog>
#include "ui_main_window.h"
#include <memory>
#include "xml/xml.h"

class MainWindow : public QDialog
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow() override = default;

private:
	void initTree();
	void addToolBar();

private Q_SLOTS:
	void onAddItem();

private:
	Ui::MainWindow ui;
};