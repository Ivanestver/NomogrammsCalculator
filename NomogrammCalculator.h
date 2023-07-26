#pragma once

#include <QtWidgets/QMainWindow>
#include <QObject>
#include "ui_NomogrammCalculator.h"

class NomogrammCalculator : public QMainWindow
{
    Q_OBJECT

public:
    NomogrammCalculator(QWidget *parent = nullptr);
    ~NomogrammCalculator() override = default;

private:
    std::unique_ptr<Ui::NomogrammCalculatorClass> ui;
};