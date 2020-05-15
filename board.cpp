#include "board.h"
#include "QDebug"

/*
Constructor for the board class
This constructor initializes 400 tiles (20 x 20) that make up the entire board
*/
Board::Board(int width, int height)
{

    //Change numbers here to increase or decrease the number of tiles in the board.
    int tile_width = width/20;
    int tile_height = height/20;
    int num_tiles = 20;

    //Track the x position, y position, and state of every tile initialized
    int x_pos = 0;
    int y_pos = 0;
    int rand_state;

    //Outer loop creates rows
    for(int i = 0; i < num_tiles; i++){
        std::vector<Tile *> row_vector;

        //Reset so that tiles don't populate off-screen to the right
        x_pos = 0;

        //Inner loop creates columns
        for(int j = 0; j < num_tiles; j++){
            rand_state = rand() % 2; //returns either 0 (dead) or 1 (alive)

            //Create a new tile outside of the push_back call so that all signals and slots can be connected
            Tile * tile_to_create = new Tile(x_pos, y_pos, tile_width, tile_height, rand_state);
            connect(tile_to_create, &Tile::tileClick, this, &Board::tileClickSlot);

            row_vector.push_back(tile_to_create);
            x_pos += tile_width;
        }

        tile_vector.push_back(row_vector);
        y_pos += tile_height;
    }
}

/*
A function to calculate the population of alive tiles as both a number and percentage

@param: None
@return: Updates the population_ vector in the board and emits a signal to update the population label
*/
void Board::CalculatePopulation(){

    double total_tiles = 0;
    double alive_count = 0;

    for(unsigned int i = 0; i < tile_vector.size(); i++){
        for(unsigned int j = 0; j < tile_vector[i].size(); j++){
            total_tiles += 1.0;
            if(tile_vector[i][j] -> get_curr_state_() == 1){
                alive_count += 1.0;
            }
        }
    }

    double percentage = (alive_count/total_tiles) * 100.0;

    population_[0] = alive_count;
    population_[1] = percentage;

    //After the population is calculated, the population label should update
    emit populationSignal(population_);

    return;
}

/*
A slot to respond to a tile switching from alive to dead or vice versa

Responds to: tileClick in the tile class
@param: None
@return: Population label and next_states update
 */
void Board::tileClickSlot(){
   this -> CalculatePopulation();
   this -> SetNextStatus();
}

