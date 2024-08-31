#include "scatterchartwidget.h"
#include "pentool.h"
#include "brushtool.h"
#include "customslice.h"

#include <QtCharts/QScatterSeries>
#include <QtCharts/QLegendMarker>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtCore/QtMath>

ScatterChartWidget::ScatterChartWidget(QWidget *parent) : QWidget(parent),m_slice(0)
{
    this->setMouseTracking(true);
    // create chart
   prepareData();

   setPalette(Qt::white);
   setAutoFillBackground(true);

    // chart settings
    m_themeComboBox = new QComboBox();
    m_themeComboBox->addItem("Light", QChart::ChartThemeLight);
    m_themeComboBox->addItem("BlueCerulean", QChart::ChartThemeBlueCerulean);
    m_themeComboBox->addItem("Dark", QChart::ChartThemeDark);
    m_themeComboBox->addItem("BrownSand", QChart::ChartThemeBrownSand);
    m_themeComboBox->addItem("BlueNcs", QChart::ChartThemeBlueNcs);
    m_themeComboBox->addItem("High Contrast", QChart::ChartThemeHighContrast);
    m_themeComboBox->addItem("Blue Icy", QChart::ChartThemeBlueIcy);
    m_themeComboBox->addItem("Qt", QChart::ChartThemeQt);

    m_aaCheckBox = new QCheckBox();
    m_animationsCheckBox = new QCheckBox();
    m_animationsCheckBox->setCheckState(Qt::Checked);
    m_legendCheckBox = new QCheckBox();

    QFormLayout *chartSettingsLayout = new QFormLayout();
    chartSettingsLayout->addRow("主题", m_themeComboBox);
    chartSettingsLayout->addRow("抗锯齿", m_aaCheckBox);
    chartSettingsLayout->addRow("动画", m_animationsCheckBox);
    chartSettingsLayout->addRow("图例", m_legendCheckBox);
    QGroupBox *chartSettings = new QGroupBox("Chart");
    chartSettings->setLayout(chartSettingsLayout);

    connect(m_themeComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &ScatterChartWidget::updateChartSettings);
    connect(m_aaCheckBox, &QCheckBox::toggled, this, &ScatterChartWidget::updateChartSettings);
    connect(m_animationsCheckBox, &QCheckBox::toggled, this, &ScatterChartWidget::updateChartSettings);
    connect(m_legendCheckBox, &QCheckBox::toggled, this, &ScatterChartWidget::updateChartSettings);

    // series settings


    m_AxissizeFactor = new QDoubleSpinBox();
    m_AxissizeFactor->setMinimum(1.0);
    m_AxissizeFactor->setValue(1.0);
    m_AxissizeFactor->setSingleStep(0.1);
    connect(m_AxissizeFactor, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ScatterChartWidget::updateChartSettings);

    QPushButton *removeline = new QPushButton("删除散点");
    QFormLayout *seriesSettingsLayout = new QFormLayout();
    seriesSettingsLayout->addRow("缩放", m_AxissizeFactor);
    seriesSettingsLayout->addRow(removeline);
    QGroupBox *seriesSettings = new QGroupBox("Series");
    seriesSettings->setLayout(seriesSettingsLayout);
    connect(removeline, &QPushButton::clicked, this, &ScatterChartWidget::remove);



    statusBar=new QStatusBar();
    statusBar->setPalette(QPalette(Qt::white));
    lab_chartXY = new QLabel("Chart X=,  Y=  ");
    lab_chartXY->setMinimumWidth(200);
    statusBar->addWidget(lab_chartXY);
    lab_hoverXY = new QLabel("Hovered X=,  Y=  ");
    lab_hoverXY->setMinimumWidth(200);
    statusBar->addWidget(lab_hoverXY);
    lab_clickXY = new QLabel("Clicked X=,  Y=  ");
    lab_clickXY->setMinimumWidth(200);
    statusBar->addWidget(lab_clickXY);



    QVBoxLayout *settingsLayout = new QVBoxLayout();
    settingsLayout->addWidget(chartSettings);
    settingsLayout->addWidget(seriesSettings);
    settingsLayout->addStretch();





    m_download=new QToolButton();
    m_download->setIconSize(QSize(30,30));
    m_download->setIcon(QIcon(":/images/image/download.png"));
    connect(m_download,SIGNAL(clicked()),this,SLOT(on_download_clicked()));

    QGridLayout *baseLayout = new QGridLayout();
    baseLayout->addLayout(settingsLayout, 0, 0);

    baseLayout->addWidget(m_chartView, 0, 1);
    baseLayout->addWidget(m_download,0,2);
    baseLayout->addWidget(statusBar,1,1);
    setLayout(baseLayout);

    updateSerieSettings();
    updateChartSettings();
}
//数据准备含 connect
void ScatterChartWidget::prepareData()
{
    QChart *chart = new QChart();
    chart->setTitle("Scatterchart 演示");
    chart->setAnimationOptions(QChart::AllAnimations);

    chart->legend()->hide();
    chart->createDefaultAxes();
    foreach (QLegendMarker* marker, chart->legend()->markers())
        connect(marker, SIGNAL(clicked()), this, SLOT(do_legendMarkerClicked()));
    QScatterSeries *series0 = new QScatterSeries();
    series0->setName("scatter1");
    series0->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    series0->setMarkerSize(15.0);

    QScatterSeries *series1 = new QScatterSeries();
    series1->setName("scatter2");
    series1->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    series1->setMarkerSize(20.0);

    QScatterSeries *series2 = new QScatterSeries();
    series2->setName("scatter3");
    series2->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    series2->setMarkerSize(30.0);
    series0->append(0, 6);
    series0->append(2, 4);
    series0->append(3, 8);
    series0->append(7, 4);
    series0->append(10, 5);
    *series1 << QPointF(1, 1) << QPointF(3, 3) << QPointF(7, 6) << QPointF(8, 3) << QPointF(10, 2);
    *series2 << QPointF(1, 5) << QPointF(4, 6) << QPointF(6, 3) << QPointF(9, 5);

    QPainterPath starPath;
    starPath.moveTo(28, 15);
    for (int i = 1; i < 5; ++i) {
        starPath.lineTo(14 + 14 * qCos(0.8 * i * Pi),
                        15 + 14 * qSin(0.8 * i * Pi));
    }
    starPath.closeSubpath();

    QImage star(30, 30, QImage::Format_ARGB32);
    star.fill(Qt::transparent);

    QPainter painter(&star);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QRgb(0xf6a625));
    painter.setBrush(painter.pen().color());
    painter.drawPath(starPath);
    m_penTool = new PenTool("Line pen", this);
    m_brushTool = new BrushTool("Line brush", this);
    m_penTool->pen().setStyle(Qt::DotLine);
    m_penTool->pen().setColor(painter.pen().color());
    m_penTool->pen().setWidth(10);
    series2->setBrush(star);
    series2->setPen(QColor(Qt::transparent));

    chart->addSeries(series0);
    chart->addSeries(series1);
    chart->addSeries(series2);
    //connect
    connect(series0,&QScatterSeries::clicked, this, &ScatterChartWidget::do_series_clicked);
    connect(series0,&QScatterSeries::hovered, this, &ScatterChartWidget::do_series_hovered);
    connect(series1,&QScatterSeries::clicked, this, &ScatterChartWidget::do_series_clicked);
    connect(series1,&QScatterSeries::hovered, this, &ScatterChartWidget::do_series_hovered);
    connect(series2,&QScatterSeries::clicked, this, &ScatterChartWidget::do_series_clicked);
    connect(series2,&QScatterSeries::hovered, this, &ScatterChartWidget::do_series_hovered);

    m_chartView=new TChartView();
    m_chartView->setChart(chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    connect(m_chartView,SIGNAL(mouseMovePoint(QPoint)),
                         this, SLOT(do_mouseMovePoint(QPoint))); //鼠标移动事件
    m_chartView->setCursor(Qt::CrossCursor);  //设置鼠标指针为十字星
    m_chartView->chart()->createDefaultAxes();
    m_chartView->chart()->setDropShadowEnabled(false);
}
//从图表中获取点坐标
int ScatterChartWidget::getIndexFromX(QXYSeries *series, qreal xValue, qreal tol)
{
    QList<QPointF> points= series->points();    //返回点的列表
    int index=-1;
    for (int i=0; i<points.count();i++)
    {
        qreal dx=xValue - points.at(i).x();
        if (qAbs(dx)<=tol)
        {
            index =i;
            break;
        }
    }
    return index;   //-1表示没有找到
}

