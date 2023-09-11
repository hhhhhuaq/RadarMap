#ifndef TPOLARGRAPHIMAGEITEM_H
#define TPOLARGRAPHIMAGEITEM_H

#include "tpolargraphitem.h"

static const qreal GraphSizeRatio(50.0/1200.0);//图标大小比例(无人机,遥控器等///

/// \brief The TPolarImageGraph class
/// 图片图像类
class TPolarGraphImageItem: public TPolarGraphItem
{
    Q_OBJECT
public:
    explicit TPolarGraphImageItem(TPolarAxisAngular *AxisAngular,QGraphicsItem *parent = Q_NULLPTR);

    void setImage(const QString &imgFileName);

protected:
    virtual void draw(QPainter *painter) override;

protected:
    QImage  _img;
    QPixmap _pix;
};

#endif // TPOLARGRAPHIMAGEITEM_H
