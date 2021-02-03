#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
#include <list>
#include "Node.h"

using namespace std;

Node::Node()
{
    //ctor
    ch1=0;
    ch2=0;
    ch3=0;
    ch4= 0;
    parent = 0;
    step = 0;
    heu = 0;
//    tot = step + heu;
    name = "";
}

//Node::~Node()
//{
//    //dtor
//}
