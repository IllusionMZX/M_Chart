#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QtWidgets/QToolBar>
#include <QDesktopWidget>
#include <QGraphicsView>
#include <QToolTip>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    setPalette(QPalette(Qt::white));
    QToolTip::setFont(QFont("SimSun",12));
    setAutoFillBackground(true);
    resize(800,500);
}

MainWidget::~MainWidget()
{
    delete ui;
}

//回到主页SLOT
void MainWidget::on_backhome_clicked()
{
    this->show();
}

//统计图表
void MainWidget::on_diagram2d_clicked()
{
    ThemeWidget *widget = new ThemeWidget();
    connect(widget,&ThemeWidget::backhome,this,&MainWidget::show);//关联子窗体关闭信号与父窗体显示事件
    widget->setWindowTitle("Diagram_2D");
    widget->setAttribute(Qt::WA_DeleteOnClose);//设置子窗体属性：关闭即销毁
    widget->show();
    this->hide();
}

//绘画
void MainWidget::on_pentoolbtn_clicked()
{

    DrawWidget*view=new DrawWidget();
    connect(view,&DrawWidget::backhome,this,&MainWidget::show);//关联子窗体关闭信号与父窗体显示事件
    view->setWindowTitle("Free Draw");
    view->setAttribute(Qt::WA_DeleteOnClose);//设置子窗体属性：关闭即销毁
    view->show();//显示子窗体
    this->hide();

}

//3D
void MainWidget::on_toolButton_4_clicked()
{

    Diagram_3D*view3d=new Diagram_3D();
    connect(view3d,&Diagram_3D::backhome,this,&MainWidget::show);//关联子窗体关闭信号与父窗体显示事件
    view3d->setWindowTitle("Diagram_3D");
    view3d->setAttribute(Qt::WA_DeleteOnClose);//设置子窗体属性：关闭即销毁
    view3d->show();//显示子窗体
    this->hide();
}

void MainWidget::on_homepage_clicked()
{
    resize(800,500);
}
