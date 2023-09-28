#include "learning_stats.h"

namespace ui
{
	LearningStatsDlg::LearningStatsDlg(const ml::LearningStatistics& stats, QWidget* parent)
		: QDialog(parent)
	{
		ui.setupUi(this);
		ui.statsWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

		int rowNumber = 0;
		ui.statsWidget->setColumnCount(2);
		ui.statsWidget->insertRow(rowNumber);
		ui.statsWidget->setItem(rowNumber, 0, createItem(QString::fromLocal8Bit("Время выполнения")));
		ui.statsWidget->setItem(rowNumber, 1, createItem(QString::number(stats.executionTime.count())));

		showStat(stats.epochLosses, QString::fromLocal8Bit("По эпохам"), QString::fromLocal8Bit("Эпоха №%1"));
		showStat(stats.decadeLosses, QString::fromLocal8Bit("По декадам"), QString::fromLocal8Bit("Декада №%1"));
	}

	QTableWidgetItem* LearningStatsDlg::createItem(const QString& text) const noexcept
	{
		auto* item = new QTableWidgetItem(text);
		item->setTextAlignment(Qt::AlignCenter);
		item->setFlags(item->flags() ^ Qt::ItemIsEditable);
		return item;
	}

	void LearningStatsDlg::showStat(const std::vector<double>& values, const QString& title, const QString& rowName) noexcept
	{
		if (values.empty())
			return;

		int rowNumber = ui.statsWidget->columnCount();
		rowNumber--;
		ui.statsWidget->insertRow(rowNumber);
		ui.statsWidget->setItem(rowNumber, 0, createItem(title));
		ui.statsWidget->setSpan(rowNumber, 0, 1, 2);
		for (size_t i = 0; i < values.size(); ++i)
		{
			ui.statsWidget->insertRow(++rowNumber);
			ui.statsWidget->setItem(rowNumber, 0, createItem(rowName.arg(i + 1)));
			ui.statsWidget->setItem(rowNumber, 1, createItem(QString::number(values[i])));
		}
	}
}
