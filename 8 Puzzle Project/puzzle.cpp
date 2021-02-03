#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
#include <list>
#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <chrono>
#include "puzzle.h"
#include "Node.h"

using namespace std;
using namespace std::chrono;

puzzle::puzzle()
{
    //ctor
    root = 0;
}

void puzzle::setPuzzle(int choice){
    Node* tile = new Node;
    goalReached = false;
    nodesExpanded = 0;
    pqNodes = 0;

    if(choice == 1){
        cout << "Default puzzle chosen, what difficulty?\n";
        cout << "(1) Easy " << endl;
        cout << "(2) Medium " << endl;
        cout << "(3) Hard " << endl;
        cout << "(4) Oh Boy (buggy on UCS) " << endl;
        vector< vector<int> > easy = { {1, 2, 0}, {4, 5, 3}, {7, 8, 6} };
        vector< vector<int> > medium = { {4, 1, 2}, {5, 8, 3}, {7, 0, 6} };
        vector< vector<int> > hard = { {1, 4, 0}, {7, 3, 2}, {6, 5, 8} };
        vector< vector<int> > ohboy = { {8, 7, 1}, {6, 0, 2}, {5, 4, 3} };

        int diff;
        cin >> diff;
        switch(diff){
        case 1:
            tile->board = easy;
            break;
        case 2:
            tile->board = medium;
            break;
        case 3:
            tile->board = hard;
            break;
        case 4:
            tile->board = ohboy;
            break;
        default:
            cout << "Invalid selection, stopping... " << endl;
            exit(1);
            break;
        }
        cout << endl;
        root = tile;                    //base case is root
        visited.push_back(root->board); //we have visited this case
        pq.push(root);                  //we push this onto the frontier
        cout << "Puzzle chosen is " << endl;
        analyze(tile);

//        for(int row = 0; row < 3; row++){  //displays the board; probably will make display function later
//            for(int col = 0; col < 3; col++){
//                cout << tile->board.at(row).at(col) << " ";
//            }
//            cout << endl;
//        }
    } else if (choice == 2){
        vector<int> temp1, temp2, temp3;
        cout << "Enter your puzzle. Please use a \"0\" to represent the blank\n";
        cout << "Enter the first row, use a space of tabs between the numbers\n";
        int a,b,c;
        cin >> a >> b >> c;
        temp1.push_back(a);
        temp1.push_back(b);
        temp1.push_back(c);
        cout << "Enter the second row, use a space of tabs between the numbers\n";
        cin >> a >> b >> c;
        temp2.push_back(a);
        temp2.push_back(b);
        temp2.push_back(c);
        cout << "Enter the third row, use a space of tabs between the numbers\n";
        cin >> a >> b >> c;
        temp3.push_back(a);
        temp3.push_back(b);
        temp3.push_back(c);

        tile->board.push_back(temp1);
        tile->board.push_back(temp2);
        tile->board.push_back(temp3);
        cout << endl;
        cout << "Puzzle has been set to: "<< endl;

        root = tile;                                //base case is root
        visited.push_back(root->board);             //visited this node
        pq.push(root);                              //root is pushed into frontier

        for(int row = 0; row < 3; row++){
            for(int col = 0; col < 3; col++){
                cout << tile->board.at(row).at(col) << " ";
            }
            cout << endl;
        }
    } else {
        cout << "Invalid input, stopping..." << endl;
        exit(1);
    }
    cout << endl;
}

void puzzle::setSearch(int choice){

    searchChoice = choice;

    if(root == 0){                                          //checks if puzzle is created
        cout << "No puzzle detected, stopping..." << endl;
        exit(1);
    }

    if(!checkPuzzle(root)){                                 //checks if puzzle is valid
        cout << "Illegal puzzle, stopping..." << endl;
        exit(1);
    }

    if(choice == 1 || choice == 2 || choice == 3){          //as long as choice is valid, we search
        cout << "Expanding state:" << endl;
        doSearch(root);                                     //choice is valid, proceed with search
        cout << endl;
    } else {
        cout << "Invalid search option, stopping..." << endl;
        exit(1);
    }

}

