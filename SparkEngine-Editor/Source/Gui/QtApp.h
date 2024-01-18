#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtApp.h"

class QtWidgetsApplication1 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication1(QWidget *parent = nullptr);
    ~QtWidgetsApplication1();

private:
    Ui::QtWidgetsApplication1Class ui;
};
