#ifndef SURFACE3DWIDGET_H
#define SURFACE3DWIDGET_H

#include    <QMainWindow>
#include    <QtDataVisualization>

using namespace QtDataVisualization;


namespace Ui {
class Surface3dWidget;
}

class Surface3dWidget : public QMainWindow
{
    Q_OBJECT

private:
    QWidget         *graphContainer;//
    Q3DSurface      *graph3D;   //三维图表
    QSurface3DSeries    *series;    //序列
    QSurfaceDataProxy   *proxy;     //数据代理

    void    iniGraph3D();
public:
    explicit Surface3dWidget(QWidget *parent = 0);
    ~Surface3dWidget();

private slots:

private:
    Ui::Surface3dWidget *ui;
};

#endif // MAINWINDOW_H
