/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "themewidget.h"
#include "diagram_2d/piechartwidget.h"
#include "diagram_2d/linechartwidget.h"
#include "diagram_2d/tablewidget.h"
#include "diagram_2d/splinechartwidget.h"
#include "diagram_2d/scatterchartwidget.h"
#include "diagram_2d/areachartwidget.h"
#include "diagram_2d/barchartwidget.h"

#include <QtDebug>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtCore/QTime>
#include <QtCharts/QBarCategoryAxis>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QToolButton>
#include <QSize>

ThemeWidget::ThemeWidget(QWidget *parent) :
    QWidget(parent),
    m_listCount(3),
    m_valueMax(10),
    m_valueCount(7),
    m_dataTable(generateRandomData(m_listCount, m_valueMax, m_valueCount)),
    m_themeComboBox(createThemeBox()),
    m_antialiasCheckBox(new QCheckBox("抗锯齿")),
    m_animatedComboBox(createAnimationBox()),
    m_legendComboBox(createLegendBox()),
    tablescreen(createsToolBtn())
{
    this->setMouseTracking(true);
    connectSignals();
    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);
    table=new TableWidget();

    QGridLayout *baseLayout = new QGridLayout();
    homepage=new QToolButton();
    homepage->setIcon(QIcon(":/images/image/home.png"));
    homepage->setIconSize(QSize(50,50));
    homepage->setToolTip("主页");
    baseLayout->addWidget(homepage,0,0);
    connect(homepage,SIGNAL(clicked()),this,SLOT(on_homepage_clicked()));
    QLabel *label=new QLabel();
    label->setText("SELECT CHART");
    label->setFont(QFont("Comic Sans MS",30));
    label->setAlignment(Qt::AlignCenter);
    baseLayout->addWidget(label,0,1);


    tablescreen->setIcon(QIcon(":/images/image/table.png"));
    tablescreen->setIconSize(QSize(50,50));
    tablescreen->setToolTip("编辑数据");
    baseLayout->addWidget(tablescreen,0,2);

    helpbtn=new QToolButton();
    helpbtn->setIcon(QIcon(":/images/image/help.png"));
    helpbtn->setIconSize(QSize(50,50));
    helpbtn->setToolTip("①点击左上角主界面按钮可返回主界面\n②点击标题右侧表格按钮编辑表格数据\n③点击各个统计图表根据表格数据生成特定的统计图\n④饼图支持一维数据，折线图和曲线图支持二维数据，面积图和散点图暂不支持数据同步生成😖");

    baseLayout->addWidget(helpbtn,0,3);

    QChartView *chartView;
    chartView = new QChartView(createAreaChart());
    baseLayout->addWidget(chartView, 2, 0);
    m_charts << chartView;

    chartView = new QChartView(createBarChart(m_valueCount));
    baseLayout->addWidget(chartView, 1, 1);
    m_charts << chartView;

    chartView = new QChartView(createLineChart());
    baseLayout->addWidget(chartView, 1, 2);
    m_charts << chartView;

    chartView = new QChartView(createPieChart());

    chartView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    baseLayout->addWidget(chartView, 1, 0);
    m_charts << chartView;

    chartView = new QChartView(createSplineChart());
    baseLayout->addWidget(chartView, 2, 2);
    m_charts << chartView;

    chartView = new QChartView(createScatterChart());
    baseLayout->addWidget(chartView, 2, 1);
    m_charts << chartView;

    setLayout(baseLayout);
    m_antialiasCheckBox->setChecked(true);
    updateUI();
}

ThemeWidget::~ThemeWidget()
{
}

void ThemeWidget::closeEvent(QCloseEvent *e)
{
    //窗口关闭时弹出的提示窗口
    QMessageBox msgBox;
    msgBox.setPalette(Qt::white);
    msgBox.setText("提示");
    msgBox.setInformativeText("确认退出?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok){
        //若用户点击确认，则接收这个事件,当前窗口会关闭
        emit backhome();
        e->accept();
    }else{
        //若用户点击取消，则忽略这个事件，当前窗口不会关闭
        e->ignore();
    }
}

void ThemeWidget::connectSignals()
{
    connect(m_themeComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &ThemeWidget::updateUI);
    connect(m_antialiasCheckBox, &QCheckBox::toggled, this, &ThemeWidget::updateUI);
    connect(m_animatedComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &ThemeWidget::updateUI);
    connect(m_legendComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &ThemeWidget::updateUI);
    connect(tablescreen, SIGNAL(clicked()), this, SLOT(on_tablescreen_clicked()));

    qDebug()<<"1";
}

