#include "tpolarplot.h"

#include <QtMath>
#include <QScrollBar>


TPolarPlot::TPolarPlot(QWidget *parent)
    : QGraphicsView(parent),
      _pScene(new TCanvas(this)),
      _pRectItem(new QGraphicsRectItem()),
      _BoundingRectVisible(false),
      _pGraphicsOpacityEffect(new QGraphicsOpacityEffect),
      _opacity(1.0)
{
    setRenderHints(QPainter::Antialiasing |
                   QPainter::SmoothPixmapTransform |
                   QPainter::HighQualityAntialiasing);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);

    setDragMode(QGraphicsView::NoDrag);//(QGraphicsView::RubberBandDrag);//QGraphicsView::ScrollHandDrag
    setStyleSheet("padding: 0px; border: 0px;");//无边框
    setMouseTracking(true);//跟踪鼠标位置
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏水平条
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏竖条

    this->setScene(_pScene);
    //scene外边框(画布框
    _pScene->addItem(_pRectItem);
    _pRectItem->setPen(QColor(Qt::red));

    this->setGraphicsEffect(_pGraphicsOpacityEffect);
    _pGraphicsOpacityEffect->setOpacity(_opacity);
}

TPolarPlot::~TPolarPlot()
{

}

bool TPolarPlot::BoundingRectVisible() const
{
    return this->_BoundingRectVisible;
}

qreal TPolarPlot::Opacity() const
{
    return this->_opacity;
}

void TPolarPlot::setBoundingRectVisible(const bool &visible)
{
    this->_BoundingRectVisible = visible;
}

void TPolarPlot::setOpacity(const qreal &opacity)
{
    this->_opacity = opacity;
    this->_pGraphicsOpacityEffect->setOpacity(opacity);
}

TCanvas *TPolarPlot::canvas() const
{
    return this->_pScene;
}


void TPolarPlot::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);
    emit KeyPress(event);
}


void TPolarPlot::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    if(_pScene == nullptr) return;

    QRectF rect = _pScene->sceneRect();
    qreal Radius = qMin(rect.width(),rect.height());
    qreal pandding = Radius*(1.0-OutterRectRatio);
    this->fitInView(_pScene->sceneRect().adjusted(-pandding,-pandding,pandding,pandding),Qt::AspectRatioMode::KeepAspectRatio);

    if(_BoundingRectVisible == true) _pRectItem->setRect(rect);
}

void TPolarPlot::showEvent(QShowEvent *event)
{
    QGraphicsView::showEvent(event);
}

void TPolarPlot::wheelEvent(QWheelEvent *event)
{
    QGraphicsView::wheelEvent(event);

}

void TPolarPlot::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
//    qDebug()<<__FILE__<<_pScene->;
}







