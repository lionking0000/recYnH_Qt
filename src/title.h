#ifndef TITLE_H
#define TITLE_H

#include <QGraphicsTextItem>

class Title : public QGraphicsTextItem
{
public:
    //Title();
    //Title(const QString & text, QGraphicsItem * parent);
    Title(const QString& text): QGraphicsTextItem(text){ setFlags(ItemIsSelectable | ItemIsMovable); }

    //QRectF boundingRect() const Q_DECL_OVERRIDE;
    //QPainterPath shape() const Q_DECL_OVERRIDE;
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) Q_DECL_OVERRIDE;

protected:
    //void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    //void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

private:

    QColor color;

};

#endif // TITLE_H
