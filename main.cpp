#include "NomogrammCalculator.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NomogrammCalculator w;
    w.show();
    return a.exec();
}