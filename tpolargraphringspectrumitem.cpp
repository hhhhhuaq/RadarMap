#include "tpolargraphringspectrumitem.h"

TPolarGraphRingSpectrumItem::TPolarGraphRingSpectrumItem(TPolarAxisAngularSpectrumRadar *AxisAngular, QGraphicsItem *parent)
    :TPolarGraphRingItem(AxisAngular,parent),
      _ColorRange(-30,160),
      _gradient(QCPColorGradient::gpSpectrum),
      _pAxisAngular(AxisAngular)
{
    QPen pen = this->RingBorderPen();
    pen.setWidth(5);
    this->setRingBorderPen(pen);
    this->setRingWidth(_pAxisAngular->SpectrumWidth());

    _pWfuture = new QFutureWatcher<void>;
    connect(_pWfuture,&QFutureWatcher<void>::finished,this,[&](){
        _pAxisAngular->scene()->update();
    });
}

TPolarGraphRingSpectrumItem::~TPolarGraphRingSpectrumItem()
{
    if(_pWfuture != nullptr) delete _pWfuture;
}

void TPolarGraphRingSpectrumItem::setData(const QVector<double> &data)
{
    _data = data;
    auto future = QtConcurrent::run(this,&TPolarGraphRingSpectrumItem::caculateImage);
    _pWfuture->setFuture(future);
}

void TPolarGraphRingSpectrumItem::setColorRange(const QCPRange &ColorRange)
{
    this->_ColorRange = ColorRange;
}

void TPolarGraphRingSpectrumItem::caculateImage()
{
    const int W = ImgW;
    const int H = ImgH;
    _pm = QPixmap(W,H);
    _pm.fill(Qt::transparent);
    QPainter pain(&_pm);
    pain.setWindow(_rect.toRect());

    //外内径差 步长
    qreal h = _RingWidth;
    qreal dh = h/(H*1.0);

    //角度步长
    qreal dangle = _angle/(W*1.0);
    qreal angle;
    //坐标
    qreal x,y;
    for(int i = 0 ; i < H ; i++)
    {
        for(int k = 0 ; k < W ; k++)
        {
            angle = (dangle*(k*1.0))/180.0*M_PI;
            x = (_InnerRadius + i*dh)*qSin(angle);
            y =-(_InnerRadius + i*dh)*qCos(angle);
            pain.setPen(QColor(_gradient.color(_data[i * W + k],_ColorRange)));
            pain.drawPoint(x,y);
        }
    }

}
