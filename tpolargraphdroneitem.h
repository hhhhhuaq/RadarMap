#ifndef TPOLARGRAPHDRONEITEM_H
#define TPOLARGRAPHDRONEITEM_H

#include "tpolargraphimageitem.h"
#include "tpolaraxisangularspectrumradar.h"
#include <QMessageBox>

static const QString ImageNomalDroneFileName(":/image/NomalDrone.jpeg");//一般无人机图片路径
static const QString ImageRemoteControlFileName(":/image/RemoteControl.png");//遥控器图片路径
static const QString ImageUnkonwDroneFileName(":/image/UnknownDrone.png");//未知无人机
static const QString ImageRadioSourceFileName(":/image/RadioSource.png");


///
/// \brief The TPolarGraphPolylineItem class
/// 折线
class TPolarGraphPolylineItem : public TPolarGraphItem
{
    Q_OBJECT
public:
    explicit TPolarGraphPolylineItem(TPolarAxisAngular *pAxisAngular,QGraphicsItem *parent = Q_NULLPTR)
        :TPolarGraphItem(pAxisAngular,parent)
    {
        this->setPen(QPen(Qt::red,4,Qt::SolidLine));
        setVisible(false);//默认不显示
    }
    void addPoint(const QPointF &p)
    {
        _Points.append(p);
    }
protected:
    virtual void draw(QPainter *painter) final
    {
        painter->setPen(this->pen());
        painter->drawPolyline(_Points);
    }
private:
    QPolygonF           _Points;
};

///
/// \brief The TPolarDroneGraph class
/// 无人机
class TPolarGraphDroneItem : public TPolarGraphImageItem
{
    Q_OBJECT
public:
    enum ThreatenLevel
    {
        Low,
        Mid,
        High
    };
    explicit TPolarGraphDroneItem(TPolarAxisAngular *pAxisAngular,QGraphicsItem *parent = Q_NULLPTR);

    virtual ~TPolarGraphDroneItem();

    void addThreatenRange(const QCPRange &range,const ThreatenLevel &level);
    void addThreatenRange(const qreal &lower,const qreal &upper,const ThreatenLevel &level);

    void addData(const qreal &rd,const qreal &angle);
    void setData(const QVector<TPolarPoint> &points);

    inline QVector<TPolarPoint> LogicPoints()const {return this->_LogicPoints;}
    inline TPolarPoint LogicPoint()const{return _LogicPoints.empty()? TPolarPoint():this->_LogicPoints.last();}

    inline void setLocusVisible(const bool &visible){this->_pPolyLine->setVisible(visible);}
    inline bool LocusVisible(){return this->_pPolyLine->isVisible();}


protected:
    virtual void draw(QPainter *painter) Q_DECL_OVERRIDE
    {
        TPolarGraphImageItem::draw(painter);
    }
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE
    {
        TPolarGraphImageItem::mousePressEvent(event);
        if(event->button() == Qt::LeftButton)
        {
            this->setBorderVisible(!this->BorderVisible());
        }
    }
private:
    QColor ThreatenLevelToColor(const ThreatenLevel &level)
    {
        if(level == Low) return Qt::green;
        else if(level == Mid) return Qt::yellow;
        else if(level == High) return Qt::red;
        return QColor();
    }
    class TDroneActions : public QList<QAction*>
    {
    public:
        TDroneActions(TPolarGraphDroneItem *item)
        {
            QAction *pPresentPoint = new QAction("当前位置",item);
            QAction *pLookPath     = new QAction("查看轨迹",item);
            QAction *pSetWhiteList = new QAction("设为白名单",item);
            QAction *pSetBlackList = new QAction("设为黑名单",item);
            QAction *pOutputReport = new QAction("生成报告",item);
            QAction *pOutputHisPath= new QAction("导出历史数据",item);
            this->append(pPresentPoint);
            this->append(pLookPath);
            this->append(pSetWhiteList);
            this->append(pSetBlackList);
            this->append(pOutputReport);
            this->append(pOutputHisPath);
            QObject::connect(pPresentPoint,&QAction::triggered,[item](){
                qreal r = item->LogicPoint().Radius();
                qreal a = item->LogicPoint().Angle();
                QMessageBox::information(0,"位置",QString("(Radius:%1,Angle:%2").arg(r).arg(a));
            });
            QObject::connect(pLookPath,&QAction::triggered,[item,pLookPath](){
                pLookPath->setText(QString(item->LocusVisible() ?"查看":"隐藏")+ "轨迹");
                item->setLocusVisible(!item->LocusVisible());
            });
        }
        virtual ~TDroneActions()
        {
            QAction *p = nullptr;
            for(int i = 0 ; i < this->size() ; i++)
            {
                p = this->at(i);
                if(p != nullptr)
                {
                    p->menu()->removeAction(p);
                    delete p;
                    p = nullptr;
                }
                else
                {
                    qDebug()<<__FILE__<<"Deconstruction error";
                }
            }
        }
    };
private:
    int _timerID;
    QMap<ThreatenLevel,QCPRange>   _ThreatenRanges;
    QVector<TPolarPoint>           _LogicPoints;        //极坐标真实方位历史位置
    qreal                          _DisRatio;           //当前位置在坐标系极轴中的占比
    TPolarAxisAngular               *_pAxisAngular;
    TDroneActions                  *_pActions;          //右键菜单;
    bool                           _LocusVisible;       //轨迹点显示
    TPolarGraphPolylineItem        *_pPolyLine;         //轨迹线
};

