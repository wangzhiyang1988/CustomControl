#pragma once

#include <QtWidgets/QWidget>
#include "ui_TestUserControl.h"

class TestUserControl : public QWidget
{
    Q_OBJECT

public:
    TestUserControl(QWidget *parent = Q_NULLPTR);

private:
    Ui::TestUserControlClass ui;
};
