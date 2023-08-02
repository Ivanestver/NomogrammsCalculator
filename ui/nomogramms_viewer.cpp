#include "nomogramms_viewer.h"

NomogrammsViewer::NomogrammsViewer(QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::NomogrammViewer())
{
	ui->setupUi(this);
}