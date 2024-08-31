#include "tablewidget.h"
#include "ui_tablewidget.h"

#include <QDir>
#include <QFileDialog>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QtWidgets/QSpinBox>
#include <QSpinBox>
#include <QLabel>
#include <QAxObject>
#include <QProgressDialog>

TableWidget::TableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableWidget)
{
    ui->setupUi(this);
    resize(1000,600);
    setPalette(Qt::white);
    setAutoFillBackground(true);
    ui->spinCount->setValue(studCount);
    //connect
    connect(ui->fileInput,SIGNAL(clicked()),this,SLOT(on_fileInputbtn_clicked()));
    connect(ui->fileOutput,SIGNAL(clicked()),this,SLOT(on_fileOutputbtn_clicked()));


    ui->tableView->setAlternatingRowColors(true);
    ui->fileInput->setIconSize(QSize(40,40));
    ui->fileOutput->setIconSize(QSize(40,40));
    ui->cntOutput->setIconSize(QSize(40,40));
    ui->fileOutput->setIcon(QIcon(":/images/image/download.png"));
    ui->cntOutput->setIcon(QIcon(":/images/image/help.png"));
    ui->fileInput->setIcon(QIcon(":/images/image/fold.png"));
}

TableWidget::~TableWidget()
{
    delete ui;
}

void TableWidget::setTableData(QStandardItemModel  *dataModel)
{
    ui->tableView->setModel(dataModel);
}

//产生数据模型
void TableWidget::generateDataModel()
{
    dataModel = new QStandardItemModel(this); //数据模型

    QStringList  headerList;
    headerList<<"姓名"<<"数学"<<"语文"<<"英语"<<"平均分";
    dataModel->setHorizontalHeaderLabels(headerList);   //设置表头文字

    QList<QStandardItem*>    itemList;     //一行的item列表
    QStandardItem   *item;

    for (int i=0; i<studCount; i++)
    {
        itemList.clear();
        QString studName=QString::asprintf("学生%2d",i+1);
        item=new QStandardItem(studName);  //创建item
        item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        itemList<<item;   //添加到列表

        qreal avgScore=0;
        for (int j=COL_MATH;j<=COL_ENGLISH;j++) //数学，语文，英语
        { //不包含最后一列
            qreal score=60.0+rand()%40; //随机数 [0,40)
            avgScore+=score;

            item=new QStandardItem(QString::asprintf("%.0f",score));   //创建 item
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            itemList<<item;   //添加到列表
        }
        item=new QStandardItem(QString::asprintf("%.1f",avgScore/3));  //创建平均分item
        item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        item->setFlags(item->flags() & (~Qt::ItemIsEditable));  //平均分不允许编辑
        itemList<<item;   //添加到列表
        dataModel->appendRow(itemList);    //添加一行，需要每个Cell的Item
    }
    connect(dataModel,SIGNAL(itemChanged(QStandardItem *)),
                this,SLOT(do_calcuAverage(QStandardItem *)));
}

//初始化统计表格
void TableWidget::initCntTable()
{
    cntdataModel=new QStandardItemModel(this);
    QStringList  headerList1,headerList2;
    headerList1<<"数学"<<"语文"<<"英语";
    headerList2<<"60-69分"<<"70-79分"<<"80-89分"<<"90分以上";
    cntdataModel->setHorizontalHeaderLabels(headerList1);
    cntdataModel->setVerticalHeaderLabels(headerList2);
}

//自动计算平均分SLOT
void TableWidget::do_calcuAverage(QStandardItem *item)
{
    if ((item->column()<COL_MATH) || (item->column()>COL_ENGLISH))
        return; //如果被修改的item不是数学、语文、英语,就退出

    int rowNo=item->row(); //获取数据的行编号
    qreal   avg=0;
    QStandardItem   *aItem;
    for (int i=COL_MATH;i<=COL_ENGLISH;i++)  //获取三个分数的和
    {
        aItem=dataModel->item(rowNo,i);
        avg+=aItem->text().toDouble();
    }
    avg=avg/3; //计算平均分

    aItem=dataModel->item(rowNo,COL_AVERAGE);       //获取平均分数据的item
    aItem->setText(QString::asprintf("%.1f",avg));  //更新平均分数据
}