/*
A function to traverse through the board and give each tile a list of 8 neightbors
This will allow tiles to easily calculate their next_state value

@param: None
@return: The neighbors_ vector is set as a std::vector<Tile *> of 8 tile pointers
*/
void Board::SetTileNeighbors(){

    //The maximum index sizes based on the current size of the board
    int row_max = tile_vector.size() - 1;
    int column_max = tile_vector[0].size() - 1;

    //i is rows, j is columns
    for(unsigned int i = 0; i < tile_vector.size(); i++){
        for(unsigned int j = 0; j < tile_vector[i].size(); j++){


            Tile * current_tile = tile_vector[i][j];
            std::vector<Tile *> neighbors;

            int row_pos = i;
            int column_pos = j;

            if(row_pos == 0 && column_pos != 0 && column_pos != column_max){
                //Top row but not a corner
                neighbors.push_back(tile_vector[row_max][column_pos]); //wrap
                neighbors.push_back(tile_vector[row_max][column_pos - 1]); //wrap
                neighbors.push_back(tile_vector[row_max][column_pos + 1]); //wrap
                neighbors.push_back(tile_vector[row_pos][column_pos - 1]);
                neighbors.push_back(tile_vector[row_pos][column_pos + 1]);
                neighbors.push_back(tile_vector[row_pos + 1][column_pos]);
                neighbors.push_back(tile_vector[row_pos + 1][column_pos - 1]);
                neighbors.push_back(tile_vector[row_pos + 1][column_pos + 1]);

            }else if(row_pos == row_max && column_pos != 0 && column_pos != column_max){
                //Bottom row but not a corner
                neighbors.push_back(tile_vector[row_pos - 1][column_pos]);
                neighbors.push_back(tile_vector[row_pos - 1][column_pos - 1]);
                neighbors.push_back(tile_vector[row_pos - 1][column_pos + 1]);
                neighbors.push_back(tile_vector[row_pos][column_pos - 1]);
                neighbors.push_back(tile_vector[row_pos][column_pos + 1]);
                neighbors.push_back(tile_vector[0][column_pos]); //wrap
                neighbors.push_back(tile_vector[0][column_pos - 1]); //wrap
                neighbors.push_back(tile_vector[0][column_pos + 1]); //wrap

            }else if(column_pos == 0 && row_pos != 0 && row_pos != row_max){
                //Leftmost row but not a corner
                neighbors.push_back(tile_vector[row_pos - 1][column_pos]);
                neighbors.push_back(tile_vector[row_pos - 1][column_max]); //wrap
                neighbors.push_back(tile_vector[row_pos - 1][column_pos + 1]);
                neighbors.push_back(tile_vector[row_pos][column_max]); //wrap
                neighbors.push_back(tile_vector[row_pos][column_pos + 1]);
                neighbors.push_back(tile_vector[row_pos + 1][column_pos]);
                neighbors.push_back(tile_vector[row_pos + 1][column_max]); //wrap
                neighbors.push_back(tile_vector[row_pos + 1][column_pos + 1]);

            }else if(column_pos == column_max && row_pos != 0 && row_pos != row_max){
                //Rightmost row but not a corner
                neighbors.push_back(tile_vector[row_pos - 1][column_pos]);
                neighbors.push_back(tile_vector[row_pos - 1][column_pos - 1]);
                neighbors.push_back(tile_vector[row_pos - 1][0]); //wrap
                neighbors.push_back(tile_vector[row_pos][column_pos - 1]);
                neighbors.push_back(tile_vector[row_pos][0]); //wrap
                neighbors.push_back(tile_vector[row_pos + 1][column_pos]);
                neighbors.push_back(tile_vector[row_pos + 1][column_pos - 1]);
                neighbors.push_back(tile_vector[row_pos + 1][0]); //wrap

            }else if(row_pos == 0 && column_pos == 0){
                //uper left tile (can't subtract row or subtract column)
                neighbors.push_back(tile_vector[row_max][column_pos]); //wrap
                neighbors.push_back(tile_vector[row_max][column_max]); //wrap
                neighbors.push_back(tile_vector[row_max][column_pos + 1]); //wrap
                neighbors.push_back(tile_vector[row_pos][column_max]); //wrap
                neighbors.push_back(tile_vector[row_pos][column_pos + 1]);
                neighbors.push_back(tile_vector[row_pos + 1][column_pos]);
                neighbors.push_back(tile_vector[row_pos + 1][column_max]); //wrap
                neighbors.push_back(tile_vector[row_pos + 1][column_pos + 1]);

            }else if(row_pos == row_max && column_pos == 0){
                //lower left tile(can't add row or subtract column)
                neighbors.push_back(tile_vector[row_pos - 1][column_pos]);
                neighbors.push_back(tile_vector[row_pos - 1][column_max]); //wrap
                neighbors.push_back(tile_vector[row_pos - 1][column_pos + 1]);
                neighbors.push_back(tile_vector[row_pos][column_max]); //wrap
                neighbors.push_back(tile_vector[row_pos][column_pos + 1]);
                neighbors.push_back(tile_vector[0][column_pos]); //wrap
                neighbors.push_back(tile_vector[0][column_max]); //wrap
                neighbors.push_back(tile_vector[0][column_pos + 1]); //wrap

            }else if(row_pos == 0 && column_pos == column_max){
                //upper right (can't subtract row or add column)
                neighbors.push_back(tile_vector[row_max][column_pos]); //wrap
                neighbors.push_back(tile_vector[row_max][column_pos - 1]); //wrap
                neighbors.push_back(tile_vector[row_max][0]); //wrap
                neighbors.push_back(tile_vector[row_pos][column_pos - 1]);
                neighbors.push_back(tile_vector[row_pos][0]); //wrap
                neighbors.push_back(tile_vector[row_pos + 1][column_pos]);
                neighbors.push_back(tile_vector[row_pos + 1][column_pos - 1]);
                neighbors.push_back(tile_vector[row_pos + 1][0]); //wrap

            }else if(row_pos == row_max && column_pos == column_max){
                //lower right (can't add row or add column)
                neighbors.push_back(tile_vector[row_pos - 1][column_pos]);
                neighbors.push_back(tile_vector[row_pos - 1][column_pos - 1]);
                neighbors.push_back(tile_vector[row_pos - 1][0]); //wrap
                neighbors.push_back(tile_vector[row_pos][column_pos - 1]);
                neighbors.push_back(tile_vector[row_pos][0]); //wrap
                neighbors.push_back(tile_vector[0][column_pos]); //wrap
                neighbors.push_back(tile_vector[0][column_pos - 1]); //wrap
                neighbors.push_back(tile_vector[0][0]); //wrap

            }else{
                //all other tiles have no wrapping they need to calculate
                neighbors.push_back(tile_vector[row_pos - 1][column_pos]);
                neighbors.push_back(tile_vector[row_pos - 1][column_pos - 1]);
                neighbors.push_back(tile_vector[row_pos - 1][column_pos + 1]);
                neighbors.push_back(tile_vector[row_pos][column_pos - 1]);
                neighbors.push_back(tile_vector[row_pos][column_pos + 1]);
                neighbors.push_back(tile_vector[row_pos + 1][column_pos]);
                neighbors.push_back(tile_vector[row_pos + 1][column_pos - 1]);
                neighbors.push_back(tile_vector[row_pos + 1][column_pos + 1]);
            }

            current_tile -> set_neighbors_(neighbors);

        }
    }

}

