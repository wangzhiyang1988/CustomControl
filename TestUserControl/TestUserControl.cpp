#include "TestUserControl.h"
#include "./eprlineedit/eprlineedit.h"

TestUserControl::TestUserControl(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    //lineedit 自定义控件
    ui.lineEdit_CustomLineEdit->setValidator(new Epr100DoubleValidator(0, 60, 1, this));
}
