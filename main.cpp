#include "ui/main_ui.h"
#include <QtWidgets/QApplication>
#include <QString>

#include "ml/AbstractNN.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ui::MainUI w;
    w.show();
    return a.exec();
}