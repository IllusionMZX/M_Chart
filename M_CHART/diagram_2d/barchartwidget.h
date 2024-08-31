#ifndef BARCHARTWIDGET_H
#define BARCHARTWIDGET_H

#include "tchartview.h"

#include <QWidget>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFontDialog>
#include <QtCharts/QChartView>
#include <QtCharts>
#include <QLabel>
#include <QtWidgets/QStatusBar>
#include <QtCharts/QChartGlobal>
#include <QPoint>
#include <QMouseEvent>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
QT_END_NAMESPACE

class PenTool;
class BrushTool;
class CustomSlice;


class BarChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BarChartWidget(QStandardItemModel  *dataModel,QWidget *parent = nullptr);
    void  prepareData();  //准备数据
    //void setDataModel(QStandardItemModel  *dataModel);
public Q_SLOTS:
    void updateChartSettings();
    void updateSerieSettings();
    void showFontDialog();
    void on_download_clicked();
    void remove();
private slots:

    void do_barHovered(bool status, int index, QBarSet *barset);
    void do_barClicked(int index, QBarSet *barset);
    void do_legendMarkerClicked();

private:
    QComboBox *m_themeComboBox;
    QCheckBox *m_aaCheckBox;
    QCheckBox *m_animationsCheckBox;
    QCheckBox *m_legendCheckBox;

    TChartView *m_chartView;
    QBarSeries *m_series;

    CustomSlice *m_slice;
    QBarSet*m_set;
    QDoubleSpinBox *m_AxissizeFactor; //坐标轴缩放

    QStatusBar*statusBar;
    QLabel  *lab_chartXY;  //状态栏上的标签
    QLabel  *lab_hoverXY;
    QLabel  *lab_clickXY;
    QPointF  pointmove;
    QLineEdit *m_lineName;
    QCheckBox *m_lineLabelVisible;
    QPushButton *m_brush;
    BrushTool *m_brushTool;
    QPushButton *m_pen;
    PenTool *m_penTool;
    QPushButton *m_font;
    QPushButton *m_labelBrush;
    QToolButton *m_download;
    BrushTool *m_labelBrushTool;
    QChart* m_chart;


    QStandardItemModel  *dataModel;  //数据模型
    void removeAllAxis(QChart *chart);
};



#endif // BARCHARTWIDGET_H
