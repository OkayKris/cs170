#ifndef PUZZLE_H
#define PUZZLE_H
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
#include <list>
#include "Node.h"

using namespace std;

//https://www.geeksforgeeks.org/priority-queue-in-cpp-stl/
//https://en.cppreference.com/w/cpp/container/vector


//https://stackoverflow.com/questions/15647162/priority-queue-in-reverse-order
class reverseQ{
public:
    bool operator()(Node* a, Node* b){
        if((a->step + a->heu) > (b->step+b->heu)){
            return true;
        } else {
            return false;
        }
    }
};


class puzzle
{
    public:
        puzzle(); //default constructor
        void setPuzzle(int choice); //default puzzle or custom
        bool checkPuzzle(Node *p); //checks if puzzle numbers are valid
        void setSearch(int choice); // sets search choice
        void doSearch(Node* p); //searches puzzle
        int findBlank(Node* p); //finds blank in node
        bool finishedPuzzle(Node* p); //checks if puzzle is finished
        void setChild(Node *p, vector< vector<int> > newChild); //sets new children of search
        bool alreadyVisited(vector< vector<int> > newChild); // checks if visited node already
        double whichHeuristic(vector< vector<int> > newChild); // which search we doing
        void totalPQ(); //total nodes in priority queue
        void analyze(Node* p); // displays best state to expand with costs
        void displayPuzzle(Node *p); //displays puzzle board
        void showFinalTrace(Node *p); //shows trace of best path
        void finishedTrace(Node *p); // finish

        //operators
        void topLeft(Node* p);
        void topMid(Node* p);
        void topRight(Node* p);
        void midLeft(Node* p);
        void midMid(Node* p);
        void midRight(Node* p);
        void botLeft(Node* p);
        void botMid(Node* p);
        void botRight(Node* p);


    protected:

    private:
        Node *root; //initial state
        Node *goal; //goal state

        bool goalReached; //checks if node is in goal state

        int nodesExpanded; //counts how many nodes were expanded in search
        unsigned pqNodes; //counts how many nodes were expanded in priority queue
        int searchChoice; //chooses uniform cost search, a* misplaced, a* euclidean

        vector < vector < vector<int>>> visited; //stores visited states
        priority_queue<Node*, vector<Node*>, reverseQ > pq; //sets priority queue to where f(n) is at top
        list<Node*> trace; //trace of final solution

};

#endif // PUZZLE_H
