#include "gstempcanvaslayer.h"
#include "gscommondef.h"
#include <QDebug>

GsTempCanvasLayer::GsTempCanvasLayer(const QSizeF &size, QGraphicsObject *parent):
    QGraphicsObject (parent),
    m_size(size)
{
    setCacheMode(DeviceCoordinateCache);
    initCanvas();
}

GsTempCanvasLayer::~GsTempCanvasLayer()
{
    SAFE_DELETE(m_pTempPainter);
    SAFE_DELETE(m_pTempCanvas);
}

//清除画布
void GsTempCanvasLayer::clear()
{
    m_pTempCanvas->fill(Qt::transparent);
    this->update();
}

//边框矩形
QRectF GsTempCanvasLayer::boundingRect() const
{
    return  QRectF(0,0,m_size.width(),m_size.height());
}

//painter画出
void GsTempCanvasLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawImage(0,0,*m_pTempCanvas);
}

//绘制不同的图形
void GsTempCanvasLayer::drawShape(GsShapeType type, QPointF p1, QPointF p2)
{
    m_pTempCanvas->fill(Qt::transparent);
    m_pTempPainter->setRenderHint(QPainter::Antialiasing, true);
    m_pTempPainter->setCompositionMode(QPainter::CompositionMode_Source);
    m_pTempPainter->setPen(QPen(Qt::black,3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    switch(type){
    case Shape_Line:
        m_pTempPainter->drawLine(p1,p2);
        break;
    case Shape_Rectange:
        m_pTempPainter->drawRect(QRectF(p1,p2));
        break;
    case Shape_Circle:
        m_pTempPainter->drawEllipse(p1.x(),p1.y(),
                             qAbs(p2.y() - p1.y()),
                             qAbs(p2.y() - p1.y()));
        break;
    case Shape_Oval:
        m_pTempPainter->drawEllipse(QRectF(p1,p2));
        break;
    default:
        break;
    }
    update();
}

//初始化临时画布
void GsTempCanvasLayer::initCanvas()
{
    m_pTempCanvas = new QImage(m_size.toSize(),QImage::Format_ARGB32_Premultiplied);
    m_pTempCanvas->fill(Qt::transparent);
    m_pTempPainter = new QPainter(m_pTempCanvas);
}
