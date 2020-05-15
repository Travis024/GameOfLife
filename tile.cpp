#include <QtWidgets>

#include "tile.h"

/*
The constructor for the Tile class; creates a new rectangle with the given characteristics

@param: An x coordinate, a y coordinate, a width, a height, and an int for state
@return: Creates a Tile with the given characteristics
*/
Tile::Tile(const int x, const int y, const int width, const int height, const int state)
{
    this->x_ = x;
    this->y_ = y;
    this->width_ = width;
    this->height_ = height;

    if(state == 0){
        color_ = dead_;
    }else{
        color_ = alive_;
    }

    this->curr_state_ = state;

}

//Creates a rectangle around the Tile to let Qt know where to paint
QRectF Tile::boundingRect() const
{
    return QRectF(x_, y_, width_, width_);
}

//Tells Qt the shape to create when the addItem() function is called
QPainterPath Tile::shape() const
{
    QPainterPath path;
    path.addRect(x_, y_, width_, width_);
    return path;
}

//Tells Qt how to paint the Tile
void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);


    QBrush b = painter->brush();
    painter->setBrush(QBrush(color_));

    painter->drawRect(QRect(this->x_, this->y_, this->width_, this->width_));
    painter->setBrush(b);
}

/*
The user can either left-click or right-click a tile
On a left-click, the tile changes between dead and alive
On a right-click, the tile becomes locked and cannot change unless unlocked

@param: A graphicsSceneMouseEvent
@return: The color of the Tile that is clicked on changes, or the tile becomes locked/un-locked
*/
void Tile::mousePressEvent(QGraphicsSceneMouseEvent *event){

    //USED FOR DEBUGGING!
    if(event->modifiers() == Qt::CTRL){

        for(unsigned int i = 0; i < neighbors_.size(); i++){
            neighbors_[i]->set_color(QColor("cyan"));
            neighbors_[i] -> update();
        }

        qDebug() << "Next state:";
        qDebug() << this -> get_next_state_();
        qDebug() << "\n";
        return;
    }

    //Shift button click locks the tile, which does NOT change the population
    //Because a shift click can occur with a left or right click, this must come first and return when finished running
   if(event->modifiers() == Qt::ShiftModifier){
       if(locked_){
           if(curr_state_ == 0){
               color_ = dead_;
           }else{
               color_ = alive_;
           }
       }else{
           if(curr_state_ == 0){
               color_ = dead_locked_;
           }else{
               color_ = alive_locked_;
           }
       }

       //update the locked status AFTER the change is made so that the changes are not blocked
       locked_ = !locked_;
       this -> set_next_state_(curr_state_); //update the next state without affecting other tiles, as state did not change
       update();
       return;
   }

    //Left button click makes a tile alive, which affects the population
    //An already live tile does NOT change
    if(event->button() == Qt::LeftButton && !(locked_)){
        if(curr_state_ == 0){
            curr_state_ = 1;
            color_ = alive_;
            emit tileClick(); //Update the population label, next_states of other tiles
        }
        update();
        return;
    }

    //Right button click makes a tile dead, which affects the population
    //An already dead tile does NOT change
    if(event->button() == Qt::RightButton && !(locked_)){
        if(curr_state_ == 1){
            curr_state_ = 0;
            color_ = dead_;
            emit tileClick(); //Update the population label, next_states of other tiles
        }
        update();
        return;
    }
}
