#pragma once

#include <QDialog>
#include <QObject>
#include <QWidget>
#include "ui/ui_learning_stats.h"
#include "ml/NNCouch.h"

namespace ui
{
	class LearningStatsDlg : public QDialog
	{
	public:
		LearningStatsDlg(const ml::LearningStatistics& stats, QWidget* parent = nullptr);
		~LearningStatsDlg() override = default;

	private:
		QTableWidgetItem* createItem(const QString& text) const noexcept;
		void showStat(const std::vector<double>& values, const QString& title, const QString& rowName) noexcept;

	private:
		Ui::LearningStatsDlg ui;
	};
}