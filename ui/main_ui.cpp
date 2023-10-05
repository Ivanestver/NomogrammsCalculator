#include "main_ui.h"
#include <nomogramms_viewer.h>
#include "nn_creator.h"
#include "view_nn.h"
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
        connect(ui->viewNNBtn, &QPushButton::clicked, this, &MainUI::onViewNNBtnClicked);
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

    void MainUI::onViewNNBtnClicked()
    {
        DlgViewNN dlg(this);
        dlg.exec();
    }
}