//产生随机数据
DataTable ThemeWidget::generateRandomData(int listCount, int valueMax, int valueCount) const
{
    DataTable dataTable;

    // set seed for random stuff
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    // generate random data
    for (int i(0); i < listCount; i++) {
        DataList dataList;
        qreal yValue(0);
        for (int j(0); j < valueCount; j++) {
            yValue = yValue + (qreal)(qrand() % valueMax) / (qreal) valueCount;
            QPointF value((j + (qreal) rand() / (qreal) RAND_MAX) * ((qreal) m_valueMax / (qreal) valueCount),
                          yValue);
            QString label = "Slice " + QString::number(i) + ":" + QString::number(j);
            dataList << Data(value, label);
        }
        dataTable << dataList;
    }

    return dataTable;
}

QComboBox *ThemeWidget::createThemeBox() const
{
    // settings layout
    QComboBox *themeComboBox = new QComboBox();
    themeComboBox->addItem("Light", QChart::ChartThemeLight);
    themeComboBox->addItem("Blue Cerulean", QChart::ChartThemeBlueCerulean);
    themeComboBox->addItem("Dark", QChart::ChartThemeDark);
    themeComboBox->addItem("Brown Sand", QChart::ChartThemeBrownSand);
    themeComboBox->addItem("Blue NCS", QChart::ChartThemeBlueNcs);
    themeComboBox->addItem("High Contrast", QChart::ChartThemeHighContrast);
    themeComboBox->addItem("Blue Icy", QChart::ChartThemeBlueIcy);
    themeComboBox->addItem("Qt", QChart::ChartThemeQt);
    return themeComboBox;
}

QComboBox *ThemeWidget::createAnimationBox() const
{
    // settings layout
    QComboBox *animationComboBox = new QComboBox();
    animationComboBox->addItem("All Animations", QChart::AllAnimations);
    animationComboBox->addItem("GridAxis Animations", QChart::GridAxisAnimations);
    animationComboBox->addItem("Series Animations", QChart::SeriesAnimations);
    animationComboBox->addItem("No Animations", QChart::NoAnimation);
    return animationComboBox;
}

QComboBox *ThemeWidget::createLegendBox() const
{
    QComboBox *legendComboBox = new QComboBox();
    legendComboBox->addItem("No Legend ", 0);
    legendComboBox->addItem("Legend Top", Qt::AlignTop);
    legendComboBox->addItem("Legend Bottom", Qt::AlignBottom);
    legendComboBox->addItem("Legend Left", Qt::AlignLeft);
    legendComboBox->addItem("Legend Right", Qt::AlignRight);
    return legendComboBox;
}

QTableView *ThemeWidget::createtable()
{
    QTableView*table=new QTableView();
    return table;
}

QToolButton *ThemeWidget::createsToolBtn()
{
    QToolButton*button=new QToolButton();
    return button;
}

//生成面积图
QChart *ThemeWidget::createAreaChart() const
{
    QChart *chart = new QChart();
    chart->setTitle("Area chart");

    // The lower series initialized to zero values
    QLineSeries *lowerSeries = 0;
    QString name("Series ");
    int nameIndex = 0;
    for (int i(0); i < m_dataTable.count(); i++) {
        QLineSeries *upperSeries = new QLineSeries(chart);
        for (int j(0); j < m_dataTable[i].count(); j++) {
            Data data = m_dataTable[i].at(j);
            if (lowerSeries) {
                const QVector<QPointF>& points = lowerSeries->pointsVector();
                upperSeries->append(QPointF(j, points[i].y() + data.first.y()));
            } else {
                upperSeries->append(QPointF(j, data.first.y()));
            }
        }
        QAreaSeries *area = new QAreaSeries(upperSeries, lowerSeries);
        area->setName(name + QString::number(nameIndex));
        nameIndex++;
        chart->addSeries(area);
        chart->createDefaultAxes();
        lowerSeries = upperSeries;
    }

    return chart;
}

