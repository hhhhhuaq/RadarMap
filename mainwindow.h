#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dronetablewidget.h"
#include "tpolargraphdroneitem.h"
#include "tpolargraphringemeitem.h"
#include "tpolargraphringspectrumitem.h"
#include "tpolarplot.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:

    virtual void resizeEvent(QResizeEvent *event)override;
    virtual QSize sizeHint() const override;


private:
    Ui::MainWindow *ui;
    TCanvas                                 *_pcanvas;//画布
    TPolarAxisAngular                       *_pAxisAngular;//极坐标
    QList<TPolarGraphDroneItem*>            _NomalDrones;   //一般无人机
    QList<TPolarGraphDroneItem*>            _UnknowDrones;   //未知无人机
    QList<TPolarGraphRemoteControlItem*>    _RemoteControl;//遥控器
    QList<TPolarGraphRadioSourceItem*>      _RadioSources;//辐射源
    DroneTableWidget                        *_pDroneTableWidget;
    TPolarGraphTimeItem                     *_pTimeItem;
    QTimer                                  _timer;
};
#endif // MAINWINDOW_H