///
/// \brief The TPolarGraphRemoteControlItem class
/// 遥控器
class TPolarGraphRemoteControlItem : public TPolarGraphImageItem
{
public:
    explicit TPolarGraphRemoteControlItem(TPolarAxisAngular *pAxisAngular,QGraphicsItem *parent = Q_NULLPTR)
        :TPolarGraphImageItem(pAxisAngular,parent),
          _pAxisAngular(pAxisAngular)
    {
        this->setImage(ImageRemoteControlFileName);
//        connect(_pAxisAngular,&TPolarAxisAngularSpectrumRadar::RadarRadiusChange,this,[&](const qreal &r)
//        {
//            Q_UNUSED(r)
//            TPolarPoint p = this->LogicPoint();
//            setPos(p.Radius(),p.Angle());
//        });
    }
    void setPos(const qreal &rd,const qreal &angle)
    {
        //rd真实距离 rd/LogicRadius得出对应比例 Radius*Range().upper为雷达半径
//        qreal R = _pAxisAngular->RadarRadius() * rd/_pAxisAngular->LogicRadius();
//        _LogicPoint.setRadius(rd);
//        _LogicPoint.setAngle(angle);
//        this->setPolarPos(R,angle);
    }
    TPolarPoint LogicPoint()const{return this->_LogicPoint;}
private:
     TPolarAxisAngular *_pAxisAngular;
     TPolarPoint                    _LogicPoint;//真实方位点
};

///
/// \brief The TPolarGraphRadioSourceItem class
/// 辐射源
class TPolarGraphRadioSourceItem : public TPolarGraphImageItem
{
public:
    explicit TPolarGraphRadioSourceItem(TPolarAxisAngular *pAxisAngular,QGraphicsItem *parent = Q_NULLPTR)
        :TPolarGraphImageItem(pAxisAngular,parent),
          _pAxisAngular(pAxisAngular)
    {
        this->setImage(ImageRadioSourceFileName);
//        connect(_pAxisAngular,&TPolarAxisAngularSpectrumRadar::RadarRadiusChange,this,[&](const qreal &r)
//        {
//            Q_UNUSED(r)
//            TPolarPoint p = this->LogicPoint();
//            setPos(p.Radius(),p.Angle());
//        });
    }
    void setPos(const qreal &rd,const qreal &angle)
    {
        //rd真实距离 rd/LogicRadius得出对应比例 Radius*Range().upper为雷达半径
//        qreal R = _pAxisAngular->RadarRadius() * rd/_pAxisAngular->LogicRadius();
//        this->setPolarPos(R,angle);
    }

    TPolarPoint LogicPoint()const{return this->_LogicPoint;}
private:
     TPolarAxisAngular *_pAxisAngular;
     TPolarPoint                    _LogicPoint;//真实方位点

};

///
/// \brief The TPolarGraphPulseItem class
/// 脉冲
class TPolarGraphPulseItem : public TPolarGraphItem
{
    Q_OBJECT
public:
    explicit TPolarGraphPulseItem(TPolarAxisAngular *pAxisAngular,QGraphicsItem *parent = Q_NULLPTR)
        :TPolarGraphItem(pAxisAngular,parent),
          _pAxisAngular(pAxisAngular)
    {
        this->setSize(QSizeF(50,25));
        this->setBrush(QColor(255,0,0));
        this->setPadding(true);
        _timer = startTimer(10);
//        connect(_pAxisAngular,&TPolarAxisAngularSpectrumRadar::RadarRadiusChange,this,[&](const qreal &r)
//        {
//            Q_UNUSED(r)
//            TPolarPoint p = this->LogicPoint();
//            setPos(p.Radius(),p.Angle());
//        });
    }
    void setLogicPos(const qreal &rd,const qreal &angle)
    {
//        _LogicPoint.setRadius(rd);
//        _LogicPoint.setAngle(angle);
//        //rd真实距离 rd/LogicRadius得出对应比例 Radius*Range().upper为雷达半径
//        qreal R = _pAxisAngular->RadarRadius() * _LogicPoint.Radius()/_pAxisAngular->LogicRadius();
//        this->setTransformOriginPoint(_rect.center());
//        this->setRotation(angle);
//        this->setPolarPos(R,angle);
    }
    TPolarPoint LogicPoint()const{return this->_LogicPoint;}
    virtual void timerEvent(QTimerEvent *event)override
    {
        if(event->timerId() == _timer)
        {
            int c = this->Brush().color().red();
            if(c == 0)
            {
                killTimer(_timer);
                emit disapear();
                return ;
            }
            this->setBrush(QColor(c-1,0,0));
            this->update(_rect);
        }
    }
signals:
    void disapear();
private:
     TPolarAxisAngular *_pAxisAngular;
     int                            _timer;//渐变定时器
     TPolarPoint                    _LogicPoint;//真实方位点
};



#endif // TPOLARGRAPHDRONEITEM_H
