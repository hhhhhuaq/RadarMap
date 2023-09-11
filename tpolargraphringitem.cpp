#include "tpolargraphringitem.h"

TPolarGraphRingItem::TPolarGraphRingItem(TPolarAxisAngular *AxisAngular, QGraphicsItem *parent)
    :TPolarGraphItem(AxisAngular,parent),
      _angle(0.0),
      _RingWidth(0.0),
      _RingBorderVisible(false),
      _RingBorderPen(Qt::red,2),
      _RingWidthRatio(1.0),
      _ArcPolygon(4),
      _OuterArcRect(0.0,0.0,0.0,0.0),
      _InnerArcRect(0.0,0.0,0.0,0.0),
      _OuterRadius(0.0),
      _InnerRadius(0.0)
{

}

void TPolarGraphRingItem::setAngleLen(const qreal &angle)
{
    _angle = angle <= 360.0 ? angle : 360.0;
    calculateBoundingRect();
    this->update();
}

void TPolarGraphRingItem::setRingWidth(const qreal &w)
{
    _RingWidthRatio = w/this->_pAxisRadius->Radius();
    this->_RingWidth = w;
    calculateBoundingRect();
}

void TPolarGraphRingItem::setRingBorderVisible(const bool &visible)
{
    this->_RingBorderVisible = visible;
    this->_pAxisAngular->scene()->update();
}

qreal TPolarGraphRingItem::AngleLen() const{return _angle;}

qreal TPolarGraphRingItem::RingWidth() const{return _RingWidth;}

QPen TPolarGraphRingItem::RingBorderPen() const{return _RingBorderPen;}

bool TPolarGraphRingItem::RingBorderVisible() const{return _RingBorderVisible;}

void TPolarGraphRingItem::calculateBoundingRect()
{
    caculatePolyPoint();
    //计算环图外接矩形坐标 可优化
    qreal   rectW,rectH;
    if( 0.0 < _angle && _angle <= 90.0)
    {
        rectW = qAbs(_ArcPolygon[1].x());
        rectH = qAbs(_ArcPolygon[0].y() - _ArcPolygon[2].y());
        _rect = QRectF(_ArcPolygon[0].x(),_ArcPolygon[0].y(),rectW,rectH);
    }
    else if(90.0<_angle && _angle<=180.0)
    {
        rectW = this->_pAxisRadius->Radius();
        rectH = qAbs(_ArcPolygon[0].y() - _ArcPolygon[1].y());
        _rect = QRectF(_ArcPolygon[0].x(),_ArcPolygon[0].y(),rectW,rectH);
    }
    else if(180.0<_angle && _angle<=270.0)
    {
        rectW = qAbs(_ArcPolygon[1].x()) + _OuterRadius;
        rectH = _OuterRadius*2;
        _rect = QRectF(_ArcPolygon[1].x(),-_OuterRadius,rectW,rectH);
    }
    else
    {
        rectW = _OuterRadius;
        rectH = _OuterRadius;
        _rect = QRectF(-rectW,-rectH,rectW*2,rectH*2);
    }
}

void TPolarGraphRingItem::caculatePolyPoint()
{
    double arclen   = _angle*M_PI/180.0;//弧长
    _OuterRadius    = this->Radius();  //外径
    _InnerRadius    = _OuterRadius - this->RingWidth(); //内径
    _OuterArcRect.setRect(-_OuterRadius,-_OuterRadius,_OuterRadius*2.0,_OuterRadius*2.0);  //外弧外接矩形
    _InnerArcRect.setRect(-_InnerRadius,-_InnerRadius,_InnerRadius*2.0,_InnerRadius*2.0);; //内弧外接矩形

    //多边形四点
    _ArcPolygon[0] = QPointF(0.0,-_OuterRadius);
    _ArcPolygon[1] = QPointF(_OuterRadius*qSin(arclen),-_OuterRadius*qCos(arclen));
    _ArcPolygon[2] = QPointF(_InnerRadius*qSin(arclen),-_InnerRadius*qCos(arclen));
    _ArcPolygon[3] = QPointF(0.0,-_InnerRadius);

    _path.moveTo(_ArcPolygon[0]);//起始点
    //! 1外层圆弧
    _path.arcTo(_OuterArcRect,90.0,-_angle);
    //! 2右内外径连线
    _path.lineTo(_ArcPolygon[2]);
    //! 3内层圆弧
    _path.arcTo(_InnerArcRect,90.0 - _angle,_angle);
    //! 4左内外径连线
    _path.lineTo(_ArcPolygon[0]);
}
