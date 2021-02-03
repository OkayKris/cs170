#include <iostream>
#include <time.h>
#include <iomanip>
#include "Node.h"
#include "puzzle.h"



using namespace std;



int main()
{
    clock_t start, end;
    puzzle p;
    cout << "Welcome to the kdec001 8-puzzle solver!" << endl << endl;
    cout << "Please enter 1 to choose the default puzzle or 2 to choose your own puzzle" << endl;
    int choice;
    cin >> choice;
    p.setPuzzle(choice);

    cout << "Enter your choice of algorithm: " << endl;
    cout << "(1) Uniform Cost Search" << endl;
    cout << "(2) A* With Misplaced Heuristic" << endl;
    cout << "(3) A* With Euclidean Distance Heuristic" << endl;

    cin >> choice;
    start = clock();
    p.setSearch(choice);
    end = clock();
    double runtime = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "The runtime of this problem was: " << fixed << runtime << setprecision(5);
    cout << " seconds." << endl;

    return 0;
}
