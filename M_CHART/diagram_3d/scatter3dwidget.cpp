#include "scatter3dwidget.h"
#include "ui_scatter3dwidget.h"

#include    <QColorDialog>

void Scatter3dWidget::iniGraph3D()
{
    graph3D = new Q3DScatter();
    graphContainer = QWidget::createWindowContainer(graph3D); //Q3DBars继承自QWindow，必须如此创建

    QScatterDataProxy *proxy = new QScatterDataProxy(); //数据代理
    series = new QScatter3DSeries(proxy);  //创建序列
    series->setItemLabelFormat("(@xLabel @yLabel @zLabel)");
    series->setMeshSmooth(true);
    graph3D->addSeries(series);
    series->setBaseColor(0xe1e6e0);

//创建坐标轴
    graph3D->axisX()->setTitle("axis X");
    graph3D->axisX()->setTitleVisible(true);

    graph3D->axisY()->setTitle("axis Y");
    graph3D->axisY()->setTitleVisible(true);

    graph3D->axisZ()->setTitle("axis Z");
    graph3D->axisZ()->setTitleVisible(true);

    graph3D->activeTheme()->setLabelBackgroundEnabled(false);

    series->setMesh(QAbstract3DSeries::MeshSphere); //MeshPoint, MeshCylinder
    series->setItemSize(0.2);//default 0. value 0~1

    int N=41;
    int itemCount=N*N;
    QScatterDataArray *dataArray = new QScatterDataArray();
    dataArray->resize(itemCount);
    QScatterDataItem *ptrToDataArray = &dataArray->first();

//墨西哥草帽,-10:0.5:10， N=41
    float x,y,z;
    int i,j;

    x=-10;
    for (i=1 ; i <=N; i++)
    {
        y=-10;
        for ( j =1; j <=N; j++)
        {
            z=qSqrt(x*x+y*y);
            if (z!=0)
                z=10*qSin(z)/z;
            else
                z=10;

            ptrToDataArray->setPosition(QVector3D(x,z,y));
            ptrToDataArray++;
            y+=0.5;
        }
        x+=0.5;
    }

    series->dataProxy()->resetArray(dataArray);
}

QVector3D Scatter3dWidget::randVector()
{
    return QVector3D(
                (float)(rand() % 100) / 2.0f - (float)(rand() % 100) / 2.0f,
                (float)(rand() % 100) / 100.0f - (float)(rand() % 100) / 100.0f,
                (float)(rand() % 100) / 2.0f - (float)(rand() % 100) / 2.0f);
}

Scatter3dWidget::Scatter3dWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Scatter3dWidget)
{
    ui->setupUi(this);
    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);
    resize(1500,1000);
    iniGraph3D();

    this->setCentralWidget(graphContainer);
}

Scatter3dWidget::~Scatter3dWidget()
{
    delete ui;
}