void ScatterChartWidget::updateChartSettings()
{
    QChart::ChartTheme theme = static_cast<QChart::ChartTheme>(m_themeComboBox->itemData(
                m_themeComboBox->currentIndex()).toInt());
    m_chartView->chart()->setTheme(theme);
    m_chartView->setRenderHint(QPainter::Antialiasing, m_aaCheckBox->isChecked());

    if (m_animationsCheckBox->checkState() == Qt::Checked)
        m_chartView->chart()->setAnimationOptions(QChart::AllAnimations);
    else
        m_chartView->chart()->setAnimationOptions(QChart::NoAnimation);

    if (m_legendCheckBox->checkState() == Qt::Checked)
        m_chartView->chart()->legend()->show();
    else
        m_chartView->chart()->legend()->hide();
    m_chartView->chart()->zoomReset();
    m_chartView->chart()->zoom(m_AxissizeFactor->value());
}

void ScatterChartWidget::updateSerieSettings()
{

}

void ScatterChartWidget::updateLineSettings()
{

}

void ScatterChartWidget::handleSliceClicked()
{

}

void ScatterChartWidget::showFontDialog()
{
    if (!m_slice)
        return;

    QFontDialog dialog(m_slice->labelFont());
    dialog.show();
    dialog.exec();

    m_slice->setLabelFont(dialog.currentFont());
    m_font->setText(dialog.currentFont().toString());
}

