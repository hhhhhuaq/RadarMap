#ifndef TPOLARGRAPHRINGEMEITEM_H
#define TPOLARGRAPHRINGEMEITEM_H
#include "tpolargraphringitem.h"
#include "tpolaraxisangularspectrumradar.h"

///
/// \brief The TPolarGraphRingEmeItem class
/// 环境谱类
class TPolarGraphRingEmeItem : public TPolarGraphRingItem
{
    Q_OBJECT
public:
    explicit TPolarGraphRingEmeItem(TPolarAxisAngularSpectrumRadar *AxisAngular,QGraphicsItem *parent = Q_NULLPTR);

protected:
    virtual qreal Radius()const override{return _pAxisAngular->EmeRadius();}
private:
    QCPRange                        _range;//设定图像范围
    TPolarAxisAngularSpectrumRadar  *_pAxisAngular;

};

#endif // TPOLARGRAPHRINGEMEITEM_H
