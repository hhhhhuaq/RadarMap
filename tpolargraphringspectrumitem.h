#ifndef TPOLARGRAPHRINGSPECTRUMITEM_H
#define TPOLARGRAPHRINGSPECTRUMITEM_H

#include "QCPColorGradient.h"
#include "tpolargraphringitem.h"
#include "tpolaraxisangularspectrumradar.h"
#include <QtConcurrent/QtConcurrent>
static const int ImgW = 1025;
static const int ImgH = 1142;
///
/// \brief The TPolarRingSpectrum class
/// 环状谱图
class TPolarGraphRingSpectrumItem : public TPolarGraphRingItem
{
    Q_OBJECT
public:
    explicit TPolarGraphRingSpectrumItem(TPolarAxisAngularSpectrumRadar *AxisAngular,QGraphicsItem *parent = Q_NULLPTR);

    virtual ~TPolarGraphRingSpectrumItem();

    void setData(const QVector<double> &data);
    void setColorRange(const QCPRange &ColorRange);

protected:
    void caculateImage();

    virtual qreal Radius()const Q_DECL_OVERRIDE{return _pAxisAngular->SpectrumRadius();}
    virtual void draw(QPainter *painter) Q_DECL_OVERRIDE
    {
        TPolarGraphRingItem::draw(painter);
        painter->drawPixmap(_rect.toRect(),_pm);
    }

private:

    QPixmap                         _pm;
    QImage                          _img;
    QVector<QRgb>                   _Rgbs;
    QVector<double>                 _data;
    QCPRange                        _ColorRange;
    QCPColorGradient                _gradient;
    QFutureWatcher<void>            *_pWfuture;
    TPolarAxisAngularSpectrumRadar  *_pAxisAngular;
};


#endif // TPOLARGRAPHRINGSPECTRUMITEM_H
