#ifndef TPOLARAXISANGULARSPECTRUMRADAR_H
#define TPOLARAXISANGULARSPECTRUMRADAR_H

#include "tpolaraxis.h"
#include "QCPColorGradient.h"

class TPolarGraphRingSpectrumItem;
class TPolarGraphRingItem;
static qreal DirectFontSpaceRatio(15.0/1000.0);//方向字体间距比例
static qreal DirectFontSizeRatio(0.02);//方位字体大小比例
static const QCPRange   RadarRatiorange(0.0,0.6);//雷达图显示范围,极坐标极径长度
static const QCPRange   EmeRatiorange(0.65,0.7);//环境干扰图范围,极坐标极径长度范围
static const QCPRange   SpectrumRatiorange(0.7,1.0);//外层谱图范围,极坐标极径长度范围

///
/// \brief The TPolarAxisSpectrumRadarAngular class
/// 特殊极坐标，同时显示频谱,干扰色谱,雷达
class TPolarAxisAngularSpectrumRadar : public TPolarAxisAngular
{
    Q_OBJECT
public:
    explicit TPolarAxisAngularSpectrumRadar(TCanvas *scene,const QCPRange &range = QCPRange(0.0,1.0),QGraphicsItem *parent = Q_NULLPTR)
        :TPolarAxisAngular(scene,parent)
        ,_RadarRange(range)
        ,_LogicRadius(1.0)
        ,_DistanceIncCount(5)
    {
    }

    void setRadarRange(const QCPRange &range)
    {
        qreal R = this->_pAxisRadius->Radius() * range.upper;
        this->_RadarRange = range;
        emit RadarRadiusChange(R);
    }
    void setEmeRange(const QCPRange &range)
    {
        qreal R = this->_pAxisRadius->Radius() * range.upper;
        this->_EmeRange = range;
        emit EmeRadiusChange(R);
    }
    void setSpectrumRange(const QCPRange &range)
    {
        qreal R = this->_pAxisRadius->Radius() * range.upper;
        this->_SpectrumRange = range;
//        emit SpectrumRadiusChange(R);
    }
    void setLogicRadius(const qreal &radius)
    {
        this->_LogicRadius = radius;
    }
    void setDistanceIncCount(const qint32 &count){this->_DistanceIncCount = count;}
    ///
    /// \brief setSpecVisible
    /// \param spec
    /// \param eme
    /// 显示或隐藏 环境谱 幅度谱
    void setSpecVisible(const bool &spec,const bool &eme)
    {
        if(spec == true && eme == true)//全显示
        {
            this->setRadarRange(RadarRatiorange);
            this->setSpectrumRange(SpectrumRatiorange);
            this->setEmeRange(EmeRatiorange);
            this->setDistanceIncCount(5);
        }
        else if(spec == false && eme == true)//幅度谱不显示 环境谱显示
        {
            QCPRange newRadarRatiorange(RadarRatiorange.lower,RadarRatiorange.upper + SpectrumRatiorange.size());
            QCPRange newSpectrumRatiorange(.0,.0);
            QCPRange newEmeRatiorange(EmeRatiorange+SpectrumRatiorange.size());

            this->setRadarRange(newRadarRatiorange);
            this->setSpectrumRange(newSpectrumRatiorange);
            this->setEmeRange(newEmeRatiorange);
            this->setDistanceIncCount(8);
        }
        else if(spec == true && eme == false)//幅度谱显示 环境谱不显示
        {

        }
        else if(spec == false && eme == false)//都不显示
        {
            QCPRange newRadarRatiorange(0.0,0.95);
            QCPRange newSpectrumRatiorange(.0,.0);
            QCPRange newEmeRatiorange(0.0,0.0);

            this->setRadarRange(newRadarRatiorange);
            this->setSpectrumRange(newSpectrumRatiorange);
            this->setEmeRange(newEmeRatiorange);
            this->setDistanceIncCount(9);
        }
    }

    QCPRange RadarRange()const{return _RadarRange;}
    QCPRange EmeRange()const{return _EmeRange;};
    QCPRange SpectrumRange()const{return _SpectrumRange;}
    qint32  DistanceIncCount()const{return this->_DistanceIncCount;}

    ///
    /// \brief LogicRadius
    /// \return
    /// 真实长度
    qreal LogicRadius()const{return _LogicRadius;}

    ///
    /// \brief RadarRadius
    /// \return
    /// 雷达像素长度
    qreal RadarRadius()const{return this->_pAxisRadius->Radius() * _RadarRange.upper;}

    ///
    /// \brief EmeRadius
    /// \return
    /// 环境谱外径像素长度
    qreal EmeRadius()const {return this->_pAxisRadius->Radius() * EmeRange().upper;}

