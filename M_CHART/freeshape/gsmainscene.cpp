#include "gsmainscene.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QPixmapCache>
#include <QDebug>
#include "gsshapebaseitem.h"

//继承自QGraphicsScene
GsMainScene::GsMainScene(const QSizeF &size, QObject *parent)
    : QGraphicsScene(QRectF(0, 0, size.width(), size.height()), parent)
{
    m_pRootItem = new GsRootItem(size);
    addItem(m_pRootItem);
    QPixmapCache::setCacheLimit(204800);
}

GsMainScene::~GsMainScene()
{
}

//鼠标点击事件
void GsMainScene::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    scenePress(1,e->scenePos());
    QGraphicsScene::mousePressEvent(e);
}

//鼠标移动事件
void GsMainScene::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    sceneMove(1,e->lastScenePos(),e->scenePos());
    QGraphicsScene::mouseMoveEvent(e);
}

//鼠标释放事件
void GsMainScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    sceneRelease(1,e->scenePos());
    QGraphicsScene::mouseReleaseEvent(e);
}

//键盘操作
void GsMainScene::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_1:
        m_pRootItem->setCurType(Shape_Line);
        break;
    case Qt::Key_2:
        m_pRootItem->setCurType(Shape_Rectange);
        break;
    case Qt::Key_3:
        m_pRootItem->setCurType(Shape_Circle);
        break;
    case Qt::Key_4:
        m_pRootItem->setCurType(Shape_Oval);
        break;
    case Qt::Key_Up:
        m_pRootItem->setScale(m_pRootItem->scale() + 0.1);
        break;
    case Qt::Key_Down:
        m_pRootItem->setScale(m_pRootItem->scale() - 0.1);
        break;
    case Qt::Key_Delete:
        m_pRootItem->clear();
        break;
    default:
        break;
    }
}

//图形点击
bool GsMainScene::scenePress(int id, const QPointF &pos)
{
    if(!isHitItem(pos)){
        m_pRootItem->drawPress(id,pos);
    }
    return true;
}

//图形移动
bool GsMainScene::sceneMove(int id, const QPointF &prepos, const QPointF &pos)
{
    m_pRootItem->drawMove(id,prepos,pos);
    return true;
}

//图形释放
bool GsMainScene::sceneRelease(int id, const QPointF &pos)
{
    m_pRootItem->drawRelease(id,pos);
    return true;
}

//多态实现父类指针调用派生类
bool GsMainScene::isHitItem(QPointF p)
{
    QList<QGraphicsItem*> items = this->items(p);

    foreach (QGraphicsItem * item,items) {
        GsShapeBaseItem * i = dynamic_cast<GsShapeBaseItem*>(item);
        if(!i){
            continue;
        }
        if(i->type() == Type_ShapeItem){
            if(i->hasSelected(p)){
                return true;
            }
            return true;
        }
    }
    return false;
}


