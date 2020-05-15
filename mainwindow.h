#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <board.h> //also includes tile.h
#include <graphcontainer.h> //also includes graphbar.h

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void increment_turn_count_(){turn_count_ += 1; } //increase the turn count by 1
    int get_turn_count_(){return turn_count_; } //get the current turn count

    double get_speed_(){return speed_; } //get the speed for the timer event

    int get_timer_id_(){return timer_id_; } //get the timer id for killing the timer

    void DrawBoard(Board * game_board); //Add every tile in the board to the boardScene

    void UpdateTurn(); //Increment the turn when the step button is pressed or time limit is reached

    void UpdateSpeed(int speed); //Change the speed value depending on the value of the slider

    void UpdateGraph(); //Draw a new Graph to the scene

    void EraseGraph(); //Erase the old Graph from the scene

protected:
    void timerEvent(QTimerEvent *event) override;

private slots:

    void populationSlot(std::vector<double> probability); //update the population label

    void on_stepButton_clicked();

    void on_reviveButton_clicked();

    void on_speedSlider_sliderReleased();

    void on_playButton_clicked();

    void on_pauseButton_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *boardScene;
    QGraphicsScene *graphScene;

    //The MainWindow acts as a container for the Board and Graph objects that are created
    Board * game_board_;
    GraphContainer * game_graph_;

    //Store the number of turns (or board updates) that have occurred
    int turn_count_ = 0;

    //Store the current speed at which the board updates in seconds
    //Starts at 2.5, the slowest speed
    double speed_ = 2.5;

    //stores the timer id for keeping track of which timer to kill
    int timer_id_;

    //Determine whether or not the game is playing (true) or paused (false)
    bool play_;
};
#endif // MAINWINDOW_H
