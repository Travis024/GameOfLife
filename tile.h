#ifndef TILE_H
#define TILE_H

#include <QObject>
#include <QGraphicsItem>
#include <QColor>

class Tile : public QObject, public QGraphicsItem
{

    Q_OBJECT


public:
    Tile(const int x, const int y, const int width, const int height, const int state); //constructor

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    void set_color(QColor color){color_ = color; } //Update the color of the tile
    void set_neighbors_(std::vector<Tile *>neightbors){neighbors_ = neightbors; } //Update the neighbors vector
    void set_curr_state_(int curr_state){curr_state_ = curr_state; } //Update the current state of the tile
    void set_next_state_(int state){next_state_ = state; } //Update the next state of the tile

    int get_curr_state_(){return curr_state_; } //Get the current state of the tile
    int get_next_state_(){return next_state_; } //Get the next state of the tile
    bool get_locked_(){return locked_ ;} //Get the locked state of the tile
    std::vector<Tile *> get_neighbors_(){return neighbors_ ;} //Get the neighbors of the tile

    //All the possible tiles colors of a tile
    //Public because these are just renamings of already available colors
    //Make for increased readability across classes
    QColor dead_ = QColor("white");
    QColor dead_locked_ = QColor("gray");
    QColor alive_ = QColor("magenta");
    QColor alive_locked_ = QColor("darkMagenta");

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override; //Used for changing between dead and alive or locking and un-locking

signals:

    void tileClick(); //Update the population label, update next_states when tile switches state

private:

    //Specify the x and y coordinates of the top-left corner
    int x_;
    int y_;

    //Each tile will have a width and height one-twentieth of the total board size
    int width_;
    int height_;

    //color_ is the current color of the tile
    //Presets are used for readability in code
    QColor color_;

    //Store the current and next state of the Tile. 0 means dead, 1 means alive.
    int curr_state_ = 0;
    int next_state_;

    //Every tile starts out as not being locked
    bool locked_ = false;

    //Every tile has 8 neighbors
    //These neighbors "wrap around" the board if the tile is on the edge
    std::vector<Tile *> neighbors_;
};

#endif // TILE_H