//打开表格文件
void TableWidget::on_fileInputbtn_clicked()
{

    QString curPath = QDir::currentPath(); // 获取当前目录路径
    QString dlgTitle = "选择一个文件"; // 对话框标题
    QString filter = "Excel文件(*.xlsx);;所有文件(*.*)"; // 文件过滤器
    QString fileName = QFileDialog::getOpenFileName(this, dlgTitle, curPath, filter);

    if (!fileName.isEmpty()) {
        //进度条
        ui->progressBar->setVisible(true);
        ui->progressBar->setValue(0);

        qDebug() << fileName;
        dataModel->clear();
        QAxObject* excel=new QAxObject(this);
            excel->setControl("Excel.Application");//连接Excel控件
            excel->dynamicCall("SetVisible (bool Visible)","false");//不显示窗体
            excel->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示
            //qDebug() << filePath << endl;

            //打开文件
            QAxObject *workbooks = excel->querySubObject("WorkBooks");//获取工作簿集合
            workbooks->dynamicCall("Open (const QString&)", fileName);
            QAxObject *workbook = excel->querySubObject("ActiveWorkBook");//获取当前工作簿

            //打开表
            QAxObject *worksheets = workbook->querySubObject("Sheets");//获取工作表集合
            QAxObject *worksheet = worksheets->querySubObject("Item(int)",1);//工作表1，即sheet1

            //获取表中的数据到QVariant中
            QAxObject *usedRange=worksheet->querySubObject("UsedRange");
            QVariant var=usedRange->dynamicCall("value");
            QList<QStandardItem*>    itemList;
            QStandardItem   *item;
            QVariantList varRowContents=var.toList();
            const int rowCount=varRowContents.size();
            QVariantList tmp;
            for(int i=0; i<rowCount; ++i)
            {
                itemList.clear();
                tmp=varRowContents[i].toList();
                qDebug() << tmp.size()<< endl;
                for(int j=0;j<tmp.size();j++){
                    item=new QStandardItem(tmp[j].toString());   //创建 item
                    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                    itemList<<item;   //添加到列表
                }
                qDebug() << tmp << endl;
                dataModel->appendRow(itemList);

            }
            for(int i=0;i<50001;i++){
                ui->progressBar->setValue(i);
            }
            //删除，防止程序关闭后有excel进程残留
            excel->dynamicCall("Quit(void)");
            delete excel;
            QString Title = "导入";
            QString Info = "导入成功！";
            QMessageBox::about(this, Title, Info);
            ui->progressBar->setValue(0);
        }
    setTableData(dataModel);
}

//导出表格文件
void TableWidget::on_fileOutputbtn_clicked()
{
    QString curPath = QCoreApplication::applicationDirPath(); // 获取应用程序的路径
    QString dlgTitle = "保存文件"; // 对话框标题
    QString filter = "Excel文件(*.xlsx);;所有文件(*.*)"; // 文件过滤器
    QString fileName = QFileDialog::getSaveFileName(this, dlgTitle, curPath, filter);

    if (!fileName.isEmpty()) {
        qDebug() << fileName;

        QAxObject *excel = new QAxObject("Excel.Application", this);
        excel->dynamicCall("SetVisible(bool)", false); // 设置 Excel 应用程序的可见性为 false

        QAxObject *workbooks = excel->querySubObject("Workbooks");
        QAxObject *workbook = workbooks->querySubObject("Add");
        QAxObject *sheets = workbook->querySubObject("Worksheets");
        QAxObject *sheet = sheets->querySubObject("Item(int)", 1);

        // 导出 dataModel 数据到 Excel 工作表
        for (int row = 0; row < dataModel->rowCount(); row++) {
            for (int column = 0; column < dataModel->columnCount(); column++) {
                QAxObject *cell = sheet->querySubObject("Cells(int, int)", row + 1, column + 1);
                cell->dynamicCall("SetValue(const QVariant&)", dataModel->data(dataModel->index(row, column)).toString());
                delete cell;
            }
        }

        // 保存并关闭工作簿
        workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(fileName));
        workbook->dynamicCall("Close(bool)", false);

        excel->dynamicCall("Quit()"); // 关闭 Excel 应用程序
        delete excel;

        QString dialogTitle = "保存";
        QString strInfo = "保存成功！";
        QMessageBox::about(this, dialogTitle, strInfo);
    }
}

void TableWidget::countData()
{
    for(int i=COL_MATH;i<=COL_ENGLISH;i++)
    {
        int cnt50=0;
        int cnt60=0;
        int cnt70=0;
        int cnt80=0;
        int cnt90=0;

        for(int j=0;j<dataModel->rowCount();j++)
        {
            qreal val=dataModel->item(j,i)->text().toDouble();

            if (val<60)
                cnt50++;
            else if ((val>=60) && (val<70))
                cnt60++;
            else if ((val>=70) && (val<80))
                cnt70++;
            else if ((val>=80) && (val<90))
                cnt80++;
            else
                cnt90++;
        }

    }
}

//生成图表按钮点击
void TableWidget::on_pushButton_clicked()
{
    this->hide();
}

//学生人数更改SLOT
void TableWidget::on_spinCount_valueChanged(int arg1)
{
    studCount=arg1;
    generateDataModel();
    ui->tableView->setModel(dataModel);
}

