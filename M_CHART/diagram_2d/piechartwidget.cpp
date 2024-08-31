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
#include "piechartwidget.h"

QT_CHARTS_USE_NAMESPACE

PieChartWidget::PieChartWidget(QStandardItemModel  *dataModel,QWidget *parent)
    : QWidget(parent),
      m_slice(0),
      dataModel(dataModel)
{
    this->setMouseTracking(true);
    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);
    // create chart
    QChart *chart = new QChart;
    chart->setTitle("Piechart 演示");
    chart->setAnimationOptions(QChart::AllAnimations);


    // create series
    m_series = new QPieSeries();
    for(int i=0;i<dataModel->columnCount();i++){
        *m_series << new CustomSlice("Slice "+QString::number(i+ 1), dataModel->item(0,i)->text().toInt());
    }

    m_series->setLabelsVisible();
    chart->addSeries(m_series);


    connect(m_series, &QPieSeries::clicked, this, &PieChartWidget::handleSliceClicked);

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

    //connect
    connect(m_themeComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &PieChartWidget::updateChartSettings);
    connect(m_aaCheckBox, &QCheckBox::toggled, this, &PieChartWidget::updateChartSettings);
    connect(m_animationsCheckBox, &QCheckBox::toggled, this, &PieChartWidget::updateChartSettings);
    connect(m_legendCheckBox, &QCheckBox::toggled, this, &PieChartWidget::updateChartSettings);

    // series settings
    m_hPosition = new QDoubleSpinBox();
    m_hPosition->setMinimum(0.0);
    m_hPosition->setMaximum(1.0);
    m_hPosition->setSingleStep(0.1);
    m_hPosition->setValue(m_series->horizontalPosition());

    m_vPosition = new QDoubleSpinBox();
    m_vPosition->setMinimum(0.0);
    m_vPosition->setMaximum(1.0);
    m_vPosition->setSingleStep(0.1);
    m_vPosition->setValue(m_series->verticalPosition());

    m_sizeFactor = new QDoubleSpinBox();
    m_sizeFactor->setMinimum(0.0);
    m_sizeFactor->setMaximum(1.0);
    m_sizeFactor->setSingleStep(0.1);
    m_sizeFactor->setValue(m_series->pieSize());

    m_startAngle = new QDoubleSpinBox();
    m_startAngle->setMinimum(-720);
    m_startAngle->setMaximum(720);
    m_startAngle->setValue(m_series->pieStartAngle());
    m_startAngle->setSingleStep(1);

    m_endAngle = new QDoubleSpinBox();
    m_endAngle->setMinimum(-720);
    m_endAngle->setMaximum(720);
    m_endAngle->setValue(m_series->pieEndAngle());
    m_endAngle->setSingleStep(1);

    m_holeSize = new QDoubleSpinBox();
    m_holeSize->setMinimum(0.0);
    m_holeSize->setMaximum(1.0);
    m_holeSize->setSingleStep(0.1);
    m_holeSize->setValue(m_series->holeSize());

    QPushButton *appendSlice = new QPushButton("添加分块");
    QPushButton *insertSlice = new QPushButton("插入分块");
    QPushButton *removeSlice = new QPushButton("删除选中分块");

    QFormLayout *seriesSettingsLayout = new QFormLayout();
    seriesSettingsLayout->addRow("水平位置", m_hPosition);
    seriesSettingsLayout->addRow("垂直位置", m_vPosition);
    seriesSettingsLayout->addRow("缩放比例", m_sizeFactor);
    seriesSettingsLayout->addRow("起始角", m_startAngle);
    seriesSettingsLayout->addRow("终止角", m_endAngle);
    seriesSettingsLayout->addRow("孔洞尺寸", m_holeSize);
    seriesSettingsLayout->addRow(appendSlice);
    seriesSettingsLayout->addRow(insertSlice);
    seriesSettingsLayout->addRow(removeSlice);
    QGroupBox *seriesSettings = new QGroupBox("序列");
    seriesSettings->setLayout(seriesSettingsLayout);

    connect(m_vPosition,
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &PieChartWidget::updateSerieSettings);
    connect(m_hPosition,
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &PieChartWidget::updateSerieSettings);
    connect(m_sizeFactor,
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &PieChartWidget::updateSerieSettings);
    connect(m_startAngle,
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &PieChartWidget::updateSerieSettings);
    connect(m_endAngle,
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &PieChartWidget::updateSerieSettings);
    connect(m_holeSize,
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &PieChartWidget::updateSerieSettings);
    connect(appendSlice, &QPushButton::clicked, this, &PieChartWidget::appendSlice);
    connect(insertSlice, &QPushButton::clicked, this, &PieChartWidget::insertSlice);
    connect(removeSlice, &QPushButton::clicked, this, &PieChartWidget::removeSlice);

    // slice settings
    m_sliceName = new QLineEdit("<选择一个分块>");
    m_sliceName->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    m_sliceValue = new QDoubleSpinBox();
    m_sliceValue->setMaximum(1000);
    m_sliceLabelVisible = new QCheckBox();
    m_sliceLabelArmFactor = new QDoubleSpinBox();
    m_sliceLabelArmFactor->setSingleStep(0.01);
    m_sliceExploded = new QCheckBox();
    m_sliceExplodedFactor = new QDoubleSpinBox();
    m_sliceExplodedFactor->setSingleStep(0.01);
    m_pen = new QPushButton();
    m_penTool = new PenTool("Slice pen", this);
    m_brush = new QPushButton();
    m_brushTool = new BrushTool("Slice brush", this);
    m_font = new QPushButton();
    m_labelBrush = new QPushButton();
    m_labelBrushTool = new BrushTool("标签颜色", this);
    m_labelPosition = new QComboBox(this);
    m_labelPosition->addItem("Outside", QPieSlice::LabelOutside);
    m_labelPosition->addItem("Inside horizontal", QPieSlice::LabelInsideHorizontal);
    m_labelPosition->addItem("Inside tangential", QPieSlice::LabelInsideTangential);
    m_labelPosition->addItem("Inside normal", QPieSlice::LabelInsideNormal);

    QFormLayout *sliceSettingsLayout = new QFormLayout();
    sliceSettingsLayout->addRow("标签名", m_sliceName);
    sliceSettingsLayout->addRow("数值", m_sliceValue);
    sliceSettingsLayout->addRow("分块边框", m_pen);
    sliceSettingsLayout->addRow("填充色", m_brush);
    sliceSettingsLayout->addRow("标签可见性", m_sliceLabelVisible);
    sliceSettingsLayout->addRow("标签字体", m_font);
    sliceSettingsLayout->addRow("标签颜色", m_labelBrush);
    sliceSettingsLayout->addRow("标签位置", m_labelPosition);
    sliceSettingsLayout->addRow("标签与分块间距", m_sliceLabelArmFactor);
    sliceSettingsLayout->addRow("分块弹出", m_sliceExploded);
    sliceSettingsLayout->addRow("分块弹出距离", m_sliceExplodedFactor);
    QGroupBox *sliceSettings = new QGroupBox("选中的分块");
    sliceSettings->setLayout(sliceSettingsLayout);

    connect(m_sliceName, &QLineEdit::textChanged, this, &PieChartWidget::updateSliceSettings);
    connect(m_sliceValue,
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &PieChartWidget::updateSliceSettings);
    connect(m_pen, &QPushButton::clicked, m_penTool, &PenTool::show);
    connect(m_penTool, &PenTool::changed, this, &PieChartWidget::updateSliceSettings);
    connect(m_brush, &QPushButton::clicked, m_brushTool, &BrushTool::show);
    connect(m_brushTool, &BrushTool::changed, this, &PieChartWidget::updateSliceSettings);
    connect(m_font, &QPushButton::clicked, this, &PieChartWidget::showFontDialog);
    connect(m_labelBrush, &QPushButton::clicked, m_labelBrushTool, &BrushTool::show);
    connect(m_labelBrushTool, &BrushTool::changed, this, &PieChartWidget::updateSliceSettings);
    connect(m_sliceLabelVisible, &QCheckBox::toggled, this, &PieChartWidget::updateSliceSettings);
    connect(m_sliceLabelVisible, &QCheckBox::toggled, this, &PieChartWidget::updateSliceSettings);
    connect(m_sliceLabelArmFactor,
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &PieChartWidget::updateSliceSettings);
    connect(m_sliceExploded, &QCheckBox::toggled, this, &PieChartWidget::updateSliceSettings);
    connect(m_sliceExplodedFactor,
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &PieChartWidget::updateSliceSettings);
    connect(m_labelPosition,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &PieChartWidget::updateSliceSettings);

    // create chart view
    m_chartView = new TChartView();
    m_chartView->setChart(chart);

    // create main layout
    QVBoxLayout *settingsLayout = new QVBoxLayout();
    settingsLayout->addWidget(chartSettings);
    settingsLayout->addWidget(seriesSettings);
    settingsLayout->addWidget(sliceSettings);
    settingsLayout->addStretch();

    m_download=new QToolButton();
    m_download->setIconSize(QSize(30,30));
    m_download->setIcon(QIcon(":/images/image/download.png"));
    connect(m_download,SIGNAL(clicked()),this,SLOT(on_download_clicked()));

    QGridLayout *baseLayout = new QGridLayout();
    baseLayout->addLayout(settingsLayout, 0, 0);

    baseLayout->addWidget(m_chartView, 0, 1);
    baseLayout->addWidget(m_download,0,2);
    setLayout(baseLayout);

    updateSerieSettings();
    updateChartSettings();
}

