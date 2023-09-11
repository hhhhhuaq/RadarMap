#ifndef TPOLARAXIS_H
#define TPOLARAXIS_H

#include <QObject>
#include <QtMath>
#include <QFontMetricsF>
#include "tabstractgraph.h"
#include "tcanvas.h"
static const qreal  PolarZValue(10);//极坐标系图层优先级
static const QRgb  FontColor(0x0cebff);//字体颜色
static const qreal DistanceLableFontSizeRatio(36.0/1000.0); //距离刻度字体比例
static const qreal TickerLableSizeFontRatio(36.0/1000.0);//字体刻度大小
///
/// \brief The TPolarPoint class
/// 坐标数据类
class TPolarPoint
{
public:
    TPolarPoint():_radius(.0),_angle(.0){}
    TPolarPoint(const qreal &radius,const qreal &angle):_radius(radius),_angle(angle){}
    TPolarPoint &operator=(const TPolarPoint &p)
    {
        if(this == &p) return *this;
        this->_radius = p._radius;
        this->_angle = p._angle;
        return *this;
    }
    TPolarPoint (const TPolarPoint &p)
    {
        this->_radius = p._radius;
        this->_angle = p._angle;
    }

    friend QDebug operator << (QDebug debug, const TPolarPoint &point) {
        debug << "(Radius: "<<point._radius<<","<<" Angle: "<<point._angle<<")";
        return debug;
    }

    void setRadius(const qreal &radius){this->_radius = radius;}
    void setAngle(const qreal &angle){this->_angle = angle;}

    QPointF toPointF()
    {
        double arcl = (_angle - 90.0)/180.0*M_PI;
        qreal x = _radius * qCos(arcl);
        qreal y = _radius * qSin(arcl);
        return QPointF(x,y);
    }
    qreal Radius()const{return this->_radius;}
    qreal Angle()const{return this->_angle;}
private:
    qreal _radius;//半径长度
    qreal _angle; //角度
};

///
/// \brief The TPolarAxisAxisRadius class
/// 极坐标极径
class TPolarAxisRadius : public TAbstractGraph
{
    Q_OBJECT
public:
    explicit TPolarAxisRadius(TCanvas *canvas,QGraphicsItem *parent = Q_NULLPTR)
        :TAbstractGraph(parent)
        ,_Radius(0.0)
        ,_pCanvas(canvas)
        ,_visible(false)
    {
        _pCanvas->addItem(this);
        QRectF rect = _pCanvas->sceneRect();
        _Radius = (rect.width()>rect.height() ? rect.height():rect.width())/2.0;
        _rect = rect;
    }

    void setVisible(const bool &visible)
    {
        this->_visible = visible;
    }
    bool Visivle()const{return this->_visible;}
    qreal Radius()const{return _Radius;}

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) Q_DECL_OVERRIDE
    {
        Q_UNUSED(option)
        Q_UNUSED((widget))

        if(painter->isActive())
        {
            draw(painter);
        }
    }
    virtual void draw(QPainter *painter)Q_DECL_OVERRIDE
    {
        //待实现
        Q_UNUSED(painter)
        if(_visible == true)
        {
            //显示极径
        }
    }
protected:
    qreal           _Radius;    //极径长度
    TCanvas         *_pCanvas;  //画布
    bool            _visible;   //是否显示极径
private:
    virtual void addAction(QAction *action)Q_DECL_OVERRIDE {Q_UNUSED(action)}
    virtual void addActions(const QList<QAction *> &actions) Q_DECL_OVERRIDE{Q_UNUSED(actions)}
};

///
/// \brief The TPolarAxisRadius class
/// 极坐标
class TPolarAxisAngular : public TAbstractGraph
{
    Q_OBJECT
public:
    explicit TPolarAxisAngular(TCanvas *scene,QGraphicsItem *parent = Q_NULLPTR)
        :TAbstractGraph(parent),
          _pScene(scene),
          _pAxisRadius(new TPolarAxisRadius(scene,parent)),
          _RingCount(5),
          _pen(QColor(Qt::green)),
          _angleTickCount(8),
          _TickerLableLen(5),
          _Tickerlen(20)

    {
        _pScene->addItem(this);
        this->setPen(_pen);
        this->setZValue(PolarZValue);
        _pAxisRadius->setZValue(PolarZValue);
    }

    TPolarAxisRadius *AxisRadius()const{return _pAxisRadius;}

    void setRingCount(const int &count)
    {
        _RingCount = count;
    }
    void setPen(const QPen &pen){this->_pen = pen;}
    void setRadiusLable(const QStringList &sl){this->_RadiusLable = sl;};

    int RingCount()const{return this->_RingCount;}
    QStringList DistanceLable()const {return this->_RadiusLable;}
    QPen Pen()const{return this->_pen;}
    qreal TickerLen()const {return this->_Tickerlen;}

