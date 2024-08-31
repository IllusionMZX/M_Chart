#include "surface3dwidget.h"
#include "ui_surface3dwidget.h"

#include    <QColorDialog>

void Surface3dWidget::iniGraph3D()
{
    graph3D = new Q3DSurface();
    graphContainer = QWidget::createWindowContainer(graph3D); //Q3DBars继承自QWindow，必须如此创建
//创建坐标轴
    QValue3DAxis *axisX=new QValue3DAxis;
    axisX->setTitle("Axis X");
    axisX->setTitleVisible(true);
    axisX->setRange(-11,11);
//    axisX->setAutoAdjustRange(true);
    graph3D->setAxisX(axisX);

    QValue3DAxis *axisY=new QValue3DAxis;
    axisY->setTitle("Axis Y");
    axisY->setTitleVisible(true);
//    axisY->setRange(-10,10);
    axisY->setAutoAdjustRange(true);
    graph3D->setAxisY(axisY);

    QValue3DAxis *axisZ=new QValue3DAxis;
    axisZ->setTitle("Axis Z");
    axisZ->setTitleVisible(true);
    axisZ->setRange(-11,11);
//    axisZ->setAutoAdjustRange(true);
    graph3D->setAxisZ(axisZ);

//创建数据代理
    proxy = new QSurfaceDataProxy();
    series = new QSurface3DSeries(proxy);
    series->setItemLabelFormat("(@xLabel @yLabel @zLabel)");
    series->setMeshSmooth(true);
    graph3D->activeTheme()->setLabelBackgroundEnabled(false);

    series->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    graph3D->addSeries(series);
    series->setBaseColor(0xf1ece6);

//创建数据, 墨西哥草帽
    int N=41;//-10:0.5:10, N个数据点
    QSurfaceDataArray *dataArray = new QSurfaceDataArray; //数组
    dataArray->reserve(N);

    float x=-10,y,z;
    int i,j;
    for (i =1 ; i <=N; i++)
    {
        QSurfaceDataRow *newRow = new QSurfaceDataRow(N); //一行的数据
        y=-10;
        int index=0;
        for (j = 1; j <=N; j++)
        {
            z=qSqrt(x*x+y*y);
            if (z!=0)
                z=10*qSin(z)/z;
            else
                z=10;
            (*newRow)[index++].setPosition(QVector3D(x, z, y));
            y=y+0.5;
        }
        x=x+0.5;
        *dataArray << newRow;
    }

    proxy->resetArray(dataArray);

}

Surface3dWidget::Surface3dWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Surface3dWidget)
{
    ui->setupUi(this);
    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);
    resize(1500,1000);
    iniGraph3D();
    this->setCentralWidget(graphContainer);
}

Surface3dWidget::~Surface3dWidget()
{
    delete ui;
}

