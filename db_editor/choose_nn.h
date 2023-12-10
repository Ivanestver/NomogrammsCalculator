#pragma once

#include <memory>
#include "ui_choose_nn.h"
#include "common_macros.h"
#include <QDialog>
#include <vector>
#include <QUuid>

class DlgChooseNN : public QDialog 
{
	Q_OBJECT;
	DECLARE_NO_COPY_NO_MOVE(DlgChooseNN);
public:
	DlgChooseNN(QWidget* parent = nullptr);
	~DlgChooseNN() override = default;

	const std::pair<QString, QUuid>& GetChosenItemInfo() const;

private Q_SLOTS:
	void onListItemClicked(int row);

private:
	void loadNets();

private:
	Ui::DlgChooseNN m_ui;
	std::vector<std::pair<QString, QUuid>> m_items;
	int chosenRow{ -1 };
};