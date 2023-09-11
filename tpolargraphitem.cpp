#include "tpolargraphitem.h"

TPolarGraphItem::TPolarGraphItem(TPolarAxisAngular *AxisAngular, QGraphicsItem *parent)
    :TAbstractGraph(parent)
    ,_Point(.0,.0)
    ,_BorderPen(Qt::green,2)
    ,_pAxisAngular(AxisAngular)
    ,_pAxisRadius(AxisAngular->AxisRadius())
    ,_BorderVisible(false)
    ,_padding(false)
    ,_brush(Qt::red)
{
    _pAxisAngular->scene()->addItem(this);
    this->setZValue(this->_pAxisAngular->zValue()+1);//置图片置于坐标之上
}

void TPolarGraphItem::setPolarPos(const qreal &radius, const qreal &angle)
{
    this->_Point.setRadius(radius);
    this->_Point.setAngle(angle);
    QPointF p = this->_Point.toPointF() - _rect.center();
    this->setPos(p);
}

void TPolarGraphItem::setPolarPos(const TPolarPoint &pos)
{
    this->setPolarPos(pos.Radius(),pos.Angle());
}



void TPolarGraphItem::setBorderPen(const QPen &pen)
{
    this->_BorderPen = pen;
}

void TPolarGraphItem::setBorderColor(const QColor &color)
{
    this->_BorderPen.setColor(color);
}

void TPolarGraphItem::setBorderVisible(const bool &visible)
{
    this->_BorderVisible = visible;
    this->update();
}

void TPolarGraphItem::setPadding(const bool &padding){this->_padding = padding;}

void TPolarGraphItem::setSize(const QSizeF &sz){_rect.setSize(sz);}

void TPolarGraphItem::setBrush(const QBrush &brush){this->_brush = brush;}

void TPolarGraphItem::draw(QPainter *painter)
{
    if(_BorderVisible)//是否显示外接矩形
    {
        painter->save();
        painter->setPen(_BorderPen);
        painter->drawRect(boundingRect());
        painter->restore();
    }
    if(_padding)//是否填充
    {
        painter->save();
        painter->setBrush(_brush);
        //若path为空则填充区域以外接矩形为准
        _path.isEmpty() ?  painter->drawRect(boundingRect()):painter->drawPath(_path);
        painter->restore();
    }
}
