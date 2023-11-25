#pragma once
#include "ui_view_nn.h"
#include <QDialog>
#include <QWidget>
#include "common_macros.h"

namespace m_ui
{
	class DlgViewNN : public QDialog
	{
		Q_OBJECT;
		DECLARE_NO_COPY_NO_MOVE(DlgViewNN);
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
		Ui::DlgViewNN m_ui;
	};
}