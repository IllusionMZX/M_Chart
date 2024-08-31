#ifndef AREACHARTWIDGET_H
#define AREACHARTWIDGET_H

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
#include <QtCharts/QAreaSeries>


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


class AreaChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AreaChartWidget(QWidget *parent = nullptr);
    void  prepareData();  //准备数据
    int   getIndexFromX(QXYSeries *series, qreal xValue, qreal tol=0.05);//根据X值返回点的索引

public Q_SLOTS:
    void updateChartSettings();
    void updateSerieSettings();
    void updateLineSettings();
    void showFontDialog();
    void on_download_clicked();
    void remove();

private slots:
    void do_mouseMovePoint(QPoint point); //鼠标移动
    void do_series_clicked(const QPointF &point);   //序列被单击
    void do_series_hovered(const QPointF &point, bool state);   //移入或移出序列

private:
    QComboBox *m_themeComboBox;
    QCheckBox *m_aaCheckBox;
    QCheckBox *m_animationsCheckBox;
    QCheckBox *m_legendCheckBox;

    TChartView *m_chartView;
    QAreaSeries *m_series;
    CustomSlice *m_slice;
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

};



#endif // AREACHARTWIDGET_H
