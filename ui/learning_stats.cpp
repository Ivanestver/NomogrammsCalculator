#include "learning_stats.h"

namespace ui
{
	LearningStatsDlg::LearningStatsDlg(const ml::LearningStatistics& stats, QWidget* parent)
		: QDialog(parent)
	{
		ui.setupUi(this);
		
		int rowNumber = 0;
		ui.statsWidget->setColumnCount(2);
		ui.statsWidget->insertRow(rowNumber);
		ui.statsWidget->setItem(rowNumber, 0, new QTableWidgetItem(QString::fromLocal8Bit("Время выполнения")));
		ui.statsWidget->setItem(rowNumber, 1, new QTableWidgetItem(QString::number(stats.executionTime.count())));
		
		ui.statsWidget->insertRow(++rowNumber);
		ui.statsWidget->setItem(rowNumber, 0, new QTableWidgetItem(QString::fromLocal8Bit("По эпохам")));
		for (size_t i = 0; i < stats.epochLosses.size(); ++i)
		{
			ui.statsWidget->insertRow(++rowNumber);
			ui.statsWidget->setItem(rowNumber, 0, new QTableWidgetItem(QString::fromLocal8Bit("Эпоха №%1").arg(i)));
			ui.statsWidget->setItem(rowNumber, 1, new QTableWidgetItem(QString::number(stats.epochLosses[i])));
		}
	}
}