void ScatterChartWidget::on_download_clicked()
{
    QPixmap p = m_chartView->grab();
            QImage image = p.toImage();
            QString curPath=QCoreApplication::applicationDirPath(); //获取应用程序的路径
            QString dlgTitle="保存文件"; //对话框标题
            QString filter="图片文件(*.png);;图片文件(*.jpg)"; //文件过滤器
            QString fileName=QFileDialog::getSaveFileName(this,dlgTitle,curPath,filter);
            if (!fileName.isEmpty()){
                qDebug()<<fileName;
                image.save(fileName);
                QString dialogTitle="save";
                QString strInfo="保存成功！";
                QMessageBox::about(this,dialogTitle,strInfo) ;
            }
}

void ScatterChartWidget::remove()
{
    m_lineName->setText("<click scatter>");
    m_chartView->chart()->removeAllSeries();
}
//鼠标移动事件
void ScatterChartWidget::do_mouseMovePoint(QPoint point)
{
    QPointF pt=m_chartView->chart()->mapToValue(point);    //转换为图表的数值
    QString str=QString::asprintf("Chart X=%.1f,Y=%.2f",pt.x(),pt.y());
    lab_chartXY->setText(str);  //状态栏显示
}
//序列点击事件，状态栏显示点坐标
void ScatterChartWidget::do_series_clicked(const QPointF &point)
{
    QString str=QString::asprintf("Clicked X=%.1f,Y=%.2f",point.x(),point.y());
    lab_clickXY->setText(str); //状态栏显示

    QScatterSeries *series = qobject_cast<QScatterSeries*> (sender());
    int index= getIndexFromX(series, point.x());

    if (index<0)
        return;

}
//序列悬停事件，状态栏显示点坐标
void ScatterChartWidget::do_series_hovered(const QPointF &point, bool state)
{
    QString str="Series X=, Y=";
    if (state)
        str=QString::asprintf("Hovered X=%.1f,Y=%.2f",point.x(),point.y());
    lab_hoverXY->setText(str); //状态栏显示


    QScatterSeries *series = qobject_cast<QScatterSeries*> (sender());
    QPen pen=series->pen();
    if (state)
        pen.setColor(Qt::red);
    else
        pen.setColor(0x4cb2e6);
    series->setPen(pen);
}


