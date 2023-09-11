#ifndef TABSTRACTGRAPH_H
#define TABSTRACTGRAPH_H

#include <QObject>
#include <QAbstractGraphicsShapeItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QPainter>
#include "QCPColorGradient.h"

///
/// \brief The TAbstractGraph class
/// 画图抽象基类
class TAbstractGraph : public QObject,public QAbstractGraphicsShapeItem
{
    Q_OBJECT
public:
    explicit TAbstractGraph(QGraphicsItem *parent = Q_NULLPTR);
    virtual ~TAbstractGraph();

    virtual QRectF boundingRect() const override;
    virtual void draw(QPainter *painter) = 0;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;
    virtual void addAction(QAction *action);
    virtual void addActions(const QList<QAction*> &actions);

    inline QMenu *Menu()const {return this->_pMenu;}

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event)Q_DECL_OVERRIDE
    {
        Q_UNUSED(event);

        QAction *selectedAction = _pMenu->exec(QCursor::pos());
        for(QAction *e : _pMenu->actions())
        {
            if(selectedAction == e)
            {
            }
        }
    }
    QRectF              _rect;//图形外接矩形 _rect实际为item坐标框
    QPainterPath        _path;//不规则图形连接点路径
    QMenu               *_pMenu;//右键菜单
};
#endif // TABSTRACTGRAPH_H