//更新图表设置
void PieChartWidget::updateChartSettings()
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
}
//更新饼图设置
void PieChartWidget::updateSerieSettings()
{
    m_series->setHorizontalPosition(m_hPosition->value());
    m_series->setVerticalPosition(m_vPosition->value());
    m_series->setPieSize(m_sizeFactor->value());
    m_holeSize->setMaximum(m_sizeFactor->value());
    m_series->setPieStartAngle(m_startAngle->value());
    m_series->setPieEndAngle(m_endAngle->value());
    m_series->setHoleSize(m_holeSize->value());
}
//更新饼图分块设置
void PieChartWidget::updateSliceSettings()
{
    if (!m_slice)
        return;

    m_slice->setLabel(m_sliceName->text());

    m_slice->setValue(m_sliceValue->value());

    m_slice->setPen(m_penTool->pen());
    m_slice->setBrush(m_brushTool->brush());

    m_slice->setLabelBrush(m_labelBrushTool->brush());
    m_slice->setLabelVisible(m_sliceLabelVisible->isChecked());
    m_slice->setLabelArmLengthFactor(m_sliceLabelArmFactor->value());
    // We assume that label position index is in sync with the enum
    m_slice->setLabelPosition((QPieSlice::LabelPosition)m_labelPosition->currentIndex());

    m_slice->setExploded(m_sliceExploded->isChecked());
    m_slice->setExplodeDistanceFactor(m_sliceExplodedFactor->value());
}
//处理分块点击事件
void PieChartWidget::handleSliceClicked(QPieSlice *slice)
{
    m_slice = static_cast<CustomSlice *>(slice);

    // name
    m_sliceName->blockSignals(true);
    m_sliceName->setText(slice->label());
    m_sliceName->blockSignals(false);

    // value
    m_sliceValue->blockSignals(true);
    m_sliceValue->setValue(slice->value());
    m_sliceValue->blockSignals(false);

    // pen
    m_pen->setText(PenTool::name(m_slice->pen()));
    m_penTool->setPen(m_slice->pen());

    // brush
    m_brush->setText(m_slice->originalBrush().color().name());
    m_brushTool->setBrush(m_slice->originalBrush());

    // label
    m_labelBrush->setText(BrushTool::name(m_slice->labelBrush()));
    m_labelBrushTool->setBrush(m_slice->labelBrush());
    m_font->setText(slice->labelFont().toString());
    m_sliceLabelVisible->blockSignals(true);
    m_sliceLabelVisible->setChecked(slice->isLabelVisible());
    m_sliceLabelVisible->blockSignals(false);
    m_sliceLabelArmFactor->blockSignals(true);
    m_sliceLabelArmFactor->setValue(slice->labelArmLengthFactor());
    m_sliceLabelArmFactor->blockSignals(false);
    m_labelPosition->blockSignals(true);
    // We assume that label position index is in sync with the enum
    m_labelPosition->setCurrentIndex(slice->labelPosition());
    m_labelPosition->blockSignals(false);

    // exploded
    m_sliceExploded->blockSignals(true);
    m_sliceExploded->setChecked(slice->isExploded());
    m_sliceExploded->blockSignals(false);
    m_sliceExplodedFactor->blockSignals(true);
    m_sliceExplodedFactor->setValue(slice->explodeDistanceFactor());
    m_sliceExplodedFactor->blockSignals(false);
}
//展示设置字体样式对话框
void PieChartWidget::showFontDialog()
{
    if (!m_slice)
        return;

    QFontDialog dialog(m_slice->labelFont());
    dialog.show();
    dialog.exec();

    m_slice->setLabelFont(dialog.currentFont());
    m_font->setText(dialog.currentFont().toString());
}
//添加分块
void PieChartWidget::appendSlice()
{
    *m_series << new CustomSlice("Slice " + QString::number(m_series->count() + 1), 10.0);
}
//插入分块
void PieChartWidget::insertSlice()
{
    if (!m_slice)
        return;

    int i = m_series->slices().indexOf(m_slice);

    m_series->insert(i, new CustomSlice("Slice " + QString::number(m_series->count() + 1), 10.0));
}
//删除分块
void PieChartWidget::removeSlice()
{
    if (!m_slice)
        return;

    m_sliceName->setText("<click a slice>");

    m_series->remove(m_slice);
    m_slice = 0;
}
//下载按钮点击事件
void PieChartWidget::on_download_clicked()
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

