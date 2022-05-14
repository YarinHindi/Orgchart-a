#pragma once
#include <string>
#include <vector>
using namespace std;
class Node{

    public:
    string name;
    vector<Node *> children;
    Node *parent;
    Node(const string & name);
//    ~Node();

};
