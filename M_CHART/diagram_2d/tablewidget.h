#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QStandardItemModel>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QPushButton>
#include <QDebug>

#define     COL_NAME       0   //姓名的列编号
#define     COL_MATH       1   //数学的列编号
#define     COL_CHINESE    2   //语文的列编号
#define     COL_ENGLISH    3   //英语的列编号
#define     COL_AVERAGE    4   //平均分的列编号
namespace Ui {
class TableWidget;
}

class TableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TableWidget(QWidget *parent = nullptr);
    ~TableWidget();
    void setTableData(QStandardItemModel  *dataModel);
    void generateDataModel();
    void countData();
    void initCntTable();

    QStandardItemModel  *dataModel;  //数据模型
    QStandardItemModel  *cntdataModel;
private:
    Ui::TableWidget *ui; 
    int studCount=10;

private:
    QToolButton *createToolBtn();
    QPushButton*createPushBtn();
    QStringList headerList;

private slots:
    void on_fileInputbtn_clicked();
    void on_fileOutputbtn_clicked();
    void on_pushButton_clicked();
    void do_calcuAverage(QStandardItem *item);
    void on_spinCount_valueChanged(int arg1);
};

#endif // TABLEWIDGET_H
