#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include "diagram_2d/tablewidget.h"
#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QToolButton>

QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE


typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;

QT_CHARTS_USE_NAMESPACE

class ThemeWidget: public QWidget
{
    Q_OBJECT
public:
    explicit ThemeWidget(QWidget *parent = 0);
    ~ThemeWidget();
    void closeEvent(QCloseEvent *);

private Q_SLOTS:
    void updateUI();

private slots:
    void on_tablescreen_clicked();
    void on_homepage_clicked();

private:
    DataTable generateRandomData(int listCount, int valueMax, int valueCount) const;
    QComboBox *createThemeBox() const;
    QComboBox *createAnimationBox() const;
    QComboBox *createLegendBox() const;
    QTableView*createtable();
    QToolButton*createsToolBtn();
    void connectSignals();
    QChart *createAreaChart() const;
    QChart *createBarChart(int valueCount) const;
    QChart *createPieChart() const;
    QChart *createLineChart() const;
    QChart *createSplineChart() const;
    QChart *createScatterChart() const;



private:
    int m_listCount;
    int m_valueMax;
    int m_valueCount;
    QList<QChartView *> m_charts;
    DataTable m_dataTable;

    QComboBox *m_themeComboBox;
    QCheckBox *m_antialiasCheckBox;
    QComboBox *m_animatedComboBox;
    QComboBox *m_legendComboBox;

    QToolButton* tablescreen;
    QToolButton* homepage;
    QToolButton* helpbtn;
    QPoint selectPoint;
    TableWidget*table;

protected:
    void mousePressEvent(QMouseEvent *event);   //鼠标左键按下

signals:
    void backhome();
};

#endif /* THEMEWIDGET_H */
