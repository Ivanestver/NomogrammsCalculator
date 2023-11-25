#pragma once

#include <memory>
#include "ui_choose_nn.h"
#include "common_macros.h"
#include <QDialog>
#include <vector>

class DlgChooseNN : public QDialog 
{
	Q_OBJECT;
	DECLARE_NO_COPY_NO_MOVE(DlgChooseNN);
public:
	DlgChooseNN(QWidget* parent = nullptr);
	~DlgChooseNN() override = default;

	const QString& GetChosenNNName() const;

private Q_SLOTS:
	void onListItemClicked(const QString& currentText);

private:
	void loadNets();

private:
	Ui::DlgChooseNN m_ui;
	QString m_chosenNNName;
};