bool puzzle::checkPuzzle(Node* p){
    bool valid = false;
    vector<int> testBench = {0,1,2,3,4,5,6,7,8};
    for(unsigned i = 0; i < testBench.size(); i++){         //loops through testbench 0-8
        valid = false;
        for(int row = 0; row < 3; row++){
            for(int col = 0; col < 3; col++){
                if(p->board.at(row).at(col) == testBench.at(i)){               //if we find the # in the puzzle at the testbench
                    //testBench.erase(find(testBench.begin(),testBench.end(),i)); //we pop it out and make sure there are no duplicates //this out of bounds woops
                    valid = true;
                }
                if(row == 2 && col == 2 && valid == false){                      //if there are duplicates or invalid puzzle, we return false
                    return false;
                }
            }
        }
    }
    return true;
}

int puzzle::findBlank(Node* p){
    //TODO: find better algorithm to do this.
    //idea: label from 0-8 and mod each col by 3
    int blankFound = 10; // since its an 8 puzzle + 1 blank space,
                         // we need a 3x3 area, so we can change
                         // the puzzle size if we have a 15 puzzle with one blank space
                         // which is a 4x4 area
                         // by simply doing (m*m)+1 where m is length and width
                         // also it is "+1" to determine if the blank space is there or not

    int index = 1;       // will use this to find where the blank space is
                         // iterate through the area of:
                         // 1 2 3
                         // 4 5 6
                         // 7 8 9
                         // which will let us know how many children that position has
    for(int row = 0; row < 3; row++){
        for (int col = 0; col < 3; col++){
            if(p->board.at(row).at(col) == 0){
                blankFound = index;           //sets blank found to index indicating that this is where the blank space is
            } else {
                index++;
            }
        }
    }

    if(blankFound == 10){                   //out of bounds error (blank cannot be m*m+1)
        cout << "Something went wrong... Stopping..." << endl;
        exit(1);
    }

    return blankFound;
}

bool puzzle::finishedPuzzle(Node *p){
    vector< vector<int> > solution = { {1, 2, 3} ,
                                       {4, 5, 6} ,
                                       {7, 8 ,0} };
    if(p->board == solution){
        return true;
    } else {
        return false;
    }
}


//influenced by https://www.geeksforgeeks.org/a-search-algorithm/
void puzzle::doSearch(Node* p){
    //auto start = high_resolution_clock::now();
    int blankIndex = findBlank(p); //finds blank space

    if(finishedPuzzle(p)){  //does current node have a goal state
        goalReached = true;
        goal = p;
    } else {
        pq.pop();               //pop it off
    }

    switch(blankIndex){
    case 1:
        topLeft(p);
        break;
    case 2:
        topMid(p);
        break;
    case 3:
        topRight(p);
        break;
    case 4:
        midLeft(p);
        break;
    case 5:
        midMid(p);
        break;
    case 6:
        midRight(p);
        break;
    case 7:
        botLeft(p);
        break;
    case 8:
       botMid(p);
        break;
    case 9:
        botRight(p);
        break;
    default:
        cout << "Out of bounds error" << endl;
        exit(1);
    }

    if(goalReached){
        while(pq.top() != goal){
            pq.pop();
        }
        analyze(pq.top());
        cout << "Goal!!!" << endl;
        //auto stop = high_resolution_clock::now();
        //auto runtime = duration_cast<milliseconds>(stop - start);
        cout << "To solve this problem, the search algorithm expanded a total of " << nodesExpanded << " nodes." << endl;
        cout << "The maximum nodes in the queue at any one time was " << pqNodes << "." << endl;
        cout << "The total depth of this problem was " << pq.top()->step << "." << endl;
        //cout << "The time taken for this search : " << runtime.count() << " milliseconds." << endl;
        cout << endl;
        cout << "Showing trace of final path: " << endl;
        showFinalTrace(pq.top());
        return;
    } else {
        if(pq.top() != 0){
            analyze(pq.top());
            doSearch(pq.top());
        }
    }
}

void puzzle::topLeft(Node *p){  //possible moves when blank is top left cell
    vector< vector<int> > right = p->board;
    vector< vector<int> > down = p->board;
    swap(right.at(0).at(0), right.at(0).at(1));
    setChild(p, right);
    swap(down.at(0).at(0), down.at(1).at(0));
    setChild(p, down);
}

