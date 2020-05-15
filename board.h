#ifndef BOARD_H
#define BOARD_H

#include <vector>

#include <tile.h>

class Board : public QObject
{

    Q_OBJECT

public:

    Board(int width, int height); //constructor

    std::vector<std::vector<Tile *>> get_tile_vector_(){return tile_vector; } //return the tile_vector; used by MainWindow in constructor to pass as an argument to DrawBoard

    double get_population_percentage_(){return population_[1] / 100.0; } //Gets just the percentage of alive tiles; used for creating new GraphBars. Remember that population is stored as full percentage, so divide
    void CalculatePopulation(); //Determine how many tiles are alive as a count and percentage of total

    void SetTileNeighbors(); //Go through every tile and initialize its neighbors_ vector

    void SetNextStatus(); //Go through every tile and determine if it dies or stays alive

    void UpdateBoard(); //Update each tile's current state simultaneously

    void ReviveBoard(); //Dead tiles have a 25% chance of becoming alive again

signals:

    void populationSignal(std::vector<double> probability); //Tell the population label what to update to

private slots:

    void tileClickSlot(); //Update population label, next_states of tiles

private:

    //The width and height of the board are the same as the width and height of the scene
    int width_;
    int height_;

    //The population of alive tiles stored as a number and a percentage of total
    //Indeces are updated, NOT appended
    std::vector<double> population_ = {0,0};

    //A two-dimensional vector; determine the size of each row and the number of columns in the constructor
    std::vector<std::vector<Tile * >> tile_vector = {};
};

#endif // BOARD_H
