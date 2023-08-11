#pragma once

#include <QtWidgets/QDialog>
#include <QObject>
#include <QWidget>
#include <memory>
#include "ui/ui_nomogramms_viewer.h"
#include "db/DataBaseWrapper.h"

namespace ui
{
	class NomogrammsViewer : public QDialog
	{
		Q_OBJECT;

	public:
		NomogrammsViewer(QWidget* parent = nullptr);
		~NomogrammsViewer() = default;

	private:
		void setTree();

	private:
		std::shared_ptr<Ui::NomogrammViewer> ui;
	};
}