    ///
    /// \brief SpectrumRadius
    /// \return
    /// 频谱外径像素长度
    qreal SpectrumRadius()const {return this->_pAxisRadius->Radius() * SpectrumRange().upper;}

    ///
    /// \brief EmeWidth
    /// \return
    /// 环境谱环宽度 像素长度
    qreal EmeWidth()const {return this->_pAxisRadius->Radius() * EmeRange().size();}

    ///
    /// \brief SpectrumWidth
    /// \return
    /// 频谱环宽度 像素长度
    qreal SpectrumWidth()const{return this->_pAxisRadius->Radius() * SpectrumRange().size();}


    //幅度/相关谱图
    void addRingSpeturm(TPolarGraphRingSpectrumItem *item)
    {
        _RingSpectrumItems.append(item);
    }
    void removeRingSpeturm(const int &index)
    {
        if(index < 0 || index > _RingSpectrumItems.size())return;
//        TPolarGraphRingSpectrumItem *p = _RingSpectrumItems[index];
//        if(p != nullptr) delete p;
        _RingSpectrumItems.removeAt(index);
    }
    TPolarGraphRingSpectrumItem *RingSpeturm(const int &index)const
    {
        if(index < 0 || index > _RingSpectrumItems.size())return nullptr;
        return _RingSpectrumItems.at(index);
    }
    QList<TPolarGraphRingSpectrumItem*> RingSpeturms()const
    {
        return this->_RingSpectrumItems;
    }

    //环境谱
    void addRingEme(TPolarGraphRingItem *item)
    {
        _RingItems.append(item);

    }
    void removeRingEme(const int &index)
    {
        if(index < 0 || index > _RingItems.size())return;
        _RingItems.removeAt(index);
    }
    TPolarGraphRingItem *RingEme(const int &index)const
    {
        if(index < 0 || index > _RingItems.size())return nullptr;
        return _RingItems.at(index);
    }
    QList<TPolarGraphRingItem*> RingEmes()const
    {
        return this->_RingItems;
    }

signals:
    void RadarRadiusChange(const qreal &R);
    void EmeRadiusChange(const qreal &R);
    void SpectrumRadiusChange(const qreal &R);

protected:
    virtual qreal Radius() const override{return TPolarAxisAngular::Radius()*_RadarRange.upper;}
    virtual void draw(QPainter *painter) override
    {
        caculateRadiusLable();
        TPolarAxisAngular::draw(painter);         
        //方向
        painter->save();
        qreal AbsoluteRadius = _pAxisRadius->Radius();//半径绝对长度
        QFont DirectLableFont;
        DirectLableFont.setPixelSize(AbsoluteRadius*DirectFontSizeRatio*3);
        QPen DirectLablePen(_pen.color());
        painter->setFont(DirectLableFont);
        painter->setPen(DirectLablePen);
        QFontMetricsF fm(DirectLableFont);
        QString N = "N",W = "W",E = "E",S = "S";
        qreal DirectLableFontSpace = DirectFontSpaceRatio * AbsoluteRadius;
        painter->drawText(-fm.width(N)/2,-AbsoluteRadius-DirectLableFontSpace,N);//上
        painter->drawText(-fm.width(S)/2,AbsoluteRadius+DirectLableFontSpace+fm.height()/2,S);//下
        painter->drawText(-AbsoluteRadius-DirectLableFontSpace-fm.width(W),+fm.height()/4,"W");//左
        painter->drawText(AbsoluteRadius+DirectLableFontSpace,+fm.height()/4,"E");//右
        painter->restore();
    }

private:
    void caculateRadiusLable()
    {

        int sz = QString::number(_LogicRadius).size()-2;
        sz = sz < 0 ? 0 :sz;
        qreal inc = this->_LogicRadius/(_DistanceIncCount*1.0*qPow(10,sz));
        QStringList sl;
        for(int i = 0 ; i <_DistanceIncCount;i++)
        {
            sl.push_back(QString::number(i*inc,'f',0)+"KM");
        }
        this->setRadiusLable(sl);
    }
private:
    QCPRange            _RadarRange;        //主雷达图显示范围比例
    QCPRange            _EmeRange;          //环境谱环境
    QCPRange            _SpectrumRange;     //频谱返回
    qreal               _RadarRadius;       //雷达半径,实际半径
    qreal               _LogicRadius;       //逻辑半径//对应真实距离
    qint32              _DistanceIncCount;  //距离步长数量
    QList<TPolarGraphRingSpectrumItem*> _RingSpectrumItems;
    QList<TPolarGraphRingItem*>         _RingItems;
};


#endif // TPOLARAXISANGULARSPECTRUMRADAR_H