//生成柱状图
QChart *ThemeWidget::createBarChart(int valueCount) const
{
    Q_UNUSED(valueCount);
    QChart *chart = new QChart();
    chart->setTitle("Bar chart");

    QStackedBarSeries *series = new QStackedBarSeries(chart);
    for (int i(0); i < m_dataTable.count(); i++) {
        QBarSet *set = new QBarSet("Bar set " + QString::number(i));
        for (const Data &data : m_dataTable[i])
            *set << data.first.y();
        series->append(set);
    }
    chart->addSeries(series);
    chart->createDefaultAxes();

    return chart;
}

//生成折线图
QChart *ThemeWidget::createLineChart() const
{
    QChart *chart = new QChart();
    chart->setTitle("Line chart");

    QString name("Series ");
    int nameIndex = 0;
    for (const DataList &list : m_dataTable) {
        QLineSeries *series = new QLineSeries(chart);
        for (const Data &data : list)
            series->append(data.first);
        series->setName(name + QString::number(nameIndex));
        nameIndex++;
        chart->addSeries(series);
    }
    chart->createDefaultAxes();

    return chart;
}

//生成饼图
QChart *ThemeWidget::createPieChart() const
{
    QChart *chart = new QChart();
    chart->setTitle("Pie chart");

    qreal pieSize = 1.0 / m_dataTable.count();
    for (int i = 0; i < m_dataTable.count(); i++) {
        QPieSeries *series = new QPieSeries(chart);
        for (const Data &data : m_dataTable[i]) {
            QPieSlice *slice = series->append(data.second, data.first.y());
            if (data == m_dataTable[i].first()) {
                slice->setLabelVisible();
                slice->setExploded();
            }
        }
        qreal hPos = (pieSize / 2) + (i / (qreal) m_dataTable.count());
        series->setPieSize(pieSize);
        series->setHorizontalPosition(hPos);
        series->setVerticalPosition(0.5);
        chart->addSeries(series);
    }

    return chart;
}

//生成平滑曲线图
QChart *ThemeWidget::createSplineChart() const
{
    // spine chart
    QChart *chart = new QChart();
    chart->setTitle("Spline chart");
    QString name("Series ");
    int nameIndex = 0;
    for (const DataList &list : m_dataTable) {
        QSplineSeries *series = new QSplineSeries(chart);
        for (const Data &data : list)
            series->append(data.first);
        series->setName(name + QString::number(nameIndex));
        nameIndex++;
        chart->addSeries(series);
    }
    chart->createDefaultAxes();
    return chart;
}

//生成散点图
QChart *ThemeWidget::createScatterChart() const
{
    // scatter chart
    QChart *chart = new QChart();
    chart->setTitle("Scatter chart");
    QString name("Series ");
    int nameIndex = 0;
    for (const DataList &list : m_dataTable) {
        QScatterSeries *series = new QScatterSeries(chart);
        for (const Data &data : list)
            series->append(data.first);
        series->setName(name + QString::number(nameIndex));
        nameIndex++;
        chart->addSeries(series);
    }
    chart->createDefaultAxes();
    return chart;
}

