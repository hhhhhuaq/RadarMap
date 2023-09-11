#ifndef TPOLARGRAPHRINGITEM_H
#define TPOLARGRAPHRINGITEM_H

#include "tpolargraphitem.h"


///
/// \brief The TPolarRingItem class
/// 环状图
class TPolarGraphRingItem : public TPolarGraphItem
{
    Q_OBJECT
public:
    explicit TPolarGraphRingItem(TPolarAxisAngular *AxisAngular,QGraphicsItem *parent = Q_NULLPTR);
    void setAngleLen(const qreal &angle);
    void setRingWidth(const qreal &w);
    void setRingBorderPen(const QPen &pen){this->_RingBorderPen = pen;}
    void setRingBorderVisible(const bool &visible);


    qreal AngleLen()const;
    qreal RingWidth()const;
    QPen  RingBorderPen()const;
    bool  RingBorderVisible()const;

    QPainterPath shape() const Q_DECL_OVERRIDE
    {
        return _path;
    }

protected:
    void calculateBoundingRect();
    void caculatePolyPoint();
    virtual void draw(QPainter *painter)Q_DECL_OVERRIDE
    {
        TPolarGraphItem::draw(painter);
        _path = QPainterPath();//半径改变 轨迹清空
        calculateBoundingRect();
        if(_RingBorderVisible)
        {  //圆环轨迹
            painter->save();
            painter->setPen(_RingBorderPen);
            painter->drawPath(_path);
            painter->restore();
        }
    }
protected:
    qreal                   _angle;             //圆弧角度
    qreal                   _RingWidth;         //圆环宽度
    bool                    _RingBorderVisible; //是否显示环边界
    QPen                    _RingBorderPen;     //环边界画笔;
    qreal                   _RingWidthRatio;    //宽度比例长度
    QPolygonF               _ArcPolygon;        //环形图坐标点路径
    QRectF                  _OuterArcRect;      //外弧外接矩形
    QRectF                  _InnerArcRect;      //内弧外接矩形
    qreal                   _OuterRadius;       //外弧半径
    qreal                   _InnerRadius;       //内弧半径
};




#endif // TPOLARGRAPHRINGITEM_H
