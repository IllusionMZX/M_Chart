#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "freeshape/gsmainscene.h"
#include "themewidget.h"
#include "drawwidget.h"
#include "diagram_3d.h"
#include <QWidget>
#include <QtWidgets/QToolBar>
#include <QDesktopWidget>
#include <QGraphicsView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private slots:
    void on_backhome_clicked();

    void on_diagram2d_clicked();

    void on_pentoolbtn_clicked();

    void on_toolButton_4_clicked();
    void on_homepage_clicked();

private:
    Ui::MainWidget *ui;
};
#endif // MAINWIDGET_H
