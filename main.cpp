#include <iostream>
#include <list>
#include <algorithm>
#include <vector>

using namespace std;

struct Field{
    bool wall_down, wall_right;
    Field();
};
Field::Field(){
    wall_down = wall_right = false;
}

struct Task{
    int position;
    int steps;
    int destX, destY;
    int total_ways;
};
bool compare_steps(const Task &t1, const Task &t2){
    return t1.steps < t2.steps;
}
bool compare_position(const Task &t1, const Task &t2){
    return t1.position < t2.position;
}

class Board{
private:
    Field **tab;
    int size;
    int amount_of_walls;
public:
    Board(int);
    ~Board();
    void setData();
    int getSize();
    int**& find_ways(Task&, int**&, int);
    //void print_tab(int**&);
    int addNeighbours(int, int, int** &);
};
Board::Board(int n){
    this->size = n;
    tab = new Field *[size]();
    for(int i=0; i<size; i++){
        tab[i] = new Field [size]();
    }
}
Board::~Board(){
    for(int i=0; i<size; i++){
        delete [] tab[i];
    }
    delete [] tab;
}
void Board::setData(){
    bool wall;
    int x,y;
    cin >> this->amount_of_walls;
    for(int i=0; i<amount_of_walls; i++){
        cin >> wall >> x >> y;
        if(!wall)
            this->tab[x][y].wall_right = true;
        else
            this->tab[x][y].wall_down = true;
    }
}
int Board::getSize(){
    return this->size;
}
int**& Board::find_ways(Task &task, int** &previous, int counter){
    int **new_tab = new int *[size];
    for(int i=0; i<size; i++){
        new_tab[i] = new int [size];
    }
    for(int k=counter; k<task.steps; k++){
        for(int i=0; i<size; i++){
            for(int j=0; j<size; j++){
                new_tab[i][j] = addNeighbours(i,j,previous);
            }
        }
        previous = new_tab;
        cout << "STEP: " << k << endl;
        //print_tab(previous);
        counter++;
    }
    return previous;
    /*for(int i=0; i<size; i++){
        delete [] new_tab[i];
    }
    delete [] new_tab;*/
   // cout << "TEST nr " << counter << endl;
}
int Board::addNeighbours(int x, int y, int** &previous){
    int total = 0;
    if(x > 0){
        if(!tab[x-1][y].wall_down)
            total += previous[x-1][y];
    }
    if(x < size-1){
        if(!tab[x][y].wall_down)
            total += previous[x+1][y];
    }
    if(y > 0){
        if(!tab[x][y-1].wall_right)
            total += previous[x][y-1];
    }
    if(y < size-1){
        if(!tab[x][y].wall_right)
            total += previous[x][y+1];
    }
   // cout << "TOTAL STEPS for " << x << ", " << y << " =  " << total << endl;
    return total;
}


void print_tab(int** &tab, int size){
    cout << endl;
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            cout << tab[i][j] << "|";
        }
        cout << endl;
    }
    cout << endl;
}
void getTasks(list<Task>& tasks){
    int m;
    cin >> m;
    for(int i=0; i<m; i++){
        Task new_task;
        cin >> new_task.steps >> new_task.destX >> new_task.destY;
        new_task.position = i;
        tasks.push_back(new_task);
    }
}
void printTasks(list<Task> &tasks){
    tasks.sort(compare_position);
    list<Task>::iterator it = tasks.begin();
    while(it != tasks.end()){
        cout << (*it).total_ways << endl;
        it++;
    }
}

int main()
{
    int n;
    cin >> n;
    Board board(n);
    board.setData();
    int startX, startY;
    cin >> startX >> startY;
    list<Task> tasks;
    getTasks(tasks);
    tasks.sort(compare_steps);
    int counter = 0;
    int **previous = new int *[board.getSize()];
    int **new_tab;
    for(int i=0; i<board.getSize(); i++){
        previous[i] = new int [board.getSize()];
    }
    for(int i=0; i<board.getSize(); i++){
        for(int j=0; j<board.getSize(); j++){
            previous[i][j] = 0;
        }
    }
    previous[startX][startY] = 1;
    list<Task>::iterator it = tasks.begin();
    while(it != tasks.end()){
        Task wsk = *it;
        if(counter == wsk.steps){
            wsk.total_ways = previous[wsk.destX][wsk.destY];
        }
        else{
            new_tab = board.find_ways(wsk,previous,counter);
            wsk.total_ways = new_tab[wsk.destX][wsk.destY];
            //print_tab(new_tab,board.getSize());
        }
        it++;
    }
    printTasks(tasks);


    return 0;
}
