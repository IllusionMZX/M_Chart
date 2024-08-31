#include "drawwidget.h"
#include "ui_drawwidget.h"

#include    <QGraphicsRectItem>
#include    <QInputDialog>
#include    <QColorDialog>
#include    <QFontDialog>
#include    <QTime>
#include    <QKeyEvent>
#include    <QtMath>
#include    <QMessageBox>

//函数模板
template<class T> void setBrushColor(T *item)
{
    QColor color=item->brush().color();
    color=QColorDialog::getColor(color,NULL,"选择填充颜色");
    if (color.isValid())
        item->setBrush(QBrush(color));
}

DrawWidget::DrawWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrawWidget),
    pen(new PenTool("画笔")),
    statusBar(new QStatusBar())
{
    this->setMouseTracking(true);
    ui->setupUi(this);
    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);
    labViewCord=new QLabel("View 坐标："); //创建状态栏标签
    labViewCord->setMinimumWidth(150);

    statusBar->addWidget(labViewCord);

    labSceneCord=new QLabel("Scene 坐标：");
    labSceneCord->setMinimumWidth(150);
    statusBar->addWidget(labSceneCord);

    labItemCord=new QLabel("Item 坐标：");
    labItemCord->setMinimumWidth(150);
    statusBar->addWidget(labItemCord);

    labItemInfo=new QLabel("ItemInfo: ");
    labItemInfo->setMinimumWidth(200);
    statusBar->addWidget(labItemInfo);

    QSize desktopSize = qApp->desktop()->screenGeometry().size();
    scene=new QGraphicsScene(-300,-200,600,200); //创建QGraphicsScene
    ui->graphicsView->setScene(scene);
    ui->graphicsView->resize(desktopSize);
    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);


    QObject::connect(ui->graphicsView,SIGNAL(mouseMovePoint(QPoint)),
                     this, SLOT(on_mouseMovePoint(QPoint)));

    QObject::connect(ui->graphicsView,SIGNAL(mouseClicked(QPoint)),
                     this, SLOT(on_mouseClicked(QPoint)));

    QObject::connect(ui->graphicsView,SIGNAL(mouseDoubleClick(QPoint)),
                     this, SLOT(on_mouseDoubleClick(QPoint)));

    QObject::connect(ui->graphicsView,SIGNAL(keyPress(QKeyEvent*)),
                     this, SLOT(on_keyPress(QKeyEvent*)));

    qsrand(QTime::currentTime().second());
}

DrawWidget::~DrawWidget()
{
    delete ui;
}

//重写关闭按钮事件
void DrawWidget::closeEvent(QCloseEvent *e)
{
    //窗口关闭时弹出的提示窗口
    QMessageBox msgBox;
    msgBox.setPalette(Qt::white);
    msgBox.setText("提示");
    msgBox.setInformativeText("确认退出?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok){
        //若用户点击确认，则接收这个事件,当前窗口会关闭
        emit backhome();
        e->accept();
    }else{
        //若用户点击取消，则忽略这个事件，当前窗口不会关闭
        e->ignore();
    }
}

//图形生成模式
void DrawWidget::on_toolButton_clicked()
{
    QSize desktopSize = qApp->desktop()->screenGeometry().size();
    scene=new QGraphicsScene(-300,-200,600,200); //创建QGraphicsScene
    ui->graphicsView->setScene(scene);
    ui->graphicsView->resize(desktopSize);
    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    ui->graphicsView->setCursor(Qt::ArrowCursor);
}

//鼠标移动
void DrawWidget::on_mouseMovePoint(QPoint point)
{//鼠标移动事件，point是 GraphicsView的坐标,物理坐标
    labViewCord->setText(QString::asprintf("View 坐标：%d,%d",point.x(),point.y()));
    QPointF pointScene=ui->graphicsView->mapToScene(point); //转换到Scene坐标
    labSceneCord->setText(QString::asprintf("Scene 坐标：%.0f,%.0f",pointScene.x(),pointScene.y()));
}

//鼠标单击
void DrawWidget::on_mouseClicked(QPoint point)
{//鼠标单击事件
    QPointF pointScene=ui->graphicsView->mapToScene(point); //转换到Scene坐标
    QGraphicsItem  *item=NULL;
    item=scene->itemAt(pointScene,ui->graphicsView->transform()); //获取光标下的绘图项
    if (item != NULL) //有绘图项
    {
        QPointF pointItem=item->mapFromScene(pointScene); //转换为绘图项的局部坐标
        labItemCord->setText(QString::asprintf("Item 坐标：%.0f,%.0f",pointItem.x(),pointItem.y()));
        labItemInfo->setText(item->data(ItemDesciption).toString()+", ItemId="+
                   item->data(ItemId).toString());
    }
}

