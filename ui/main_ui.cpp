#include "main_ui.h"
#include <nomogramms_viewer.h>
#include "nn_creator.h"
#include <QPushButton>

namespace ui
{
    MainUI::MainUI(QWidget* parent)
        : QMainWindow(parent)
        , ui(new Ui::NomogrammCalculatorClass())
    {
        ui->setupUi(this);
        connect(ui->allNomogrammsBtn, &QPushButton::clicked, this, &MainUI::onAllNomogrammsClicked);
        connect(ui->createNNBtn, &QPushButton::clicked, this, &MainUI::onCreateNNBtnClicked);
    }

    void MainUI::onAllNomogrammsClicked()
    {
        NomogrammsViewer viewer(this);
        viewer.exec();
    }

    void MainUI::onCreateNNBtnClicked()
    {
        DlgNNCreator dlg(this);
        dlg.exec();
    }
}