#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
#include <list>
#include <cstdlib>

using namespace std;

class Node
{
    public:
        Node();
        //virtual ~Node();
        Node *ch1, *ch2, *ch3, *ch4;
        Node *parent;

        int step; //g(n)
        double heu;    // h(n)
//        int tot = step + heu;    // f(n) = g(n) + h(n)
        string name;
        vector< vector<int> > board;
};

#endif // NODE_H
