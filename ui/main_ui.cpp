#include "main_ui.h"
#include <nomogramms_viewer.h>
#include <QPushButton>

namespace ui
{
    MainUI::MainUI(QWidget* parent)
        : QMainWindow(parent)
        , ui(new Ui::NomogrammCalculatorClass())
    {
        ui->setupUi(this);
        connect(ui->allNomogrammsBtn, &QPushButton::clicked, this, &MainUI::onAllNomogrammsClicked);
    }

    void MainUI::onAllNomogrammsClicked()
    {
        NomogrammsViewer viewer(this);
        viewer.exec();
    }
}