#ifndef SCATTER3DWIDGET_H
#define SCATTER3DWIDGET_H

#include    <QMainWindow>
#include    <QtDataVisualization>

using namespace QtDataVisualization;


namespace Ui {
class Scatter3dWidget;
}

class Scatter3dWidget : public QMainWindow
{
    Q_OBJECT

private:
    QWidget     *graphContainer;//
    Q3DScatter  *graph3D;       //散点图
    QScatter3DSeries *series;  //散点序列

    void    iniGraph3D(); //初始化绘图
    QVector3D randVector();

public:
    explicit Scatter3dWidget(QWidget *parent = 0);
    ~Scatter3dWidget();

private slots:


private:
    Ui::Scatter3dWidget *ui;
};

#endif // SCATTER3DWIDGET_H