void puzzle::topMid(Node *p){  //possible moves when blank is top middle cell
    vector< vector<int> > right = p->board;
    vector< vector<int> > down = p->board;
    vector< vector<int> > left = p->board;
    swap(right.at(0).at(1), right.at(0).at(2));
    setChild(p, right);
    swap(down.at(0).at(1), down.at(1).at(1));
    setChild(p, down);
    swap(left.at(0).at(1), left.at(0).at(0));
    setChild(p, left);
}

void puzzle::topRight(Node *p){  //possible moves when blank is top right cell
    vector< vector<int> > left = p->board;
    vector< vector<int> > down = p->board;
    swap(left.at(0).at(2), left.at(0).at(1));
    setChild(p, left);
    swap(down.at(0).at(2), down.at(1).at(2));
    setChild(p, down);
}

void puzzle::midLeft(Node *p){  //possible moves when blank is middle left cell
    vector< vector<int> > right = p->board;
    vector< vector<int> > down = p->board;
    vector< vector<int> > up = p->board;
    swap(right.at(1).at(0), right.at(1).at(1));
    setChild(p, right);
    swap(down.at(1).at(0), down.at(2).at(0));
    setChild(p, down);
    swap(up.at(1).at(0), up.at(0).at(0));
    setChild(p, up);
}

void puzzle::midMid(Node *p){ //possible moves when blank is dead middle cell
    vector< vector<int> > right = p->board;
    vector< vector<int> > down = p->board;
    vector< vector<int> > up = p->board;
    vector< vector<int> > left = p->board;
    swap(right.at(1).at(1), right.at(1).at(2));
    setChild(p, right);
    swap(down.at(1).at(1), down.at(2).at(1));
    setChild(p, down);
    swap(up.at(1).at(1), up.at(0).at(1));
    setChild(p, up);
    swap(left.at(1).at(1), left.at(1).at(0));
    setChild(p, left);
}

void puzzle::midRight(Node *p){ //possible moves when blank is middle right cell
    vector< vector<int> > left = p->board;
    vector< vector<int> > down = p->board;
    swap(left.at(1).at(2), left.at(1).at(1));
    setChild(p, left);
    swap(down.at(1).at(2), down.at(2).at(2));
    setChild(p, down);
}

void puzzle::botLeft(Node *p){  //possible moves when blank is bottom left cell
    vector< vector<int> > right = p->board;
    vector< vector<int> > up = p->board;
    swap(right.at(2).at(0), right.at(2).at(1));
    setChild(p, right);
    swap(up.at(2).at(0), up.at(1).at(0));
    setChild(p, up);
}

void puzzle::botMid(Node *p){  //possible moves when blank is bottom middle cell
    vector< vector<int> > right = p->board;
    vector< vector<int> > up = p->board;
    vector< vector<int> > left = p->board;
    swap(right.at(2).at(1), right.at(2).at(2));
    setChild(p, right);
    swap(up.at(2).at(1), up.at(1).at(1));
    setChild(p, up);
    swap(left.at(2).at(1), left.at(2).at(0));
    setChild(p, left);
}

void puzzle::botRight(Node *p){  //possible moves when blank is bottom right cell
    vector< vector<int> > up = p->board;
    vector< vector<int> > left = p->board;
    swap(left.at(2).at(2), left.at(2).at(1));
    setChild(p, left);
    swap(up.at(2).at(2), up.at(1).at(2));
    setChild(p, up);
}

bool puzzle::alreadyVisited(vector< vector<int> > newChild){
    for(unsigned i = 0; i < visited.size(); i++){
        if(newChild == visited.at(i)){
            return true;
        }
    }
    return false;
}

