#include "graphcontainer.h"

/*
Constructor for the GraphContainer

@param: The width and height of the graphScene
@return: A new GraphContainer is created
*/
GraphContainer::GraphContainer(int width, int height)
{
    this -> width_ = width;
    this -> height_ = height;
}

/*
A function for creating a new GraphBar and adding it to the GraphContainer's vector of GraphBars

@param: The current population of the game
@return: If there are less than 18 GraphBars, a new GraphBar is created and added to the vector
         If there are 18 GraphBars, the oldest GraphBar is deleted and a new GraphBar is added to the end of the vector
*/
void GraphContainer::AddGraphBar(double population){

    //Once the screen is filled with GraphBars, the oldest must be removed
    //This occurs before other calculations to ensure the x-coordinate of the newest GraphBar is set properly
    if(graphbar_vector_.size() == graphbar_limit_){
        RemoveGraphBar();
    }

    //Determine the width and height of the GraphBar based on characterisitcs of the scene
    int graphbar_width = width_ / 20;
    double graphbar_height_double = height_ * population; //Store as a double first, otherwise this won't change the value
    int graphbar_height = int(graphbar_height_double); //Cast to an int with minor round-off

    //Every GraphBar has a width of 1/20 the width of the GraphContainer.
    //Since x is the leftmost point of the GraphBar, each GraphBar must be additionally offset by half the width of the GraphBar multiplied by the total number of GraphBars
    int graphbar_x = (20 * graphbar_counter_) + ((graphbar_width / 2) * graphbar_counter_);
    int graphbar_y = height_ - graphbar_height; //Ensreu the GraphBar touches the very bottom of the GraphContainer

    graphbar_vector_.push_back(new GraphBar(graphbar_x, graphbar_y, graphbar_width, graphbar_height));

    graphbar_counter_ ++;

}

/*
A function to remove a GraphBar from the vector and update the x-coordinate of the remaining GraphBars

@param: None
@return: A GraphBar is removed from the front of the graphbar_vector and the x-coordinated of the remaining GraphBars are updated
*/
void GraphContainer::RemoveGraphBar(){


    //Ensure the item is removed both from mthe vector and from memory
    GraphBar * graphbar_to_delete = graphbar_vector_[0];
    graphbar_vector_.erase(graphbar_vector_.begin());
    delete graphbar_to_delete;

    graphbar_counter_ --;

    //Update the x-coordinates of remaining GraphBars so they shift left
    for(unsigned int i = 0; i < graphbar_vector_.size(); i++){
        graphbar_vector_[i] -> DecrementX(width_);
    }

}

