#ifndef GRAPHCONTAINER_H
#define GRAPHCONTAINER_H

#include <vector>

#include <graphbar.h>

class GraphContainer : public QObject
{

    Q_OBJECT


public:

    GraphContainer(int width, int height);

    std::vector<GraphBar *> get_graphbar_vector_(){return graphbar_vector_; } //Get the vector of GraphBars

    void AddGraphBar(double population); //Add a new GraphBar to the vector; utilizes RemoveGraphBar as a helper function

    void RemoveGraphBar(); //Remove the oldest GraphBar when the limit is reached and update the x-coordinates of all other GraphBars

private:

    //The GraphContainer has the same width and height as the GraphScene
    int width_;
    int height_;

    //The GraphContainer stores indivudal GraphBars and manages them when the entire graphScene fills up
    std::vector<GraphBar *> graphbar_vector_;

    //The number of GraphBars that can fit on the scene at any given time
    unsigned int graphbar_limit_ = 18;

    //Determine the number of GraphBars currently in the vector (this is more readbale than accessing the size)
    //Note that 0-indexing means limit is 19, as bars have 1/20 the width of the screen
    int graphbar_counter_ = 0;
};

#endif // GRAPHCONTAINER_H
