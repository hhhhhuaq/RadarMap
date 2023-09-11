#include "tcanvas.h"
#include <QGraphicsSceneMouseEvent>
#include <QtDebug>
#include <QGraphicsRectItem>
static const QRectF DefaultSceneRect(QPoint(-1200,-1200),QSizeF(1200*2,1200*2));//默认画布大小

TCanvas::TCanvas(QObject *parent)
    :QGraphicsScene(parent),
      _backgroundBrush(QColor(Qt::black))
{
    this->setBackgroundBrush(_backgroundBrush);
    this->setSceneRect(DefaultSceneRect);
//    QRectF r = QRect(0,0,100,100);

//    auto e = this->addRect(r,QPen(Qt::red,5));
//    e->setPos(QPointF(1200,0) - r.center());
//    qDebug()<<1;
}

TCanvas::TCanvas(const QRectF &sceneRect, QObject *parent)
    :QGraphicsScene(sceneRect,parent),
      _backgroundBrush(QColor(Qt::black))
{
    this->setBackgroundBrush(_backgroundBrush);
    this->setSceneRect(DefaultSceneRect);
}

TCanvas::TCanvas(qreal x, qreal y, qreal width, qreal height, QObject *parent)
    :QGraphicsScene(x,y,width,height,parent),
      _backgroundBrush(QColor(Qt::black))
{
    this->setBackgroundBrush(_backgroundBrush);
    this->setSceneRect(DefaultSceneRect);
}

void TCanvas::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    qDebug()<<__FILE__<<event->pos();
}
