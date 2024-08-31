#include "qwgraphicsview.h"

#include    <QMouseEvent>
#include    <QPoint>

//鼠标移动事件
void QWGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QPoint point=event->pos(); //QGraphicsView的坐标
    emit mouseMovePoint(point); //释放信号
    QGraphicsView::mouseMoveEvent(event);
}

//鼠标左键按下事件
void QWGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton)
    {
        QPoint point=event->pos(); //QGraphicsView的坐标
        emit mouseClicked(point);//释放信号
    }
    QGraphicsView::mousePressEvent(event);
}

//鼠标双击事件
void QWGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton)
    {
        QPoint point=event->pos(); //QGraphicsView的坐标
        emit mouseDoubleClick(point);//释放信号
    }
    QGraphicsView::mouseDoubleClickEvent(event);
}

//按键事件
void QWGraphicsView::keyPressEvent(QKeyEvent *event)
{
    emit keyPress(event);
    QGraphicsView::keyPressEvent(event);
}

QWGraphicsView::QWGraphicsView(QWidget *parent):QGraphicsView(parent)
{

}