//鼠标点击不同图表事件处理
void ThemeWidget::mousePressEvent(QMouseEvent *event)
{

    if (event->button()==Qt::LeftButton){
        selectPoint = event->pos();
        qDebug()<<"123";
        for(int i=0;i<6;i++){
            if(selectPoint.x()>(m_charts[i]->x())&&selectPoint.x()<(m_charts[i]->x()+m_charts[i]->width())
                    &&selectPoint.y()>(m_charts[i]->y())&&selectPoint.y()<(m_charts[i]->y()+m_charts[i]->height())){
                switch (i) {
                case 0:
                    {
                        qDebug()<<"1";
                        AreaChartWidget*wid=new AreaChartWidget();
                        wid->resize(1500,1000);
                        wid->setWindowTitle("Area Chart");
                        wid->show();
                        qDebug()<<"1";
                        break;
                    }
                case 1:
                    {
                        qDebug()<<"2";
                        BarChartWidget*wid=new BarChartWidget(table->dataModel);
                        wid->resize(1500,1000);
                        wid->setWindowTitle("Bar Chart");
                        wid->show();
                        break;
                    }
                case 2:
                    {
                        qDebug()<<"3";
                        if(table->dataModel->rowCount()==2){
                            LineChartWidget*wid=new LineChartWidget(table->dataModel);
                            wid->resize(1500,1000);
                            wid->setWindowTitle("Line Chart");
                            wid->show();
                            qDebug()<<"1";
                        }
                        else{
                            QString dialogTitle = "提示";
                            QString strInfo = "该数据不适合生成折线图！";
                            QMessageBox::about(this, dialogTitle, strInfo);
                        }
                        break;
                    }
                case 3:
                    {
                        qDebug()<<"4";
                        if(table->dataModel->rowCount()==1){
                            PieChartWidget*wid=new PieChartWidget(table->dataModel);
                            wid->resize(1500,1000);
                            wid->setWindowTitle("Pie Chart");
                            wid->show();
                            qDebug()<<"1";
                        }
                        else{
                            QString dialogTitle = "提示";
                            QString strInfo = "该数据不适合生成饼图！";
                            QMessageBox::about(this, dialogTitle, strInfo);
                        }

                        break;
                    }
                case 4:
                    {
                        qDebug()<<"5";
                        if(table->dataModel->rowCount()==2){
                            SplineChartWidget *wid=new SplineChartWidget(table->dataModel);
                            wid->resize(1500,1000);
                            wid->setWindowTitle("Spline Chart");
                            wid->show();
                        }
                        else{
                            QString dialogTitle = "提示";
                            QString strInfo = "该数据不适合生成曲线图！";
                            QMessageBox::about(this, dialogTitle, strInfo);
                        }
                        break;
                    }
                case 5:
                    {
                        qDebug()<<"6";
                        ScatterChartWidget *wid=new ScatterChartWidget();
                        wid->resize(1500,1000);
                        wid->setWindowTitle("Scatter Chart");
                        wid->show();
                        break;
                    }
                }
            }
        }
    }
    QWidget::mousePressEvent(event);
}

//更新界面
void ThemeWidget::updateUI()
{
    resize(1800,900);
    QChart::ChartTheme theme = static_cast<QChart::ChartTheme>(
                m_themeComboBox->itemData(m_themeComboBox->currentIndex()).toInt());

    const auto charts = m_charts;
    if (m_charts.at(0)->chart()->theme() != theme) {
        for (QChartView *chartView : charts)
            chartView->chart()->setTheme(theme);

        QPalette pal = window()->palette();
        if (theme == QChart::ChartThemeLight) {
            pal.setColor(QPalette::Window, QRgb(0xffffff));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        } else if (theme == QChart::ChartThemeDark) {
            pal.setColor(QPalette::Window, QRgb(0x121218));
            pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
        } else if (theme == QChart::ChartThemeBlueCerulean) {
            pal.setColor(QPalette::Window, QRgb(0x40434a));
            pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
        } else if (theme == QChart::ChartThemeBrownSand) {
            pal.setColor(QPalette::Window, QRgb(0x9e8965));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        } else if (theme == QChart::ChartThemeBlueNcs) {
            pal.setColor(QPalette::Window, QRgb(0x018bba));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        } else if (theme == QChart::ChartThemeHighContrast) {
            pal.setColor(QPalette::Window, QRgb(0xffab03));
            pal.setColor(QPalette::WindowText, QRgb(0x181818));
        } else if (theme == QChart::ChartThemeBlueIcy) {
            pal.setColor(QPalette::Window, QRgb(0xcee7f0));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        } else {
            pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        }
        window()->setPalette(pal);
    }

    bool checked = m_antialiasCheckBox->isChecked();
    for (QChartView *chart : charts)
        chart->setRenderHint(QPainter::Antialiasing, checked);

    QChart::AnimationOptions options(
                m_animatedComboBox->itemData(m_animatedComboBox->currentIndex()).toInt());
    if (m_charts.at(0)->chart()->animationOptions() != options) {
        for (QChartView *chartView : charts)
            chartView->chart()->setAnimationOptions(options);
    }

    Qt::Alignment alignment(m_legendComboBox->itemData(m_legendComboBox->currentIndex()).toInt());

    if (!alignment) {
        for (QChartView *chartView : charts)
            chartView->chart()->legend()->hide();
    } else {
        for (QChartView *chartView : charts) {
            chartView->chart()->legend()->setAlignment(alignment);
            chartView->chart()->legend()->show();
        }
    }
}

//编辑表格按钮点击事件
void ThemeWidget::on_tablescreen_clicked()
{
    qDebug()<<"1";
    table->show();
}

//返回主界面按钮点击事件
void ThemeWidget::on_homepage_clicked()
{
    this->close();
    emit backhome();
}
