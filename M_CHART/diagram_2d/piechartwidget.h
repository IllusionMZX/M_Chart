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
#ifndef PIECHARTWIDGET_H
#define PIECHARTWIDGET_H

#include "tchartview.h"
#include "customslice.h"
#include "pentool.h"
#include "brushtool.h"

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFontDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QCoreApplication>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>

class PieChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PieChartWidget(QStandardItemModel  *dataModel,QWidget *parent = 0);

public Q_SLOTS:
    void updateChartSettings();
    void updateSerieSettings();
    void updateSliceSettings();
    void handleSliceClicked(QPieSlice *slice);
    void showFontDialog();
    void appendSlice();
    void insertSlice();
    void removeSlice();
    void on_download_clicked();

private:
    QComboBox *m_themeComboBox;
    QCheckBox *m_aaCheckBox;
    QCheckBox *m_animationsCheckBox;
    QCheckBox *m_legendCheckBox;

    TChartView *m_chartView;
    QPieSeries *m_series;
    CustomSlice *m_slice;

    QDoubleSpinBox *m_hPosition;
    QDoubleSpinBox *m_vPosition;
    QDoubleSpinBox *m_sizeFactor;
    QDoubleSpinBox *m_startAngle;
    QDoubleSpinBox *m_endAngle;
    QDoubleSpinBox *m_holeSize;

    QLineEdit *m_sliceName;
    QDoubleSpinBox *m_sliceValue;
    QCheckBox *m_sliceLabelVisible;
    QDoubleSpinBox *m_sliceLabelArmFactor;
    QCheckBox *m_sliceExploded;
    QDoubleSpinBox *m_sliceExplodedFactor;
    QPushButton *m_brush;
    BrushTool *m_brushTool;
    QPushButton *m_pen;
    PenTool *m_penTool;
    QPushButton *m_font;
    QPushButton *m_labelBrush;
    QToolButton *m_download;
    QComboBox *m_labelPosition;
    BrushTool *m_labelBrushTool;

    QStandardItemModel  *dataModel;  //数据模型
};

#endif // PIECHARTWIDGET_H
