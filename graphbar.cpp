#include <QtWidgets>

#include "graphbar.h"

/*
The constructor for the GraphBar class

@param: An x-coordinate, y-coorinate, width, and height (Color is the same for every GraphBar, white)
@return: An instance of the GraphBar class is created
*/
GraphBar::GraphBar(const int x, const int y, const int width, const int height)
{
    this -> x_ = x;
    this -> y_ = y;
    this -> width_ = width;
    this -> height_ = height;
}

//Creates a rectangle around the GraphBar to let Qt know where to paint
QRectF GraphBar::boundingRect() const
{
    return QRectF(x_, y_, width_, height_);
}

//Tells Qt the shape to create when the addItem() function is called
QPainterPath GraphBar::shape() const
{
    QPainterPath path;
    path.addRect(x_, y_, width_, height_);
    return path;
}

//Tells Qt how to paint the Tile
void GraphBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);


    QBrush b = painter->brush();
    painter->setBrush(QBrush(color_));

    painter->drawRect(QRect(this->x_, this->y_, this->width_, this->height_));
    painter->setBrush(b);
}

/*
A function to decrease the x-coordinate of a GraphBar after another is removed from the scene
The +3 maintains the bit of space between each GraphBar

@param: The width of the scene (since this uis used to determine GraphBar width as well)
@return: The GraphBar's width is decreased appropriately
*/
void GraphBar::DecrementX(int width){
    this -> x_ -= (width / 20) + 3;
}
