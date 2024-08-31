#include "areachartwidget.h"
#include "pentool.h"
#include "brushtool.h"
#include "customslice.h"

#include <QtCharts/QScatterSeries>
#include <QtCharts/QLegendMarker>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtCore/QtMath>

AreaChartWidget::AreaChartWidget(QWidget *parent) : QWidget(parent),m_slice(0)
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
            this, &AreaChartWidget::updateChartSettings);
    connect(m_aaCheckBox, &QCheckBox::toggled, this, &AreaChartWidget::updateChartSettings);
    connect(m_animationsCheckBox, &QCheckBox::toggled, this, &AreaChartWidget::updateChartSettings);
    connect(m_legendCheckBox, &QCheckBox::toggled, this, &AreaChartWidget::updateChartSettings);

    // series settings


    m_AxissizeFactor = new QDoubleSpinBox();
    m_AxissizeFactor->setMinimum(1.0);
    m_AxissizeFactor->setValue(1.0);
    m_AxissizeFactor->setSingleStep(0.1);
    //m_AxissizeFactor->setValue(m_series->)
    connect(m_AxissizeFactor, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &AreaChartWidget::updateChartSettings);
    QPushButton *removearea = new QPushButton("清空区域");
    QFormLayout *seriesSettingsLayout = new QFormLayout();
    seriesSettingsLayout->addRow("缩放", m_AxissizeFactor);
    seriesSettingsLayout->addRow(removearea);
    QGroupBox *seriesSettings = new QGroupBox("Series");
    seriesSettings->setLayout(seriesSettingsLayout);
    connect(removearea, &QPushButton::clicked, this, &AreaChartWidget::remove);

    // line settings
    m_lineName = new QLineEdit("<click an area>");
    m_lineName->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    m_pen = new QPushButton();
    m_brush = new QPushButton();
    m_font = new QPushButton();
    m_labelBrush = new QPushButton();
    m_labelBrushTool = new BrushTool("Label brush", this);

    QFormLayout *lineSettingsLayout = new QFormLayout();
    lineSettingsLayout->addRow("标题名", m_lineName);
    lineSettingsLayout->addRow("区域颜色", m_pen);
    lineSettingsLayout->addRow("填充色", m_brush);
    lineSettingsLayout->addRow("标题格式", m_font);
    lineSettingsLayout->addRow("标题颜色", m_labelBrush);
    QGroupBox *lineSettings = new QGroupBox("选中区域");
    lineSettings->setLayout(lineSettingsLayout);

    //connect

    connect(m_lineName, &QLineEdit::textChanged, this, &AreaChartWidget::updateLineSettings);

    connect(m_pen, &QPushButton::clicked, m_penTool, &PenTool::show);
    connect(m_penTool, &PenTool::changed, this, &AreaChartWidget::updateLineSettings);
    connect(m_brush, &QPushButton::clicked, m_brushTool, &BrushTool::show);
    connect(m_brushTool, &BrushTool::changed, this, &AreaChartWidget::updateLineSettings);
    connect(m_labelBrush, &QPushButton::clicked, m_labelBrushTool, &BrushTool::show);
    connect(m_labelBrushTool, &BrushTool::changed, this, &AreaChartWidget::updateLineSettings);
    connect(m_font, &QPushButton::clicked, this, &AreaChartWidget::showFontDialog);


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
    settingsLayout->addWidget(lineSettings);
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

