#include "barchartwidget.h"
#include "tablewidget.h"
#include "pentool.h"
#include "brushtool.h"
#include "customslice.h"

BarChartWidget::BarChartWidget(QStandardItemModel  *dataModel,QWidget *parent) : QWidget(parent),m_slice(0),dataModel(dataModel)
{
    m_series = new QBarSeries(this);
    m_chart = new QChart();
    this->setMouseTracking(true);
    m_chart->setAnimationOptions(QChart::AllAnimations);
    setPalette(Qt::white);
    setAutoFillBackground(true);
    prepareData();

    m_penTool = new PenTool("Bar pen", this);
    m_brushTool = new BrushTool("Bar brush", this);
    m_penTool->pen().setStyle(Qt::DotLine);

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
    m_legendCheckBox->setCheckState(Qt::Checked);
    QFormLayout *chartSettingsLayout = new QFormLayout();
    chartSettingsLayout->addRow("主题", m_themeComboBox);
    chartSettingsLayout->addRow("抗锯齿", m_aaCheckBox);
    chartSettingsLayout->addRow("动画", m_animationsCheckBox);
    chartSettingsLayout->addRow("图例", m_legendCheckBox);
    QGroupBox *chartSettings = new QGroupBox("Chart");
    chartSettings->setLayout(chartSettingsLayout);

    connect(m_themeComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &BarChartWidget::updateChartSettings);
    connect(m_aaCheckBox, &QCheckBox::toggled, this, &BarChartWidget::updateChartSettings);
    connect(m_animationsCheckBox, &QCheckBox::toggled, this, &BarChartWidget::updateChartSettings);
    connect(m_legendCheckBox, &QCheckBox::toggled, this, &BarChartWidget::updateChartSettings);

    m_AxissizeFactor=new QDoubleSpinBox();
    m_AxissizeFactor->setMinimum(1.0);
    m_AxissizeFactor->setValue(1.0);
    m_AxissizeFactor->setSingleStep(0.1);
    connect(m_AxissizeFactor, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &BarChartWidget::updateChartSettings);
    QPushButton *removeline = new QPushButton("清空区域");
    QFormLayout *seriesSettingsLayout = new QFormLayout();
    seriesSettingsLayout->addRow("缩放", m_AxissizeFactor);
    seriesSettingsLayout->addRow(removeline);
    QGroupBox *seriesSettings = new QGroupBox("Series");
    seriesSettings->setLayout(seriesSettingsLayout);
    connect(removeline, &QPushButton::clicked, this, &BarChartWidget::remove);



    statusBar=new QStatusBar();
    statusBar->setPalette(QPalette(Qt::white));



    m_chartView->setRenderHint(QPainter::Antialiasing);

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

void BarChartWidget::prepareData()
{

    m_chart->setTitle("Horizontal BarChart 演示");
    m_chart->removeAllSeries();   //删除所有序列
    removeAllAxis(m_chart);       //删除左右坐标轴


    //创建一个QLineSeries序列用于显示平均分
    QLineSeries *seriesLine = new QLineSeries();
    seriesLine->setName("平均分");
    QPen  pen(Qt::red);
    pen.setWidth(2);
    seriesLine->setPen(pen);
    QFont font=seriesLine->pointLabelsFont();
    font.setPointSize(12);
    font.setBold(true);
    seriesLine->setPointLabelsFont(font);
    seriesLine->setPointLabelsVisible(true);    //显示数据点的数值

    seriesLine->setPointLabelsFormat("@xPoint");

    //创建三个QBarSet数据集,从数据模型获取数据
    QBarSet *setMath = new QBarSet("数学");
    QBarSet *setChinese = new QBarSet("语文");
    QBarSet *setEnglish= new QBarSet("英语");
    for(int i=0;i<dataModel->rowCount();i++)
    {
        setMath->append(dataModel->item(i,COL_MATH)->text().toInt());       //数学
        setChinese->append(dataModel->item(i,COL_CHINESE)->text().toInt()); //语文
        setEnglish->append(dataModel->item(i,COL_ENGLISH)->text().toInt()); //英语
        seriesLine->append(dataModel->item(i,COL_AVERAGE)->text().toDouble(), i);  //平均分
    }

    //创建一个柱状图序列 QBarSeries, 并添加三个数据集
    QAbstractBarSeries *seriesBar;

    seriesBar = new QHorizontalBarSeries();
    seriesBar->setLabelsVisible(true);      //显示棒柱的标签
    seriesBar->setLabelsFormat("@value");  //棒柱标签格式

    seriesBar->append(setMath);     //添加数据集
    seriesBar->append(setChinese);
    seriesBar->append(setEnglish);
    connect(seriesBar,&QBarSeries::hovered, this,&BarChartWidget::do_barHovered);
    connect(seriesBar,&QBarSeries::clicked, this,&BarChartWidget::do_barClicked);
    m_chart->addSeries(seriesBar); //添加柱状图序列
    //m_chart->addSeries(seriesLine); //添加折线图序列

    //QBarCategoryAxis 坐标轴
    QStringList categories;
    for (int i=0;i<dataModel->rowCount();i++)
        categories <<dataModel->item(i,COL_NAME)->text();
    QBarCategoryAxis *axisStud = new QBarCategoryAxis();    //用于柱状图的坐标轴
    axisStud->append(categories); //添加横坐标文字列表
    axisStud->setRange(categories.at(0), categories.at(categories.count()-1)); //这只坐标轴范围

    //QValueAxis 坐标轴
    QValueAxis *axisValue = new QValueAxis();
    axisValue->setRange(0, 100);
    axisValue->setTitleText("分数");
    axisValue->setTickCount(6);
    axisValue->setLabelFormat("%.0f"); //标签格式
    axisValue->applyNiceNumbers();

    //为chart和序列添加坐标轴

    m_chart->addAxis(axisStud, Qt::AlignLeft);
    m_chart->addAxis(axisValue, Qt::AlignBottom);
    seriesBar->attachAxis(axisStud);
    seriesBar->attachAxis(axisValue);
    seriesLine->attachAxis(axisStud);
    seriesLine->attachAxis(axisValue);
    m_chart->legend()->setAlignment(Qt::AlignTop); //图例显示在上方
    //m_chart->legend()->setVisible(true); //显示图例
    m_chartView=new TChartView();
    m_chartView->setChart(m_chart);


}

void BarChartWidget::removeAllAxis(QChart *chart)
{//删除一个chart的所有坐标轴
    QList<QAbstractAxis *> axisList=chart->axes();  //获取坐标轴列表
    int count=axisList.count();
    for(int i=0; i<count; i++ )
    {
        QAbstractAxis *one=axisList.at(0);
        chart->removeAxis(one);     //从图表中移除坐标轴，
        axisList.removeFirst();     //从列表中移除坐标轴
        delete one;     //删除坐标轴对象，释放内存
    }
}

void BarChartWidget::updateChartSettings()
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
    m_chartView->chart()->setTheme(QChart::ChartThemeLight);

    m_chartView->chart()->zoomReset();
    m_chartView->chart()->zoom(m_AxissizeFactor->value());

    m_chartView->chart()->legend()->setVisible(true);
}

