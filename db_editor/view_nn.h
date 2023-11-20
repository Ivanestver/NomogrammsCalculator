#pragma once
#include "ui_view_nn.h"
#include <QDialog>
#include <QWidget>

namespace ui
{
	class DlgViewNN : public QDialog
	{
		Q_OBJECT;
	public:
		DlgViewNN(QWidget* parent = nullptr);
		~DlgViewNN() override = default;

	private Q_SLOTS:
		void onCurrentNNNameChanged(QListWidgetItem* current);
		void onTestBtnClicked();
		void onShowContextMenu(const QPoint& point);
		void onRemoveContextMenuItemClicked();

	private:
		void fillNNList();
		void fillByNetInfo();

	private:
		Ui::DlgViewNN ui;
	};
}