//鼠标双击
void DrawWidget::on_mouseDoubleClick(QPoint point)
{//鼠标双击事件,调用相应的对话框，设置填充颜色、线条颜色或字体
    QPointF pointScene=ui->graphicsView->mapToScene(point); //转换到Scene坐标
    QGraphicsItem  *item=NULL;
    item=scene->itemAt(pointScene,ui->graphicsView->transform()); //获取光标下的绘图项

    if (item == NULL) //没有绘图项
        return;

    switch (item->type())  //绘图项的类型
    {
      case    QGraphicsRectItem::Type: //矩形框
      { //强制类型转换
        QGraphicsRectItem *theItem=qgraphicsitem_cast<QGraphicsRectItem*>(item);
        setBrushColor(theItem);
        break;
      }
      case    QGraphicsEllipseItem::Type: //椭圆和圆都是 QGraphicsEllipseItem
      {
        QGraphicsEllipseItem *theItem;
        theItem=qgraphicsitem_cast<QGraphicsEllipseItem*>(item);
        setBrushColor(theItem);
        break;
      }

      case    QGraphicsPolygonItem::Type: //梯形和三角形
      {
        QGraphicsPolygonItem *theItem=qgraphicsitem_cast<QGraphicsPolygonItem*>(item);
        setBrushColor(theItem);
        break;
      }
      case    QGraphicsLineItem::Type: //直线，设置线条颜色
      {
        QGraphicsLineItem *theItem=qgraphicsitem_cast<QGraphicsLineItem*>(item);
        QPen    pen=theItem->pen();
        QColor  color=theItem->pen().color();
        color=QColorDialog::getColor(color,this,"选择线条颜色");
        if (color.isValid())
        {
            pen.setColor(color);
            theItem->setPen(pen);
        }
        break;
      }
      case    QGraphicsPathItem::Type: //路径作图，设置线条颜色
      {
        QGraphicsPathItem *theItem=qgraphicsitem_cast<QGraphicsPathItem*>(item);
        setBrushColor(theItem);
        break;
        }
      case    QGraphicsTextItem::Type: //文字，设置字体
      {
        QGraphicsTextItem *theItem=qgraphicsitem_cast<QGraphicsTextItem*>(item);
        QFont font=theItem->font();
        bool ok=false;
        font=QFontDialog::getFont(&ok,font,this,"设置字体");
        if (ok)
            theItem->setFont(font);
        break;
      }
    }
}

//键盘操作
void DrawWidget::on_keyPress(QKeyEvent *event)
{ //按键事件
    if (scene->selectedItems().count()!=1)
        return; //没有选中的绘图项，或选中的多于1个
    QGraphicsItem   *item=scene->selectedItems().at(0);

    if (event->key()==Qt::Key_Delete)//删除
       scene->removeItem(item);
    else if (event->key()==Qt::Key_Space) //顺时针旋转90度
       item->setRotation(30+item->rotation());
    else if (event->key()==Qt::Key_Plus)//放大
        item->setScale(0.1+item->scale());
    else if (event->key()==Qt::Key_Minus) //缩小
        item->setScale(-0.1+item->scale());
    else if (event->key()==Qt::Key_Left)  //左移
        item->setX(-1+item->x());
    else if (event->key()==Qt::Key_Right) //右移
        item->setX(1+item->x());
    else if (event->key()==Qt::Key_Up) //上移
        item->setY(-1+item->y());
    else if (event->key()==Qt::Key_Down) //下移
        item->setY(1+item->y());
}

//生成矩形
void DrawWidget::on_rectbtn_clicked()
{
    //添加一个矩形
    QGraphicsRectItem   *item=new QGraphicsRectItem(-50,-25,100,50);//x,y 为左上角的图元局部坐标，图元中心点为0,0
    item->setFlags(QGraphicsItem::ItemIsMovable
                   | QGraphicsItem::ItemIsSelectable
                   | QGraphicsItem::ItemIsFocusable);
    item->setBrush(QBrush(0xbab1a7));
    item->setZValue(++frontZ);

    item->setPos(-50+(qrand() % 100),-50+(qrand() % 100));

    item->setData(ItemId,++seqNum);
    item->setData(ItemDesciption,"矩形");

    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}

