#ifndef TPOLARPLOT_H
#define TPOLARPLOT_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QResizeEvent>
#include <QGraphicsItem>

#include <QCPColorGradient.h>
#include <QMessageBox>
#include <QGraphicsOpacityEffect>

#include "tpolargraphitem.h"
#include "tpolaraxis.h"

using namespace std;


static const qreal      OutterRectRatio(0.95);
static const qreal      TickerLenRatio(0.02);//刻度长度 相对于雷达图半径比例
static const qreal      GraphZValue(11);//图标图层优先级
static const QCPRange   DroneWarnRange(0.1,0.2);//无人机警戒范围
static const QCPRange   DroneFatalRange(0.0,0.1);
static const qint32     RingCount(5);           //雷达图内圈个数
static const qint32     TickerCount(5);         //谱图刻度数量
static const qint32     PolarInnerLineCount(8); //极坐标内圈连线条数

#include <QDateTime>

///
/// \brief The TPolarGraphTimeItem class
/// ...

static const qreal TimeFontRatio(36.0/1000.0);//时间字体大小
class TPolarGraphTimeItem: public TPolarGraphItem
{
    Q_OBJECT
public:
    explicit TPolarGraphTimeItem(TPolarAxisAngular *AxisAngular,QGraphicsItem *parent = Q_NULLPTR)
        :TPolarGraphItem(AxisAngular,parent),
          _fm(_font)
    {
        qreal R = _pAxisRadius->Radius();
        qreal d = (1.0-qSqrt(0.5))*R;
        _rect.setRect(-R,-R,d,d);
        _timer = startTimer(1000);
        _font.setPixelSize(R*TimeFontRatio);
    }

protected:

    virtual void timerEvent(QTimerEvent *event)override
    {
        if(event->timerId() == _timer)
        {
            this->update(_rect);
        }
    }

    virtual void draw(QPainter *painter) override
    {
        painter->save();
        painter->setPen(QPen(Qt::red,2));
        painter->setFont(_font);
        QDateTime curTime = QDateTime::currentDateTime();
        painter->drawText(_rect.topLeft()+ QPoint(+20,_fm.height()*3),curTime.toString("当前时间:"));
        painter->drawText(_rect.topLeft()+ QPoint(+20,_fm.height()*6),curTime.toString("yyyy-MM-dd"));
        painter->drawText(_rect.topLeft()+ QPoint(+20,_fm.height()*9),curTime.toString("hh:mm:ss"));
        painter->restore();
        TPolarGraphItem::draw(painter);
    }
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override
    {
        TPolarGraphItem::mousePressEvent(event);
        qDebug()<<__FILE__<<_rect;
    }

private:
    int             _timer;//定时器更新时间
    QFont           _font;//时间字体
    QFontMetrics    _fm;//
};


class TPolarPlot : public QGraphicsView
{
    Q_OBJECT

public:
    TPolarPlot(QWidget *parent = Q_NULLPTR);
    ~TPolarPlot();

    //setter
    void setBoundingRectVisible(const bool &visible);
    void setOpacity(const qreal &opacity);

    //getter
    bool BoundingRectVisible()const;
    qreal Opacity()const;
    TCanvas *canvas()const;
signals:
    void KeyPress(QKeyEvent *event);
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;

private:
    TCanvas                     *_pScene;
    QGraphicsRectItem           *_pRectItem;
    bool                        _BoundingRectVisible;//是否显示外接矩形框
    QGraphicsOpacityEffect      *_pGraphicsOpacityEffect;//透明度设置
    qreal                       _opacity;//透明度
};
#endif // TPOLARPLOT_H










