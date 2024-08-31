#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#define Pi 3.1415926
#include <QDesktopWidget>
#include "freeshape/gsmainscene.h"
#include <QWidget>
#include <QLabel>
#include <QGraphicsScene>
#include "qwgraphicsview.h"
#include <QtWidgets/QStatusBar>
#include <QBrush>
#include <QColorDialog>
#include <QPen>

#include "pentool.h"
#include "brushtool.h"

namespace Ui {
class DrawWidget;
}

class DrawWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DrawWidget(QWidget *parent = nullptr);
    ~DrawWidget();
    void closeEvent(QCloseEvent *);

private slots:
    void on_toolButton_clicked();

private slots:
    void    on_mouseMovePoint(QPoint point); //鼠标移动

    void    on_mouseClicked(QPoint point); //鼠标单击

    void    on_mouseDoubleClick(QPoint point); //鼠标双击

    void    on_keyPress(QKeyEvent *event); //按键

    void on_rectbtn_clicked();

    void on_ovalbtn_clicked();

    void on_trapbtn_clicked();

    void on_clearbtn_clicked();

    void on_linebtn_clicked();

    void on_textbtn_clicked();

    void on_circlebtn_clicked();

    void on_trianglebtn_clicked();

    void on_square_clicked();

    void on_para_clicked();

    void on_drawbtn_clicked();

    void on_homepagebtn_clicked();

    void on_piebtn_clicked();

    void on_starbtn_clicked();

signals:
    void backhome();

private:
    Ui::DrawWidget *ui;
    PenTool*pen;
    QStatusBar*statusBar;

private:
    static const int ItemId = 1;   //绘图项自定义数据的key
    static const int ItemDesciption = 2;   //绘图项自定义数据的key

    int seqNum=0;
    int backZ=0;
    int frontZ=0;

    QGraphicsScene  *scene;
    QLabel  *labViewCord;
    QLabel  *labSceneCord;
    QLabel  *labItemCord;
    QLabel  *labItemInfo;
};

#endif // DRAWWIDGET_H