void AreaChartWidget::prepareData()
{
    QChart *chart = new QChart();
    chart->setTitle("Areachart 演示");
    chart->legend()->hide();
    chart->setAnimationOptions(QChart::AllAnimations);
      foreach (QLegendMarker* marker, chart->legend()->markers())
        connect(marker, SIGNAL(clicked()), this, SLOT(do_legendMarkerClicked()));
    QLineSeries *series0 = new QLineSeries();
    QLineSeries *series1 = new QLineSeries();
    *series0 << QPointF(1, 5) << QPointF(3, 7) << QPointF(7, 6) << QPointF(9, 7) << QPointF(12, 6)
             << QPointF(16, 7) << QPointF(18, 5);
    *series1 << QPointF(1, 3) << QPointF(3, 4) << QPointF(7, 3) << QPointF(8, 2) << QPointF(12, 3)
             << QPointF(16, 4) << QPointF(18, 3);
    m_series=new QAreaSeries(series0, series1);
    m_series->setName("Batman");
    m_penTool = new PenTool("Area pen", this);
    m_brushTool = new BrushTool("Area brush", this);
    m_penTool->pen().setWidth(3);
    m_penTool->pen().setColor(0x059605);
    m_series->setPen(m_penTool->pen());

    QLinearGradient gradient(QPointF(0, 0), QPointF(0, 1));
    gradient.setColorAt(0.0, 0x3cc63c);
    gradient.setColorAt(1.0, 0x26f626);
    gradient.setCoordinateMode(QGradient::ObjectBoundingMode);

    m_series->setBrush(gradient);
    m_brushTool->brush().setColor(m_series->brush().color());
    m_series->setName("Area_1");
    chart->addSeries(m_series);
    chart->createDefaultAxes();
    chart->axisX()->setRange(0, 20);
    chart->axisY()->setRange(0, 10);


    connect(m_series,&QAreaSeries::clicked, this, &AreaChartWidget::do_series_clicked);
    connect(m_series,&QAreaSeries::hovered, this, &AreaChartWidget::do_series_hovered);
    m_chartView=new TChartView();
    m_chartView->setChart(chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    connect(m_chartView,SIGNAL(mouseMovePoint(QPoint)),
                         this, SLOT(do_mouseMovePoint(QPoint))); //鼠标移动事件
    m_chartView->setCursor(Qt::CrossCursor);  //设置鼠标指针为十字星
    m_chartView->chart()->createDefaultAxes();
    m_chartView->chart()->setDropShadowEnabled(false);
}

int AreaChartWidget::getIndexFromX(QXYSeries *series, qreal xValue, qreal tol)
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

void AreaChartWidget::updateChartSettings()
{
    QChart::ChartTheme theme = static_cast<QChart::ChartTheme>(m_themeComboBox->itemData(
                m_themeComboBox->currentIndex()).toInt());
    m_chartView->chart()->setTheme(theme);
    m_chartView->setRenderHint(QPainter::Antialiasing, m_aaCheckBox->isChecked());

    if (m_legendCheckBox->checkState() == Qt::Checked)
        m_chartView->chart()->legend()->show();
    else
        m_chartView->chart()->legend()->hide();
    m_chartView->chart()->zoomReset();
    m_chartView->chart()->zoom(m_AxissizeFactor->value());
}

void AreaChartWidget::updateSerieSettings()
{

}

void AreaChartWidget::updateLineSettings()
{
    m_series->chart()->setTitle(m_lineName->text());
    m_series->setPen(m_penTool->pen());

    //m_series->setBrush(m_brushTool->brush().color());
    m_pen->setText(PenTool::name(m_series->pen()));
    m_brush->setText(m_series->brush().color().name());
}

void AreaChartWidget::showFontDialog()
{
    if (!m_series)
        return;

    QFontDialog dialog(m_series->chart()->titleFont());
    dialog.show();
    dialog.exec();

    m_slice->setLabelFont(dialog.currentFont());
    m_font->setText(dialog.currentFont().toString());
}

void AreaChartWidget::on_download_clicked()
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

void AreaChartWidget::remove()
{
    m_lineName->setText("<click an area>");
    m_chartView->chart()->removeSeries(m_series);
}

void AreaChartWidget::do_mouseMovePoint(QPoint point)
{
    QPointF pt=m_chartView->chart()->mapToValue(point);    //转换为图表的数值
    QString str=QString::asprintf("Chart X=%.1f,Y=%.2f",pt.x(),pt.y());
    lab_chartXY->setText(str);  //状态栏显示
}

void AreaChartWidget::do_series_clicked(const QPointF &point)
{
    QAreaSeries *series = qobject_cast<QAreaSeries*> (sender());
    QString str=QString::asprintf("Clicked X=%.1f,Y=%.2f",point.x(),point.y());
    lab_clickXY->setText(str); //状态栏显示

    m_lineName->setText(series->name());
    m_pen->setText(PenTool::name(m_series->pen()));

    m_brush->setText(BrushTool::name(m_series->brush()));

    m_font->setText(m_series->name());
    m_labelBrush->setText(m_series->name());
}

void AreaChartWidget::do_series_hovered(const QPointF &point, bool state)
{
    QString str="Series X=, Y=";
    if (state)
        str=QString::asprintf("Hovered X=%.1f,Y=%.2f",point.x(),point.y());
    lab_hoverXY->setText(str); //状态栏显示


    QAreaSeries *series = qobject_cast<QAreaSeries*> (sender());
    QPen pen=series->pen();
    if (state)
        pen.setColor(Qt::red);
    else
        pen.setColor(0x4cb2e6);
    series->setPen(pen);
}
