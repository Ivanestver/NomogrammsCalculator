#include "choose_nomogramm_picture_dlg.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsScene>
#include "db_executor.h"

ChooseNomogrammPictureDlg::ChooseNomogrammPictureDlg(const QUuid& nomogrammId_, QWidget* parent)
	: QDialog(parent)
	, nomogrammId(nomogrammId_)
{
	ui.setupUi(this);
	ui.currentPicture->setScene(new QGraphicsScene());
	ui.newPicture->setScene(new QGraphicsScene());

	showCurrentPicture();

	connect(ui.choosePictureBtn, &QPushButton::clicked, this, &ChooseNomogrammPictureDlg::onChoosePictureBtnClicked);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &ChooseNomogrammPictureDlg::onAcceptBtnClicked);
}

void ChooseNomogrammPictureDlg::showCurrentPicture()
{
	auto db = DBExecutor::GetInstance();
	if (!db)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Нет подключения к БД"));
		return;
	}

	QString error;
	QImage image = db->LoadPicture(nomogrammId, error);
	if (image.isNull())
	{
		if (error.isEmpty())
			return;

		QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка"), error);
		return;
	}

	ui.currentPicture->scene()->addItem(new QGraphicsPixmapItem(QPixmap::fromImage(image)));
}

void ChooseNomogrammPictureDlg::onAcceptBtnClicked()
{
	auto db = DBExecutor::GetInstance();
	if (!db)
	{
		QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка"), QString::fromLocal8Bit("Отсутствует подключение к БД"));
		return;
	}

	QString error;
	if (!db->SavePicture(nomogrammId, { chosenImage, pictureFormat }, error))
	{
		QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка"), error);
	}
}

void ChooseNomogrammPictureDlg::onChoosePictureBtnClicked()
{
	auto filePath = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("Выбрать изображение"), QString("./"), QString("*.png, *.jpg"), nullptr, QFileDialog::DontUseNativeDialog);
	if (filePath.isEmpty())
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Необходимо выбрать файл"));
		return;
	}

	chosenImage = QImage(filePath);

	ui.newPicture->scene()->removeItem(item);
	item = new QGraphicsPixmapItem();
	item->setPixmap(QPixmap::fromImage(chosenImage));

	ui.newPicture->scene()->addItem(item);

	auto fileParts = filePath.split('/');
	auto fileName = fileParts[fileParts.size() - 1];
	ui.pictureName->setText(fileName);

	pictureFormat = fileName.split(".")[1];
}
