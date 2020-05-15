#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <string>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //We need to set up the ui before we can draw on a scene
    ui->setupUi(this);

    //the QGraphicsView is he UI element that contains the scene that is drawn on
    QGraphicsView * boardView = ui->boardView;
    QGraphicsView * graphView = ui->graphView;

    //boardScene and graphScene are pointer fields of the MainWindow class
    //this makes it possible to easily access the scene from across other functions in the class
    boardScene = new QGraphicsScene;
    boardView->setScene(boardScene);
    boardView->setSceneRect(0,0,boardView->frameSize().width() - 3, boardView->frameSize().height() - 3); //frameSize gets JUST a bit more than we want; this mkakes it so there are no scrolly bars

    graphScene = new QGraphicsScene;
    graphView->setScene(graphScene);
    graphView->setSceneRect(0,0,graphView->frameSize().width() - 3, graphView->frameSize().height() - 3); //frameSize gets JUST a bit more than we want; this makes it so there are no scrolly bars

    //Initialize the board, which in turn initializes the tiles
    //Also connect all signals and slots associated with the board
    //DrawBoard prints every single tile to the scene
    game_board_ = new Board(boardView->frameSize().width(), boardView->frameSize().height());
    connect(game_board_, &Board::populationSignal, this, &MainWindow::populationSlot);
    DrawBoard(game_board_);

    //The window should open with the population set, boar intialized, and tiles initalized
    game_board_ -> CalculatePopulation();
    game_board_ -> SetTileNeighbors();
    game_board_ -> SetNextStatus();

    //Initialize the slider to have a range of values from 0 to 100.
    //Speed will increment/decrement in groups of 10
    ui -> speedSlider -> setRange(0,100);

    //Initialize the graph, which begins with no GraphBars
    //New GraphBars will be added every time a new turn begins, which is also when the graph will be drawn and re-drawn
    game_graph_ = new GraphContainer(graphView->frameSize().width() - 3, graphView->frameSize().height() - 3);
    this -> game_graph_ -> AddGraphBar(game_board_ -> get_population_percentage_());
    UpdateGraph();

}

/*
A function to draw every single tile initialized in the board to the screen

@param: An instance of the board class
@return: Every tile with the board is drawn on the boardScene in the main window
*/
void MainWindow::DrawBoard(Board *game_board){

    std::vector<std::vector<Tile *>> tile_vector = game_board->get_tile_vector_();

    for(unsigned int i = 0; i < tile_vector.size(); i++){
        std::vector<Tile *> current_row = tile_vector[i];

        for(unsigned int j = 0; j < current_row.size(); j++){
            boardScene -> addItem(current_row[j]);
        }
    }
}


/*
Updates the graph by drawing the new items after th old graph has been erased

@param: None
@return: The updated graph is drawn to the screen
*/
void MainWindow::UpdateGraph(){

    std::vector<GraphBar *> graphbar_vector = game_graph_ -> get_graphbar_vector_();

    for(unsigned int i = 0; i < game_graph_ -> get_graphbar_vector_().size(); i++){
        graphScene -> addItem(game_graph_ -> get_graphbar_vector_()[i]);
    }

}

/*
Erases the old graph so that GraphBars that have been moved within the vector can be re-drawn to the scene (scene will not add the same item twice)

@param: None
@return: The entire graph is erased
*/
void MainWindow::EraseGraph(){

    std::vector<GraphBar *> graphbar_vector = game_graph_ -> get_graphbar_vector_();

    for(unsigned int i = 0; i < game_graph_ -> get_graphbar_vector_().size(); i++){
        graphScene -> removeItem(game_graph_ -> get_graphbar_vector_()[i]);
    }
}

/*
A slot to update the population label after the population has been calculated for the board

Responds to: populationSignal in the board class
@param: A vector containing two doubles, where the first number is the number of alive tiles and the second is the percentage of alive tiles
@return: The population label is updated as Probability: alive_count (percentage%)
*/
void MainWindow::populationSlot(std::vector<double> probability){

    //Removing trailing zeros and decimal
    std::string alive_count_string = std::to_string(probability[0]);
    alive_count_string.erase(alive_count_string.find_last_not_of('0') + 1, std::string::npos );
    alive_count_string.erase(alive_count_string.find_last_not_of('.') + 1, std::string::npos );

    //Removing trailing zeros and decimal
    std::string percentage_alive_string = std::to_string(probability[1]);
    percentage_alive_string.erase(percentage_alive_string.find_last_not_of('0') + 1, std::string::npos );
    percentage_alive_string.erase(percentage_alive_string.find_last_not_of('.') + 1, std::string::npos );

    //Format the population as a QString
    std::string new_population = "Population: " + alive_count_string + " (" + percentage_alive_string + "%)";
    QString new_population_qstring(new_population.c_str());
    ui->populationLabel->setText(new_population_qstring);
}


