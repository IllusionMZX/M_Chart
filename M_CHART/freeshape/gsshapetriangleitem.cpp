#include "gsshapetriangleitem.h"
#include <QPainter>


GsShapeTriangleItem::GsShapeTriangleItem(QRectF rectF, QGraphicsObject *parent):
    GsShapeBaseItem(rectF,parent)
{

}

GsShapeTriangleItem::~GsShapeTriangleItem()
{

}

bool GsShapeTriangleItem::hasSelected(QPointF pos)
{
    return shape().contains(pos);
}

GsShapeType GsShapeTriangleItem::shapeType() const
{
    return Shape_Triangle;
}

void GsShapeTriangleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing);
    QColor color = Qt::red;//(rand()%255,rand()%255,rand()%255);
    painter->setBrush(color);
    painter->setPen(QPen(color,3,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
    static const QPointF points[3] = {
          QPointF(m_firstPoint.x(), m_firstPoint.y()),
          QPointF(m_lastPoint.y(), m_firstPoint.x()),
          QPointF(m_firstPoint.y(), m_lastPoint.x()),
      };
    painter->drawPolygon(points,3);
}
