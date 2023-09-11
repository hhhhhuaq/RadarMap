#ifndef TPOLARGRAPHITEM_H
#define TPOLARGRAPHITEM_H

#include "tpolaraxis.h"
#include <QtDebug>
#include <QGraphicsView>

///
/// \brief The TPolarGraph class
/// 图像基类
class TPolarGraphItem :public TAbstractGraph
{
    Q_OBJECT
public:
    explicit TPolarGraphItem(TPolarAxisAngular *AxisAngular,QGraphicsItem *parent = Q_NULLPTR);
    void setPolarPos(const qreal &radius,const qreal &angle);
    void setPolarPos(const TPolarPoint &pos);
    void setBorderPen(const QPen &pen);
    void setBorderColor(const QColor &color);
    void setBorderVisible(const bool &visible);
    void setPadding(const bool &padding);
    void setSize(const QSizeF &sz);
    void setBrush(const QBrush &brush);

    inline QPen BorderPen()const{return this->_BorderPen;}
    inline QColor BorderColor()const{return this->_BorderPen.color();}
    inline TPolarPoint PolarPos()const{return _Point;}
    inline bool BorderVisible()const{return _BorderVisible;}
    inline bool Padding()const{return this->_padding;}
    inline QSizeF SizeF()const{return this->_rect.size();}
    inline QBrush Brush()const{return this->_brush;}


signals:
    void mousePress(QGraphicsSceneMouseEvent *e);
    void mouseDoubleClick(QGraphicsSceneMouseEvent *e);
    void mouseRelease(QGraphicsSceneMouseEvent *e);
    void mouseMove(QGraphicsSceneMouseEvent *e);

protected:
    virtual qreal Radius()const{return this->_pAxisRadius->Radius();}
    virtual void draw(QPainter *painter)override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)override
    {
        QAbstractGraphicsShapeItem::mousePressEvent(event);
        emit mousePress(event);
    }
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override
    {
        QAbstractGraphicsShapeItem::mouseMoveEvent(event);
        emit mouseMove(event);
    }
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override
    {
        QAbstractGraphicsShapeItem::mouseReleaseEvent(event);
        emit mouseRelease(event);
    }
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE
    {
        QAbstractGraphicsShapeItem::mouseDoubleClickEvent(event);
        emit mouseDoubleClick(event);
    }
    virtual QRectF boundingRect()const override{return _rect;}

protected:
    TPolarPoint         _Point;             //坐标位置
    QPen                _BorderPen;         //边框画笔
    TPolarAxisAngular   *_pAxisAngular;     //极坐标
    TPolarAxisRadius    *_pAxisRadius;      //极径
    bool                _BorderVisible;     //是否显示边框
    bool                _padding;           //是否填充
    QBrush              _brush;             //填充笔刷
};



#endif // TPOLARGRAPHITEM_H
