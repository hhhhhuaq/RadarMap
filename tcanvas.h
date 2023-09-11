#ifndef TCANVAS_H
#define TCANVAS_H
#include <QGraphicsScene>

///
/// \brief The TCanvas class
/// 画布
class TCanvas : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit TCanvas(QObject *parent = Q_NULLPTR);
    explicit TCanvas(const QRectF &sceneRect, QObject *parent = Q_NULLPTR);
    explicit TCanvas(qreal x, qreal y, qreal width, qreal height, QObject *parent = Q_NULLPTR);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)override;

private:
    QBrush        _backgroundBrush;
};
#endif // TCANVAS_H