void puzzle::setChild(Node *p, vector< vector<int> > newChild){
    Node *temp = new Node;
    temp->board = newChild;
    bool child1, child2, child3, child4 = false;
    if(p->ch1 != 0){
        child1 = true;
    }
    if(p->ch2 != 0){
        child2 = true;
    }
    if(p->ch3 != 0){
        child3 = true;
    }
    if(p->ch4 != 0){
        child4 = true;
    }
    if(goalReached){
        return;
    }
    if(finishedPuzzle(temp)){
        goalReached = true;
        visited.push_back(newChild);
        temp->step = p->step + 1;
        temp->heu = whichHeuristic(newChild);
        if(!child1){
            p->ch1 = temp;
            p->ch1->parent = p;
            pq.push(p->ch1);
            goal = p->ch1;
        }else if(!child2){
            p->ch2 = temp;
            p->ch2->parent = p;
            pq.push(p->ch2);
            goal = p->ch2;
        } else if(!child3){
            p->ch3 = temp;
            p->ch3->parent = p;
            pq.push(p->ch3);
            goal = p->ch3;
        } else if(!child4){
            p->ch4 = temp;
            p->ch4->parent = p;
            pq.push(p->ch4);
            goal = p->ch4;
        }
        nodesExpanded++;
        totalPQ();

        return;
    }
    if(alreadyVisited(newChild)){
        if(!child1){
            p->ch1 = 0;
        }
        if(!child2){
            p->ch2 = 0;
        }
        if(!child3){
            p->ch3 = 0;
        }
        if(!child4){
            p->ch4 = 0;
        }
        return;
    } else if (!alreadyVisited(newChild)){
        visited.push_back(newChild);
        temp->board = newChild;
        temp->step = p->step + 1;
        temp->heu = whichHeuristic(newChild);
        if(!child1){
            p->ch1 = temp;
            p->ch1->parent = p;
            pq.push(p->ch1);
        }else if(!child2){
            p->ch2 = temp;
            p->ch2->parent = p;
            pq.push(p->ch2);
        } else if(!child3){
            p->ch3 = temp;
            p->ch3->parent = p;
            pq.push(p->ch3);
        } else if(!child4){
            p->ch4 = temp;
            p->ch4->parent = p;
            pq.push(p->ch4);
        }
        nodesExpanded++;
        totalPQ();

    }
}

double puzzle::whichHeuristic(vector< vector<int> > newChild){
    vector< vector<int> > solution = { {1, 2, 3} ,
                                       {4, 5, 6} ,
                                       {7, 8 ,0} };

    double heuristic = 0;
    double heutemp = 0;
    double heusqrt = 0;
    switch(searchChoice){
    case 1: //UNIFORM COST
        heuristic = 0;
        break;
    case 2: //MISPLACED TILE
        for(int row = 0; row < 3; row++){
            for (int col = 0; col < 3; col++){
                if(newChild.at(row).at(col) != solution.at(row).at(col)){
                    heuristic++;
                }
            }
        }
        break;
    case 3:
        //this definitely increases runtime, if have extra time find better algorithm
        for(int goalRow = 0; goalRow < 3; goalRow ++){
            for(int goalCol = 0; goalCol < 3; goalCol++){
                for(int row = 0; row < 3; row++){
                    for(int col = 0; col < 3; col++){
                        if(solution.at(goalRow).at(goalCol) == newChild.at(row).at(col)){
                            heutemp += pow(row - goalRow,2);
                            heutemp += pow(col - goalCol,2);
                        }
                    }
                }
            }
        }
        heusqrt = sqrt(heutemp);
        heuristic = heusqrt;
        break;
    default:
        cout << "No search choice selected, quitting" << endl;
        exit(1);
        break;
    }

    return heuristic;
}


void puzzle::totalPQ(){
    if(pq.size() > pqNodes){
        pqNodes = pq.size();
    }
}


void puzzle::analyze(Node *p){
    if(p != root){
        cout << "The best state to expand with a g(n) = " << p->step
            << " and h(n) = " << p->heu << " is: " << endl;
    }

    displayPuzzle(p);
    cout << endl;
}


void puzzle::displayPuzzle(Node *p){
    for(int row = 0; row < 3; row++){
            for(int col = 0; col < 3; col++){
                cout << p->board.at(row).at(col) << " ";
            }
            cout << endl;
        }
}


void puzzle::showFinalTrace(Node *p){
    if(p == root){
        trace.push_front(p);
        finishedTrace(p);
    } else {
        trace.push_front(p);
        showFinalTrace(p->parent);
    }
}

void puzzle::finishedTrace(Node *p){
    cout << "Final size : " << trace.size() << endl;
    int finalsize = trace.size();
    for(unsigned long i = 0; i < finalsize; i++){
        if(trace.front() == root){
            cout << "Started from : " << endl;
        } else {
            cout << "Expanded " << i << ": " << endl;
        }
        for(int row = 0; row < 3; row++){
            for(int col = 0; col < 3; col++){
                cout << trace.front()->board.at(row).at(col) << " ";
            }
            cout << endl;
        }
        cout << endl;
        trace.pop_front();
    }
    cout << "Finished" << endl;
    return;
}
