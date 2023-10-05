#pragma once
#include "ui/ui_view_nn.h"
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

	private:
		void fillNNList();

	private:
		Ui::DlgViewNN ui;
	};
}