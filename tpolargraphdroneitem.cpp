#include "tpolargraphdroneitem.h"

TPolarGraphDroneItem::TPolarGraphDroneItem(TPolarAxisAngular *pAxisAngular, QGraphicsItem *parent)
    :TPolarGraphImageItem(pAxisAngular,parent)
    , _pAxisAngular(pAxisAngular)
    ,_pActions(new TDroneActions(this))
    ,_LocusVisible(false)
    ,_pPolyLine(new TPolarGraphPolylineItem(pAxisAngular))
{
    this->setBorderVisible(true);
    this->setImage(ImageNomalDroneFileName);
    this->addThreatenRange(QCPRange(0,10000),TPolarGraphDroneItem::High);
    this->addThreatenRange(QCPRange(10000,20000),TPolarGraphDroneItem::Mid);
    this->addThreatenRange(QCPRange(20000,30000),TPolarGraphDroneItem::Low);
//    connect(_pAxisAngular,&TPolarAxisAngularSpectrumRadar::RadarRadiusChange,this,[&](const qreal &r)
//    {
//        //rd真实距离 rd/LogicRadius得出对应比例 Radius*Range().upper为雷达半径
//        TPolarPoint p = this->LogicPoint();
//        qreal R = r * p.Radius()/_pAxisAngular->Radius();
//        this->setPolarPos(R,p.Angle());
//    });
    this->addActions(*_pActions);

}

TPolarGraphDroneItem::~TPolarGraphDroneItem()
{
    if(_pActions != nullptr) delete _pActions;
}

void TPolarGraphDroneItem::addThreatenRange(const QCPRange &range, const TPolarGraphDroneItem::ThreatenLevel &level)
{
    this->_ThreatenRanges.insert(level,range);
}

void TPolarGraphDroneItem::addThreatenRange(const qreal &lower, const qreal &upper, const TPolarGraphDroneItem::ThreatenLevel &level)
{
    addThreatenRange(QCPRange(lower,upper),level);
}

void TPolarGraphDroneItem::addData(const qreal &rd, const qreal &angle)
{
//    _LogicPoints.push_back(TPolarPoint(rd,angle));
//    TPolarPoint p = LogicPoint();
//    //rd真实距离 rd/LogicRadius得出对应比例 Radius*Range().upper为雷达半径
//    qreal R = _pAxisAngular->RadarRadius() * p.Radius()/_pAxisAngular->LogicRadius();
//    this->setPolarPos(R,angle);
//    _pPolyLine->addPoint(this->pos()+this->boundingRect().center());
}

void TPolarGraphDroneItem::setData(const QVector<TPolarPoint> &points)
{
    _LogicPoints = points;
}
