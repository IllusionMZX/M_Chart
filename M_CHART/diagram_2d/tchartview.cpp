#include    "tchartview.h"
#include    <QChartView>

//鼠标左键按下
void TChartView::mousePressEvent(QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton)
        beginPoint=event->pos();
    QChartView::mousePressEvent(event);
}

//鼠标左键释放
void TChartView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton)
    {
        endPoint=event->pos();

        if ((this->dragMode() ==QGraphicsView::ScrollHandDrag)
            &&(this->rubberBand()==QChartView::NoRubberBand))   //drag
            chart()->scroll(beginPoint.x()-endPoint.x(), endPoint.y() - beginPoint.y());
        else if (m_customZoom && this->dragMode() ==QGraphicsView::RubberBandDrag)   //zoom
        {
            QRectF  rectF;
            rectF.setTopLeft(beginPoint);
            rectF.setBottomRight(endPoint);
            this->chart()->zoomIn(rectF);   //按矩形框放大
        }
    }

    QChartView::mouseReleaseEvent(event);

}

//鼠标移动事件
void TChartView::mouseMoveEvent(QMouseEvent *event)
{
    QPoint point=event->pos();
    emit mouseMovePoint(point);     //发射信号
    QChartView::mouseMoveEvent(event);  //父类继续处理事件
}

//按键控制
void TChartView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
        chart()->scroll(10, 0);
        break;
    case Qt::Key_Right:
        chart()->scroll(-10, 0);
        break;
    case Qt::Key_Up:
        chart()->scroll(0, -10);
        break;
    case Qt::Key_Down:
        chart()->scroll(0, 10);
        break;
    case Qt::Key_PageUp:
        chart()->scroll(0, -50);
        break;
    case Qt::Key_PageDown:
        chart()->scroll(0, 50);
        break;
    case Qt::Key_Escape:
        chart()->zoomReset();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

//鼠标滚轮事件处理，缩放
void TChartView::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;
    if (!numDegrees.isNull())
    {
        QPoint numSteps = numDegrees / 15;  //步数
        int stepY=numSteps.y();     //垂直方向滚轮的滚动步数
        if (stepY >0)       //大于0，前向滚动，放大
            chart()->zoom(1.1*stepY);
        else
            chart()->zoom(-0.9*stepY);
    }
    event->accept();
}

TChartView::TChartView(QWidget *parent):QChartView(parent)
{
    this->setMouseTracking(true);   //必须开启此功能，默认是false
    this->setDragMode(QGraphicsView::NoDrag);
    this->setRubberBand(QChartView::NoRubberBand);   //设置为矩形选择方式

}

TChartView::~TChartView()
{

}

void TChartView::setCustomZoomRect(bool custom)
{
    m_customZoom=custom;
}
