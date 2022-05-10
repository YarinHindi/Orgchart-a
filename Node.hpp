using namespace std;
#include <string>
#include <vector>
class Node{

    public:
    string name;
    vector<Node *> children;
    Node(const string & name);
    ~Node();

};
