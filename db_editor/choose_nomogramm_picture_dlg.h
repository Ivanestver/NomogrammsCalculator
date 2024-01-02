#pragma once

#include <QDialog>
#include <QUuid>
#include <QString>
#include <QWidget>
#include "ui_choose_nomogramm_picture_dlg.h"
#include <QGraphicsPixmapItem>

class ChooseNomogrammPictureDlg : public QDialog
{
public:
	explicit ChooseNomogrammPictureDlg(const QUuid& nomogrammId_, QWidget* parent = nullptr);

private:
	void showCurrentPicture();

private Q_SLOTS:
	void onChoosePictureBtnClicked();
	void onAcceptBtnClicked();

private:
	Ui::ChooseNomogrammPictureDlg ui;
	QImage chosenImage;
	QGraphicsPixmapItem* item{ nullptr };
	QUuid nomogrammId;
	QString pictureFormat;
};