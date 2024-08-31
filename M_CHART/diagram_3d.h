#ifndef DIAGRAM_3D_H
#define DIAGRAM_3D_H

#include <QWidget>

#include "diagram_3d/scatter3dwidget.h"
#include "diagram_3d/bar3dwidget.h"
#include "diagram_3d/surface3dwidget.h"

namespace Ui {
class Diagram_3D;
}

class Diagram_3D : public QWidget
{
    Q_OBJECT

public:
    explicit Diagram_3D(QWidget *parent = nullptr);
    ~Diagram_3D();
    void closeEvent(QCloseEvent *);

private:
    Ui::Diagram_3D *ui;

signals:
    void backhome();

private slots:
    void on_toolButton_4_clicked();
    void on_toolButton_clicked();
    void on_toolButton_2_clicked();
    void on_toolButton_3_clicked();
};

#endif // DIAGRAM_3D_H