void BarChartWidget::updateSerieSettings()
{
     m_series->setBarWidth(0.5);
}

void BarChartWidget::showFontDialog()
{
    if (!m_slice)
        return;

    QFontDialog dialog(m_slice->labelFont());
    dialog.show();
    dialog.exec();

    m_slice->setLabelFont(dialog.currentFont());
    m_font->setText(dialog.currentFont().toString());
}

void BarChartWidget::on_download_clicked()
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

void BarChartWidget::remove()
{
    m_lineName->setText("<click a bar>");
    m_chartView->chart()->removeAllSeries();
}

void BarChartWidget::do_barHovered(bool status, int index, QBarSet *barset)
{
    QString str= "hovered barSet="+barset->label();
        if (status)
            str += QString::asprintf(", index=%d, value=%.2f",index, barset->at(index));
        else
            str="";
        statusBar->showMessage(str);
}

void BarChartWidget::do_barClicked(int index, QBarSet *barset)
{
    QString str= "clicked barSet="+barset->label();
    str += QString::asprintf(", index=%d, count=%d",index, barset->count());
    statusBar->showMessage(str);
}

void BarChartWidget::do_legendMarkerClicked()
{

    QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());

    marker->series()->setVisible(!marker->series()->isVisible()); //关联序列的可见性
    marker->setVisible(true);       //图例总是可见的
    qreal alpha = 1.0;
    if (!marker->series()->isVisible())
        alpha = 0.5;                //用半透明表示序列不可见

    QBrush brush = marker->labelBrush();
    QColor color = brush.color();
    color.setAlphaF(alpha);
    brush.setColor(color);
    marker->setLabelBrush(brush);   //设置文字的brush

    brush = marker->brush();
    color = brush.color();
    color.setAlphaF(alpha);
    brush.setColor(color);
    marker->setBrush(brush);        //设置图例标记的brush

    //    QPen pen = marker->pen();
    //    color = pen.color();
    //    color.setAlphaF(alpha);
    //    pen.setColor(color);
    //    marker->setPen(pen);
}
