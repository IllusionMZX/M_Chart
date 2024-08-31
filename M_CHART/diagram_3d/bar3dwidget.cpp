#include "bar3dwidget.h"
#include "ui_bar3dwidget.h"

#include    <QColorDialog>

void Bar3dWidget::iniGraph3D()
{
    graph3D = new Q3DBars();
    graphContainer = QWidget::createWindowContainer(graph3D); //Q3DBars继承自QWindow
    graph3D->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFrontHigh); //设置视角

//创建坐标轴
    QStringList rowLabs;  //
    QStringList colLabs;

    rowLabs << "第一行" << "第二行"<<"第三行";
    colLabs << "第一列" << "第二列" << "第三列" << "第四列"<<"第五列";

    QValue3DAxis *axisV=new QValue3DAxis; //数值坐标
    axisV->setTitle("数值坐标");
    axisV->setTitleVisible(true);

    QCategory3DAxis *axisRow=new QCategory3DAxis;
    axisRow->setTitle("行坐标");
    axisRow->setLabels(rowLabs);
    axisRow->setTitleVisible(true);

    QCategory3DAxis *axisCol=new QCategory3DAxis;
    axisCol->setTitle("列坐标");
    axisCol->setLabels(colLabs);
    axisCol->setTitleVisible(true);


    graph3D->setValueAxis(axisV);
    graph3D->setRowAxis(axisRow);
    graph3D->setColumnAxis(axisCol);

//创建序列
    series = new QBar3DSeries;
    series->setMesh(QAbstract3DSeries::MeshCylinder);
    series->setItemLabelFormat("(@rowLabel,@colLabel): %.1f"); //项的标签显示格式
    graph3D->addSeries(series);

    QBarDataArray *dataSet = new QBarDataArray; //数据代理
    dataSet->reserve(rowLabs.count());

    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    QBarDataRow *dataRow = new QBarDataRow;
    *dataRow << qrand()%5 << qrand()%5<< qrand()%5<<qrand()%5<<qrand()%5; //第1行数据，有5列
    dataSet->append(dataRow);

    QBarDataRow *dataRow2 = new QBarDataRow;
    *dataRow2 <<qrand()%5<< qrand()%5<< qrand()%5<< qrand()%5<<qrand()%5; //第2行数据，有5列
    dataSet->append(dataRow2);

    QBarDataRow *dataRow3 = new QBarDataRow;
    *dataRow3 <<qrand()%10<< qrand()%10<< qrand()%10<< qrand()%10<<qrand()%10; //第2行数据，有5列
    dataSet->append(dataRow3);

    series->dataProxy()->resetArray(dataSet);
}

Bar3dWidget::Bar3dWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Bar3dWidget)
{
    ui->setupUi(this);
    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);
    resize(1500,1000);
    iniGraph3D(); //创建图表

    this->setCentralWidget(graphContainer); //设置主窗口中心组建
}

Bar3dWidget::~Bar3dWidget()
{
    delete ui;
}