//生成椭圆
void DrawWidget::on_ovalbtn_clicked()
{
    //添加一个椭圆
        QGraphicsEllipseItem   *item=new QGraphicsEllipseItem(-50,-30,100,60);
        item->setFlags(QGraphicsItem::ItemIsMovable
                       | QGraphicsItem::ItemIsSelectable
                       | QGraphicsItem::ItemIsFocusable);
        item->setBrush(QBrush(0x939391)); //填充颜色
        item->setZValue(++frontZ); //用于叠放顺序
        item->setPos(-50+(qrand() % 100),-50+(qrand() % 100)); //初始位置, qrand()函数过时

        item->setData(ItemId,++seqNum);  //自定义数据，ItemId键
        item->setData(ItemDesciption,"椭圆"); //自定义数据，ItemDesciption键

        scene->addItem(item);
        scene->clearSelection();
        item->setSelected(true);
}

//生成梯形
void DrawWidget::on_trapbtn_clicked()
{
    //添加一个梯形
        QGraphicsPolygonItem   *item=new QGraphicsPolygonItem;

        QPolygonF   points;
        points.append(QPointF(-40,-40));
        points.append(QPointF(40,-40));
        points.append(QPointF(100,40));
        points.append(QPointF(-100,40));
        item->setPolygon(points);
        item->setPos(-50+(qrand() % 100),-50+(qrand() % 100));

        item->setFlags(QGraphicsItem::ItemIsMovable
                       | QGraphicsItem::ItemIsSelectable
                       | QGraphicsItem::ItemIsFocusable);
        item->setBrush(QBrush(0x7d8b72));
        item->setZValue(++frontZ);

        item->setData(ItemId,++seqNum);
        item->setData(ItemDesciption,"梯形");

        scene->addItem(item);
        scene->clearSelection();
        item->setSelected(true);
}

//清空画布
void DrawWidget::on_clearbtn_clicked()
{
    //删除所有选中的绘图项
    int cnt=scene->selectedItems().count();
    if (cnt>0)
    for (int i=0;i<cnt;i++)
    {
        QGraphicsItem*  item=scene->selectedItems().at(0);
        scene->removeItem(item); //删除绘图项
    }
}

//生成直线
void DrawWidget::on_linebtn_clicked()
{
    //添加直线
        QGraphicsLineItem   *item=new QGraphicsLineItem(-100,0,100,0);//x,y 为左上角的图元局部坐标，图元中心点为0,0
        item->setFlags(QGraphicsItem::ItemIsMovable
                       | QGraphicsItem::ItemIsSelectable
                       | QGraphicsItem::ItemIsFocusable);

        QPen    pen(0x945657);
        pen.setWidth(3);
        item->setPen(pen);

        item->setZValue(++frontZ);
        item->setPos(-50+(qrand() % 100),-50+(qrand() % 100));

        item->setData(ItemId,++seqNum);
        item->setData(ItemDesciption,"直线");

        scene->addItem(item);
        scene->clearSelection();
        item->setSelected(true);
}

//生成文本
void DrawWidget::on_textbtn_clicked()
{
    //添加文字
        QString str=QInputDialog::getText(this,"输入文字","请输入文字");
        if (str.isEmpty())
            return;

        QGraphicsTextItem   *item=new QGraphicsTextItem(str);

        QFont   font=this->font();
        font.setPointSize(20);
        font.setBold(true);
        item->setFont(font);

        item->setFlags(QGraphicsItem::ItemIsMovable
                       | QGraphicsItem::ItemIsSelectable
                       | QGraphicsItem::ItemIsFocusable);
        item->setPos(-50+(qrand() % 100),-50+(qrand() % 100));


        item->setZValue(++frontZ);

        item->setData(ItemId,++seqNum);
        item->setData(ItemDesciption,"文字");

        scene->addItem(item);
        scene->clearSelection();
        item->setSelected(true);
}

//生成圆形
void DrawWidget::on_circlebtn_clicked()
{
    //添加圆形
        QGraphicsEllipseItem   *item=new QGraphicsEllipseItem(-50,-50,100,100);
        item->setFlags(QGraphicsItem::ItemIsMovable
                       | QGraphicsItem::ItemIsSelectable
                       | QGraphicsItem::ItemIsFocusable);
        item->setBrush(QBrush(Qt::green));
        item->setZValue(++frontZ);
        item->setPos(-50+(qrand() % 100),-50+(qrand() % 100));


        item->setData(ItemId,++seqNum);
        item->setData(ItemDesciption,"圆形");

        scene->addItem(item);
        scene->clearSelection();
        item->setSelected(true);
}

