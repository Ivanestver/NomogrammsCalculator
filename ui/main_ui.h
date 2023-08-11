#pragma once

#include <QtWidgets/QMainWindow>
#include <QObject>
#include <memory>
#include "ui/ui_main_ui.h"

namespace ui
{
    class MainUI : public QMainWindow
    {
        Q_OBJECT

    public:
        MainUI(QWidget* parent = nullptr);
        ~MainUI() override = default;

    private:
        std::unique_ptr<Ui::NomogrammCalculatorClass> ui;

    private Q_SLOTS:
        void onAllNomogrammsClicked();
    };
}