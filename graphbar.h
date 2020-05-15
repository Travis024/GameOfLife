#ifndef GRAPHBAR_H
#define GRAPHBAR_H

#include <QObject>
#include <QGraphicsItem>
#include <QColor>

class GraphBar : public QObject, public QGraphicsItem
{

    Q_OBJECT

public:
    GraphBar(const int x, const int y, const int width, const int height);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    void DecrementX(int width); //used for changing the x_coordinate when a GraphBar is removed from the scene

private:

    //Specify the x and y coordinates of the top-left corner
    int x_;
    int y_;

    //Each GraphBar will have the same width and a height dependent on the population
    int width_;
    int height_;

    //All GraphBars have the same color
    QColor color_ = QColor("white");
};

#endif // GRAPHBAR_H
