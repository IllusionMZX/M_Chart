#ifndef BAR3DWIDGET_H
#define BAR3DWIDGET_H

#include    <QMainWindow>
#include    <QtDataVisualization>

using namespace QtDataVisualization;


namespace Ui {
class Bar3dWidget;
}

class Bar3dWidget : public QMainWindow
{
    Q_OBJECT

private:
    QWidget *graphContainer;//图表的容器
    Q3DBars *graph3D;   //图表
    QBar3DSeries *series;   //序列

    void    iniGraph3D();   //创建图表

public:
    explicit Bar3dWidget(QWidget *parent = 0);
    ~Bar3dWidget();

private slots:


private:
    Ui::Bar3dWidget *ui;
};

#endif // BAR3DWIDGET_H