/*
A function to simultaneously set the next_state of every single tile on the board
Live cells with less then two alive neighbors die
Live cells with two or three alive neighbors live
Live cells with greater than three alive neighbors die
Dead cells with exactly three alive neighbors revive
Locked tiles can NOT change state

@param: None
@return: The next_state_ field is set for every tile on the board
*/
void Board::SetNextStatus(){

    //Variables to store the information used for calculations throughout the function
    int alive_neighbors = 0;
    Tile * current_tile;
    std::vector<Tile *> current_neighbors;
    int current_state;

    //Go through every single tile on the board
    for(unsigned int i = 0; i < tile_vector.size(); i++){
        for(unsigned int j = 0; j < tile_vector[i].size(); j++){

            //Get information about the current tile
            //Reset the alive_neighbors count for every tile
            current_tile = tile_vector[i][j];
            current_neighbors = current_tile -> get_neighbors_();
            current_state = current_tile -> get_curr_state_();
            alive_neighbors = 0;

            //If a tile is locked, it can NOT change states
            //No reason to do any of the further checks
            if(current_tile -> get_locked_()){
                tile_vector[i][j] -> set_next_state_(current_state);
                continue;
            }else{
                for(unsigned int k = 0; k < current_neighbors.size(); k++){
                    if(current_neighbors[k] -> get_curr_state_()){
                        alive_neighbors += 1;
                    }
                }
            }

            //Two condisitons to make a live cell and two conditions to make a dead cell
            if( (current_state && alive_neighbors < 2) || (current_state && alive_neighbors > 3) ){
                tile_vector[i][j] -> set_next_state_(0);
            }else if(current_state && (alive_neighbors == 2 || alive_neighbors == 3)){
                tile_vector[i][j] -> set_next_state_(1);
            }
            else if(!current_state && alive_neighbors == 3){
                tile_vector[i][j] -> set_next_state_(1);
            }else if(!current_state && alive_neighbors != 3){ //catch dead tiles set to become alive, but tiles are clicked and they should now die
                tile_vector[i][j] -> set_next_state_(0);
            }
        }
    }

}


/*
A function to update each tile individually based on its next_state value
Note that locked tiles do not change

@param: None
@return: Each tile on the board updates its curr_state and color
*/
void Board::UpdateBoard(){

    for(unsigned int i = 0; i < tile_vector.size(); i++){
        for(unsigned int j = 0; j < tile_vector[i].size(); j++){
            if(tile_vector[i][j] -> get_locked_()){
                continue;
            }
            else if(tile_vector[i][j] -> get_next_state_()){
                tile_vector[i][j] -> set_curr_state_(1);
                tile_vector[i][j] -> set_color(tile_vector[i][j] -> alive_);
            }else{
                tile_vector[i][j] -> set_curr_state_(0);
                tile_vector[i][j] -> set_color(tile_vector[i][j] -> dead_);
            }

            tile_vector[i][j]->update();
        }
    }

}

/*
Give every dead tile a 25% chance of becoming alive again

@param: None
@return: dead tiles revive, the population is re-calculated, and the next_status of the tiles is updated
*/
void Board::ReviveBoard(){

    for(unsigned int i = 0; i < tile_vector.size(); i++){
        for(unsigned int j = 0; j < tile_vector[i].size(); j++){

            int rand_int = rand() % 4; //By using a modulo, there are only 4 possible numbers, thus a 25% chance

            if(rand_int == 0 && tile_vector[i][j] -> get_curr_state_() == 0 && !(tile_vector[i][j] -> get_locked_())){
                tile_vector[i][j] -> set_curr_state_(1);
                tile_vector[i][j] -> set_color(tile_vector[i][j] -> alive_);
                tile_vector[i][j] -> update();
            }
        }
    }

    //As a number of tile states have changed, the population and next_states must also change
    this -> CalculatePopulation();
    this -> SetNextStatus();

}
