#include "diagram_3d.h"
#include "ui_diagram_3d.h"

#include <QMessageBox>


Diagram_3D::Diagram_3D(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Diagram_3D)
{
    ui->setupUi(this);
    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);
    resize(800,500);
}

Diagram_3D::~Diagram_3D()
{
    delete ui;
}

void Diagram_3D::closeEvent(QCloseEvent *e)
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

//返回主界面
void Diagram_3D::on_toolButton_4_clicked()
{
    this->close();
    emit backhome();
}

//BAR_3D按钮
void Diagram_3D::on_toolButton_clicked()
{
    Bar3dWidget*widget=new Bar3dWidget();
    widget->setWindowTitle("BAR_3D");
    widget->show();
}

//SCATTER_3D按钮
void Diagram_3D::on_toolButton_2_clicked()
{
   Scatter3dWidget*widget=new Scatter3dWidget();
   widget->setWindowTitle("SCATTER_3D");
   widget->show();
}

//SURFACE_3D按钮
void Diagram_3D::on_toolButton_3_clicked()
{
    Surface3dWidget*widget=new Surface3dWidget();
    widget->setWindowTitle("SURFACE_3D");
    widget->show();
}
