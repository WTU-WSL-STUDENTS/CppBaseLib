#ifndef CVDEVELOPWIDGET_H
#define CVDEVELOPWIDGET_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class CVDevelopWidget; }
QT_END_NAMESPACE

class CVDevelopWidget : public QMainWindow
{
    Q_OBJECT

public:
    CVDevelopWidget(QWidget *parent = nullptr);
    ~CVDevelopWidget();

private:
    Ui::CVDevelopWidget *ui;
};
#endif // CVDEVELOPWIDGET_H