    ///
    /// \brief qreal
    /// 极坐标主图半径长度
    virtual qreal Radius()const {return this->_pAxisRadius->Radius();}

protected:
    virtual void draw(QPainter *painter) Q_DECL_OVERRIDE
    {

        calPoint();
        qreal AbsoluteRadius = _pAxisRadius->Radius();//半径绝对长度
        //十字线
        painter->save();
        QPen CrossLinePen;
        CrossLinePen.setColor(_pen.color());//主画笔
        CrossLinePen.setWidth(3);
        painter->setPen(CrossLinePen);
        painter->drawLine(_XLine);
        painter->drawLine(_YLine);
        painter->restore();

        //角度分割线;
        painter->save();
        QPen AngleLinePen;
        AngleLinePen.setColor(_pen.color());//主画笔
        AngleLinePen.setWidth(3);
        AngleLinePen.setStyle(Qt::DashLine);
        painter->setPen(AngleLinePen);
        for(int i = 0 ; i < _AngleLine.size() ; i++)
        {
            painter->drawLine(_AngleLine[i]);
        }
        painter->restore();

        //刻度
        painter->save();
        QPen TickersPen;
        TickersPen.setColor(_pen.color());
        painter->setPen(TickersPen);
        for(int i = 0 ; i < _Tickers.size() ; i++)
        {
            painter->drawLine(_Tickers[i]);
        }
        painter->restore();

        //刻度标签
        painter->save();
        QPen TickerLablesPen;
        TickerLablesPen.setColor(_pen.color());
        painter->setPen(TickerLablesPen);
        QFont LableFont;
        LableFont.setPixelSize(TickerLableSizeFontRatio*AbsoluteRadius);
        painter->setFont(LableFont);

        qreal dangle = 360.0/_TickerLable.size();
        for(int i = 0 ; i < _TickerLable.size() ; i++)
        {
            painter->drawText(0.0,-this->Radius()-15,QString::number(i*dangle));
            painter->rotate(dangle);
        }
        painter->restore();

        //外圈
        painter->save();
        QPen OuterRingPen;
        OuterRingPen.setColor(_pen.color());
        OuterRingPen.setWidth(2);
        qreal R = this->Radius();
        painter->setPen(OuterRingPen);
        painter->drawArc(-R,-R,R*2,R*2,0,16*360);
        painter->restore();

        //内圈环
        painter->save();
        QPen InnerRingPen;
        InnerRingPen.setColor(_pen.color());
        InnerRingPen.setStyle(Qt::DashLine);
        QFont DistanceLableFont;
        DistanceLableFont.setPixelSize(AbsoluteRadius*DistanceLableFontSizeRatio);
        painter->setFont(DistanceLableFont);
        painter->setPen(InnerRingPen);


        for(int i = 1 ; i < _RadiusLable.size() ; i++)
        {
            painter->drawArc(_RingRect[i],0,16*360);
            painter->drawText(_RadiusLablePoint[i],_RadiusLable[i]);
        }
        painter->restore();

    }

    void calPoint()
    {
        qreal _Radius = this->Radius();
        qreal ox = 0.0,oy = 0.0;;//原点
        qreal ex = 0.0,ey = -_Radius;

        //十字线
        _XLine.setPoints(QPointF(-_Radius,0.0),QPointF(_Radius,0.0));
        _YLine.setPoints(QPointF(0.0,-_Radius),QPointF(0.0,_Radius));

        //角度分割线
        _AngleLine.clear();
        qreal dAngle = 360.0/_angleTickCount; //每一角度步长
        qreal angle = 0;
        qreal theta = (dAngle)/180.0*M_PI;
        for(int i = 0 ; i < _angleTickCount ; i++)
        {
            angle = dAngle*i;
            theta = (angle)*M_PI/180.0;
            ex = _Radius*qSin(theta);
            ey = _Radius*qCos(theta);
            if((int(angle))%90 != 0)_AngleLine.append(QLineF(ox,oy,ex,ey));
        }

        //刻度
        _TickerLable.clear();
        _Tickers.clear();
        qreal oR = _Radius -_Tickerlen;
        qreal eR = _Radius;
        for(int i = 0 ; i < 360 ; i++)
        {
            angle = i;
            theta = (angle)/180.0*M_PI;
            ox = oR*qSin(theta);
            oy = oR*qCos(theta);

            ex = eR*qSin(theta);
            ey = eR*qCos(theta);

            if(i%15 == 0)
            {
                ox = (oR - _Tickerlen)*qSin(theta);
                oy = (oR - _Tickerlen)*qCos(theta);
                _Tickers.append(QLineF(ox,oy,ex,ey));
                _TickerLable.append(QPointF(ox,oy));
            }
            else
            {
                _Tickers.append(QLineF(ox,oy,ex,ey));
            }
        }

        //内圈环
        _RingRect.clear();
        _RadiusLablePoint.clear();
        const qreal dR = _Radius/_RadiusLable.size();
        for(int i = 0 ; i < _RadiusLable.size() ; i++)
        {
            _RingRect.append(QRectF(-dR*i,-dR*i,dR*i*2,dR*i*2));
            _RadiusLablePoint.append(QPointF(0.0,-dR*i));
        }
    }


protected:
    TCanvas                 *_pScene;                   //场景(画布
    TPolarAxisRadius        *_pAxisRadius;              //极径
    int                     _RingCount;                 //内层圈数量
    QPen                    _pen;                       //画笔
    int                     _angleTickCount;            //角度刻度数量
    qreal                   _TickerLableLen;            //刻度长度
    QLineF                  _XLine;                     //十字横线
    QLineF                  _YLine;                     //十字竖线
    QList<QLineF>           _AngleLine;                 //角度分割线
    QList<QLineF>           _Tickers;                   //刻度
    QList<QPointF>          _TickerLable;               //标签
    QList<QRectF>           _RingRect;                  //内圈环外接矩形；
    QList<QPointF>          _RadiusLablePoint;          //半径刻度坐标
    qreal                   _Tickerlen;                 //标签长度
    QStringList             _RadiusLable;               //半径刻度长度标签

private:
    virtual void addAction(QAction *action)Q_DECL_OVERRIDE {Q_UNUSED(action)}
    virtual void addActions(const QList<QAction *> &actions) Q_DECL_OVERRIDE{Q_UNUSED(actions)}
};


#endif // TPOLARAXIS_H
