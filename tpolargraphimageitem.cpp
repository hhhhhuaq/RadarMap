#include "tpolargraphimageitem.h"

TPolarGraphImageItem::TPolarGraphImageItem(TPolarAxisAngular *AxisAngular, QGraphicsItem *parent)
    :TPolarGraphItem(AxisAngular,parent)
{
    //默认图片大小;
    _rect.setWidth(50);
    _rect.setHeight(50);
}

void TPolarGraphImageItem::setImage(const QString &imgFileName)
{
    _img.load(imgFileName);
    _pix = QPixmap::fromImage(_img).scaled(_rect.size().toSize());
}

void TPolarGraphImageItem::draw(QPainter *painter)
{
    painter->save();
    painter->drawPixmap(_rect.topLeft(),_pix);
    painter->restore();
    TPolarGraphItem::draw(painter);
}
