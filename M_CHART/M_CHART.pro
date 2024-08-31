QT       += core gui
QT       += charts
QT += datavisualization
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += axcontainer
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    brushtool.cpp \
    customslice.cpp \
    diagram_2d/areachartwidget.cpp \
    diagram_2d/barchartwidget.cpp \
    diagram_2d/linechartwidget.cpp \
    diagram_2d/piechartwidget.cpp \
    diagram_2d/scatterchartwidget.cpp \
    diagram_2d/splinechartwidget.cpp \
    diagram_2d/tablewidget.cpp \
    diagram_2d/tchartview.cpp \
    diagram_3d.cpp \
    diagram_3d/scatter3dwidget.cpp \
    diagram_3d/surface3dwidget.cpp \
    diagram_3d/bar3dwidget.cpp \
    drawwidget.cpp \
    freeshape/gsmainscene.cpp \
    freeshape/gsrootitem.cpp \
    freeshape/gsshapebaseitem.cpp \
    freeshape/gsshapecircleitem.cpp \
    freeshape/gsshapefactory.cpp \
    freeshape/gsshapeitem.cpp \
    freeshape/gsshapelineitem.cpp \
    freeshape/gsshapeovalitem.cpp \
    freeshape/gsshaperectangeitem.cpp \
    freeshape/gstempcanvaslayer.cpp \
    main.cpp \
    mainwidget.cpp \
    pentool.cpp \
    qwgraphicsview.cpp \
    themewidget.cpp

HEADERS += \
    brushtool.h \
    customslice.h \
    diagram_2d/areachartwidget.h \
    diagram_2d/barchartwidget.h \
    diagram_2d/linechartwidget.h \
    diagram_2d/piechartwidget.h \
    diagram_2d/scatterchartwidget.h \
    diagram_2d/splinechartwidget.h \
    diagram_2d/tablewidget.h \
    diagram_2d/tchartview.h \
    diagram_3d.h \
    diagram_3d/bar3dwidget.h \
    diagram_3d/scatter3dwidget.h \
    diagram_3d/surface3dwidget.h \
    drawwidget.h \
    freeshape/gscommondef.h \
    freeshape/gsmainscene.h \
    freeshape/gsrootitem.h \
    freeshape/gsshapebaseitem.h \
    freeshape/gsshapecircleitem.h \
    freeshape/gsshapefactory.h \
    freeshape/gsshapeitem.h \
    freeshape/gsshapelineitem.h \
    freeshape/gsshapeovalitem.h \
    freeshape/gsshaperectangeitem.h \
    freeshape/gstempcanvaslayer.h \
    mainwidget.h \
    pentool.h \
    qwgraphicsview.h \
    themewidget.h

FORMS += \
    bar3dwidget.ui \
    diagram_3d.ui \
    drawwidget.ui \
    mainwidget.ui \
    scatter3dwidget.ui \
    surface3dwidget.ui \
    tablewidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
