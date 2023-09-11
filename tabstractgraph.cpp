#include "tabstractgraph.h"



TAbstractGraph::TAbstractGraph(QGraphicsItem *parent)
    :QAbstractGraphicsShapeItem(parent),
      _rect(.0,.0,.0,.0),
      _pMenu(new QMenu)
{
}

TAbstractGraph::~TAbstractGraph()
{
    if(_pMenu != nullptr)delete _pMenu;
}

QRectF TAbstractGraph::boundingRect() const
{
    return _rect;
}

void TAbstractGraph::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    if(painter->isActive())
    {
        draw(painter);
    }
}

void TAbstractGraph::addAction(QAction *action)
{
    _pMenu->addAction(action);
}

void TAbstractGraph::addActions(const QList<QAction *> &actions)
{
    _pMenu->addActions(actions);
}