//生成三角形
void DrawWidget::on_trianglebtn_clicked()
{
    //添加三角形
        QGraphicsPolygonItem   *item=new QGraphicsPolygonItem;
        QPolygonF   points;
        points.append(QPointF(0,-40));
        points.append(QPointF(60,40));
        points.append(QPointF(-60,40));
        item->setPolygon(points);
        item->setPos(-50+(qrand() % 100),-50+(qrand() % 100));

        item->setFlags(QGraphicsItem::ItemIsMovable
                       | QGraphicsItem::ItemIsSelectable
                       | QGraphicsItem::ItemIsFocusable);
        item->setBrush(QBrush(0x8797a6));
        item->setZValue(++frontZ);

        item->setData(ItemId,++seqNum);
        item->setData(ItemDesciption,"三角形");

        scene->addItem(item);
        scene->clearSelection();
        item->setSelected(true);
}

//生成正方形
void DrawWidget::on_square_clicked()
{
    //添加一个正方形
    QGraphicsRectItem   *item=new QGraphicsRectItem(-50,-50,100,100);//x,y 为左上角的图元局部坐标，图元中心点为0,0
    item->setFlags(QGraphicsItem::ItemIsMovable
                   | QGraphicsItem::ItemIsSelectable
                   | QGraphicsItem::ItemIsFocusable);
    item->setBrush(QBrush(0x9eaaba));
    item->setZValue(++frontZ);

    item->setPos(-50+(qrand() % 100),-50+(qrand() % 100));

    item->setData(ItemId,++seqNum);
    item->setData(ItemDesciption,"正方形");

    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}

//生成平行四边形
void DrawWidget::on_para_clicked()
{
    //添加一个平行四边形
        QGraphicsPolygonItem   *item=new QGraphicsPolygonItem;

        QPolygonF   points;
        points.append(QPointF(-40,-40));
        points.append(QPointF(40,-40));
        points.append(QPointF(-20,40));
        points.append(QPointF(-100,40));
        item->setPolygon(points);
        item->setPos(-50+(qrand() % 100),-50+(qrand() % 100));

        item->setFlags(QGraphicsItem::ItemIsMovable
                       | QGraphicsItem::ItemIsSelectable
                       | QGraphicsItem::ItemIsFocusable);
        item->setBrush(QBrush(0xccc1d2));
        item->setZValue(++frontZ);

        item->setData(ItemId,++seqNum);
        item->setData(ItemDesciption,"平行四边形");

        scene->addItem(item);
        scene->clearSelection();
        item->setSelected(true);
}

//自由绘画模式
void DrawWidget::on_drawbtn_clicked()
{
    QSize desktopSize = qApp->desktop()->screenGeometry().size();
    GsMainScene* pScene(new GsMainScene(desktopSize));
    ui->graphicsView->setScene(pScene);
    ui->graphicsView->resize(desktopSize);
    ui->graphicsView->setCursor(Qt::CrossCursor);
}

//返回主界面
void DrawWidget::on_homepagebtn_clicked()
{
    this->close();
    emit backhome();
}

//生成扇形
void DrawWidget::on_piebtn_clicked()
{
    //添加扇形
    QPainterPath piepath;
    QPointF center(28,15); // 圆心坐标
    qreal radius = 100; // 半径
    qreal startAngle = 30; // 起始角度为30度
    qreal spanAngle = 120; // 跨越角度为120度
    piepath.moveTo(center); // 移动到圆心
    piepath.arcTo(QRectF(center.x() - radius, center.y() - radius, 2 * radius, 2 * radius), startAngle, spanAngle); // 绘制扇形
    piepath.lineTo(center); // 连接到圆心，闭合路径
    QGraphicsPathItem   *item=new QGraphicsPathItem(piepath);
    item->setFlags(QGraphicsItem::ItemIsMovable
                   | QGraphicsItem::ItemIsSelectable
                   | QGraphicsItem::ItemIsFocusable);
    item->setBrush(QBrush(0x656565));
    item->setZValue(++frontZ);
    item->setPos(-50+(qrand() % 100),-50+(qrand() % 100));
    item->setData(ItemId,++seqNum);
    item->setData(ItemDesciption,"扇形");
    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}

//生成五角星
void DrawWidget::on_starbtn_clicked()
{
    QPainterPath starPath;
    starPath.moveTo(28, 15);
    for (int i = 1; i < 5; ++i) {
        starPath.lineTo(14 + 14 * qCos(0.8 * i * Pi),
                        15 + 14 * qSin(0.8 * i * Pi));
    }
    starPath.closeSubpath();
    QGraphicsPathItem   *item=new QGraphicsPathItem(starPath);
    item->setFlags(QGraphicsItem::ItemIsMovable
                   | QGraphicsItem::ItemIsSelectable
                   | QGraphicsItem::ItemIsFocusable);
    item->setBrush(QBrush(0xb7c6b3));
    item->setZValue(++frontZ);
    item->setPos(-50+(qrand() % 100),-50+(qrand() % 100));
    item->setData(ItemId,++seqNum);
    item->setData(ItemDesciption,"五角星");
    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}
