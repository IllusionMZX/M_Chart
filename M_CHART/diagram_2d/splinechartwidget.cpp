#include "splinechartwidget.h"
#include "pentool.h"
#include "brushtool.h"
#include "customslice.h"

SplineChartWidget::SplineChartWidget(QStandardItemModel  *dataModel,QWidget *parent) : QWidget(parent),dataModel(dataModel),m_slice(0)
{
    this->setMouseTracking(true);
    // create chart
    QChart *chart = new QChart;
    chart->setTitle("Splinechart 演示");
    chart->setAnimationOptions(QChart::AllAnimations);
    setPalette(Qt::white);
    setAutoFillBackground(true);
    prepareData();
    connect(m_series,&QLineSeries::clicked, this, &SplineChartWidget::do_series_clicked);
    connect(m_series,&QLineSeries::hovered, this, &SplineChartWidget::do_series_hovered);

    m_penTool = new PenTool("Line pen", this);
    m_brushTool = new BrushTool("Line brush", this);
    m_penTool->pen().setStyle(Qt::DotLine);
    m_penTool->pen().setColor(m_series->pen().color());
    m_penTool->pen().setWidth(10);

    chart->legend()->hide();
    chart->addSeries(m_series);
    chart->createDefaultAxes();
    chart->setTitle("Splinechart 演示");
    foreach (QLegendMarker* marker, chart->legend()->markers())
        connect(marker, SIGNAL(clicked()), this, SLOT(do_legendMarkerClicked()));



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
            this, &SplineChartWidget::updateChartSettings);
    connect(m_aaCheckBox, &QCheckBox::toggled, this, &SplineChartWidget::updateChartSettings);
    connect(m_animationsCheckBox, &QCheckBox::toggled, this, &SplineChartWidget::updateChartSettings);
    connect(m_legendCheckBox, &QCheckBox::toggled, this, &SplineChartWidget::updateChartSettings);

    // series settings


    m_AxissizeFactor = new QDoubleSpinBox();
    m_AxissizeFactor->setMinimum(1.0);
    m_AxissizeFactor->setValue(1.0);
    m_AxissizeFactor->setSingleStep(0.1);
    connect(m_AxissizeFactor, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &SplineChartWidget::updateChartSettings);

    QPushButton *removeline = new QPushButton("删除线条");
    QFormLayout *seriesSettingsLayout = new QFormLayout();
    seriesSettingsLayout->addRow("缩放", m_AxissizeFactor);
    seriesSettingsLayout->addRow(removeline);
    QGroupBox *seriesSettings = new QGroupBox("Series");
    seriesSettings->setLayout(seriesSettingsLayout);
    connect(removeline, &QPushButton::clicked, this, &SplineChartWidget::remove);

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

    m_chartView=new TChartView();
    m_chartView->setChart(chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    connect(m_chartView,SIGNAL(mouseMovePoint(QPoint)),
                         this, SLOT(do_mouseMovePoint(QPoint)));  //鼠标移动事件
    connect(m_series,&QLineSeries::clicked, this, &SplineChartWidget::do_series_clicked);
    connect(m_series,&QLineSeries::hovered, this, &SplineChartWidget::do_series_hovered);
    QVBoxLayout *settingsLayout = new QVBoxLayout();
    settingsLayout->addWidget(chartSettings);
    settingsLayout->addWidget(seriesSettings);
    settingsLayout->addStretch();


    m_chartView->setCursor(Qt::CrossCursor);  //设置鼠标指针为十字星


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

void SplineChartWidget::prepareData()
{
    m_series= new QSplineSeries();
    m_series->setPointsVisible(true);
    for(int i=0;i<dataModel->columnCount();i++){
        *m_series << QPointF(dataModel->item(0,i)->text().toInt(),dataModel->item(1,i)->text().toInt());
    }

}

int SplineChartWidget::getIndexFromX(QXYSeries *series, qreal xValue, qreal tol)
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

void SplineChartWidget::updateChartSettings()
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

void SplineChartWidget::updateSerieSettings()
{

}

void SplineChartWidget::updateLineSettings()
{

}

void SplineChartWidget::handleSliceClicked()
{

}

void SplineChartWidget::showFontDialog()
{
    if (!m_slice)
        return;

    QFontDialog dialog(m_slice->labelFont());
    dialog.show();
    dialog.exec();

    m_slice->setLabelFont(dialog.currentFont());
    m_font->setText(dialog.currentFont().toString());
}

void SplineChartWidget::on_download_clicked()
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

void SplineChartWidget::remove()
{
    m_lineName->setText("<click a line>");
    m_chartView->chart()->removeSeries(m_series);
}

void SplineChartWidget::do_mouseMovePoint(QPoint point)
{
    QPointF pt=m_chartView->chart()->mapToValue(point);    //转换为图表的数值
    QString str=QString::asprintf("Chart X=%.1f,Y=%.2f",pt.x(),pt.y());
    lab_chartXY->setText(str);  //状态栏显示
}

void SplineChartWidget::do_series_clicked(const QPointF &point)
{
    QString str=QString::asprintf("Clicked X=%.1f,Y=%.2f",point.x(),point.y());
    lab_clickXY->setText(str); //状态栏显示

    QLineSeries *series = qobject_cast<QLineSeries*> (sender());
    int index= getIndexFromX(series, point.x());

    if (index<0)
        return;

}

void SplineChartWidget::do_series_hovered(const QPointF &point, bool state)
{
    QString str="Series X=, Y=";
    if (state)
        str=QString::asprintf("Hovered X=%.1f,Y=%.2f",point.x(),point.y());
    lab_hoverXY->setText(str); //状态栏显示


    QLineSeries *series = qobject_cast<QLineSeries*> (sender());
    QPen pen=series->pen();
    if (state)
        pen.setColor(Qt::red);
    else
        pen.setColor(0x4cb2e6);
    series->setPen(pen);
}