/*
Update the turn count every time the step button is clicked or the time limit is hit

@param: None
@return: The turn count is incremented and the turn label updated
*/

void MainWindow::UpdateTurn(){

    this -> increment_turn_count_();

    std::string new_turn = "Turn: " + std::to_string(this -> get_turn_count_());
    QString new_turn_qstring(new_turn.c_str());
    ui->turnLabel->setText(new_turn_qstring);

}


/*
When the slider is released, update the speed_variable and speedLabel to reflect the change
A new Timer should also begin separate of the play button being pressed
This also means that the old timer must be killed

@param: An integer speed, which is the value of the speedSlider on a scale of 0 to 100
@return: The speed_ variable and speedLabel are updated, old timer is killed and new one begins
*/
void MainWindow::UpdateSpeed(int speed){


    //The speed is calculated as 2.1 - (2 * speed)/100
    double max_speed = 2.1;
    double speed_to_double = (speed * 2) / 100.0;
    double new_speed = max_speed - speed_to_double;

    speed_ = new_speed;

    //Removing trailing zeros and decimal
    std::string speed_as_string = std::to_string(new_speed);
    speed_as_string.erase(speed_as_string.find_last_not_of('0') + 1, std::string::npos );
    speed_as_string.erase(speed_as_string.find_last_not_of('.') + 1, std::string::npos );

    //Update the speed label
    std::string new_speed_string = "Speed: " + speed_as_string + "s";
    QString new_speed_qstring(new_speed_string.c_str());
    ui->speedLabel->setText(new_speed_qstring);

    //if the game is currently playing, update the timer with the new speed
    if(play_){

        double speed_ms = this -> get_speed_() * 1000;

        //used for the case when user presses pause (thus killing a timer) and then presses play.
        //the timer was already killed, so trying to kill it again will cause an error
        if(timer_id_ != -1){
            this->killTimer(timer_id_);
        }

        this -> timer_id_ = this -> startTimer(speed_ms); //Timer takes a time as miliseconds

    }

}

/*
When the step button is clicked and the game is paused, the game should advance a single turn

@param: None
@return: The tiles update, the populations updates, the turn count updates, the next_status of each tile updates, and a GraphBar is added
*/
void MainWindow::on_stepButton_clicked()
{
    this -> game_board_ -> UpdateBoard();
    this -> game_board_ -> CalculatePopulation();
    this -> game_board_ -> SetNextStatus();

    EraseGraph();
    this -> game_graph_ -> AddGraphBar(game_board_ -> get_population_percentage_());
    UpdateGraph();

    this -> UpdateTurn();
}


/*
When the revive button is clicked and the game is paused, dead tiles have a 25% chance of becoming alive again

@param: none
@return: 25% of dead tiles become alive again, the population updates, and the next state of ech tile updates
*/
void MainWindow::on_reviveButton_clicked()
{
    this -> game_board_ -> ReviveBoard();
}

/*
When the speedSlider is released, the speed of the game and the speedLabel should update

@param: none
@return: The speed_ variable and speedLabel both update to the new speed
*/
void MainWindow::on_speedSlider_sliderReleased()
{
    this -> UpdateSpeed(ui->speedSlider->value());
}


/*
When the play button is clicked, the game should switch states and update at the current speed of the speed slider
The new timer starts in the UpdateSpeed() function

@param: none
@return: The play_ variable, speed label, and play pause label all update
*/
void MainWindow::on_playButton_clicked()
{
    if(!play_){
        play_ = true;
        ui->playPauseLabel->setText(QString("The game is currently playing."));
        this -> UpdateSpeed(ui->speedSlider->value()); //This also kills any previous timer and starts a new one
    }
}

/*
When the pause button is clicked, the game should switch states and stop updating unless the step button is clicked

@param: none
@return: The play_ variable, speed label, and play pause label all update
*/
void MainWindow::on_pauseButton_clicked()
{
    if(play_){
        play_ = false;
        ui->playPauseLabel->setText(QString("The game is currently paused."));
        ui->speedLabel->setText(QString("Speed: - - -"));
    }

    //kill the timer after determining the id of the timer currently running
    int timer_to_kill = this -> timer_id_;
    this -> killTimer(timer_to_kill);
    this -> timer_id_ = -1; //used to tell UpdateSpeed() that a timer was already killed
}

/*
When the timer fires, the board, population, tiles, and turn should all update
This update occurs at the speed of the timer, as determined by the speed slider

@param: none
@return: the board, population, tiles, turn,a nd graph all update
*/
void MainWindow::timerEvent(QTimerEvent *event){
    this -> game_board_ -> UpdateBoard();
    this -> game_board_ -> CalculatePopulation();
    this -> game_board_ -> SetNextStatus();

    EraseGraph();
    this -> game_graph_ -> AddGraphBar(game_board_ -> get_population_percentage_());
    UpdateGraph();

    this -> UpdateTurn();
}

MainWindow::~MainWindow()
{
    delete ui;
}

