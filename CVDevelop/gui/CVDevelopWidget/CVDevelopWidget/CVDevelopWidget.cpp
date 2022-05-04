#include "CVDevelopWidget.h"
#include "./ui_CVDevelopWidget.h"

CVDevelopWidget::CVDevelopWidget(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CVDevelopWidget)
{
    ui->setupUi(this);
}

CVDevelopWidget::~CVDevelopWidget()
{
    delete ui;
}

