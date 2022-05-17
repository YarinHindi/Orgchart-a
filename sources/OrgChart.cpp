#include "OrgChart.hpp"

using namespace ariel;
using namespace std;

OrgChart::OrgChart() {
    this->root = nullptr;
}

OrgChart::OrgChart(const OrgChart & other){
    traverse_for_copy(other);
}
OrgChart::~OrgChart() {
      traverse_for_delete();
}
OrgChart::OrgChart(OrgChart &&org)noexcept {
    this->root = org.root;
    org.root = nullptr;
}
OrgChart& OrgChart::operator=(OrgChart &&org)noexcept {
    if(this!=&org){
    traverse_for_delete();
    this->root = org.root;
    org.root = nullptr;
    }
    return *this;
}
void OrgChart::traverse_for_delete(){
    deque <Node*> beforeans;
    vector <Node*> templist;
    if (this->root == nullptr) {
        return;
    }
    beforeans.push_back(root);
    Node *temp = nullptr;
    while (!beforeans.empty()) {
        temp = (Node *) beforeans.front();
        beforeans.pop_front();
        templist.push_back(temp);
        for (unsigned int i = 0; i < temp->children.size(); i++) {
            beforeans.push_back(temp->children.at(i));
        }
    }
    for (unsigned int i = 0; i < templist.size(); ++i) {
        delete templist.at(i);
    }
}
void OrgChart::traverse_for_copy(const OrgChart &other) {
    deque <Node *>list_to_find;
    if (other.root == nullptr) {
        this->root = nullptr;
        return ;
    }
    this->root = nullptr;
    list_to_find.push_back(other.root);
    this->add_root(other.root->name);
    while (!list_to_find.empty()) {
        Node *temp = list_to_find.front();
        list_to_find.pop_front();
        for (unsigned int i = 0; i < temp->children.size(); i++) {
            this->add_sub(temp->name,temp->children.at(i)->name);
            list_to_find.push_back(temp->children.at(i));
        }
    }
}
OrgChart& OrgChart::operator=(const OrgChart &other){
    if(&other!=this) {
        traverse_for_delete();
        traverse_for_copy(other);
    }
    return *this;
}

OrgChart & OrgChart::add_root(const string &name) {
    if (this->root == nullptr) {
        this->root = new Node(name);
    } else {
        Node *new_root = new Node(name);
        this->root->parent = new_root;
        new_root->children.push_back(this->root);
        this->root = new_root;
    }
    return *this;
}
OrgChart& OrgChart::add_sub(const string &master, const string &sub) {
    Node *dad = find_node(master);
    if (dad != nullptr) {
        Node *new_node = new Node(sub);
        dad->children.push_back(new_node);
        new_node->parent = dad;
        return *(this);
    }
    throw invalid_argument("there is no master node");
}
Node* OrgChart::find_node(const string &name)const  {
    deque < Node * > list_to_find;
    if (this->root != nullptr) {
        list_to_find.push_back(this->root);
    }
    while (!list_to_find.empty()) {
        Node *temp = list_to_find.front();
        if (temp->name == name) {
            return temp;
        }
        list_to_find.pop_front();
        for (unsigned int i = 0; i < temp->children.size(); i++) {
            list_to_find.push_back(temp->children.at(i));
        }
    }
    return nullptr;
}
vector <vector<string>> OrgChart::pre_output(vector <vector<string>> &ans, Node *root) {
    deque < Node * > beforeans;
    vector <string> templist;
    if (root == nullptr) {
        return ans;
    }
    beforeans.push_back(root);
    Node *encounter = root;
    Node *temp = root;
    bool update = true;
    bool start = false;
    while (!beforeans.empty()) {
        temp = (Node *) beforeans.front();
        if (encounter == temp && start) {
            ans.push_back(templist);
            templist.clear();
            update = true;
        }
        start = true;
        beforeans.pop_front();
        if(temp!=root) {
            templist.push_back(temp->name + "(PARENT:"+temp->parent->name+")");
        }else{
            templist.push_back(temp->name + " : HEAD");
        }
        for (unsigned int i = 0; i < temp->children.size(); i++) {
            if (i == 0 && update) {
                encounter = temp->children.at(i);
                update = false;
            }
            beforeans.push_back(temp->children.at(i));
        }
    }
    ans.push_back(templist);
    return ans;
}
ostream& ariel::operator<<(ostream &out, const OrgChart &org) {
    vector<vector<string>> ans;
    org.pre_output(ans, org.root);
    for (unsigned int i = 0; i < ans.size(); i++) {
        if (i != 0) {
            out << endl;
        }
        for (unsigned j = 0; j < ans.at(i).size(); j++) {
            out << "--" << ans[i][j] << "--";
        }
    }
    return out;
}
OrgChart::iterator::iterator(Node *node, const int &type) {
        this->current = node;
        if (node != nullptr) {
        traverse.push_back(node);
        }
        if (type == iterator::State::REVERSE_LEVEL_ORDER) {
        update_traverse_reverse(node, 0);
        current = traverse.at(0);
        }
        if (type == iterator::State::LEVEL_ORDER) {
            if(node!= nullptr) {
                update_traverse_reg(node, 0);
            }
        }
        if (type == iterator::State::PRE_ORDER) {
        traverse.erase(traverse.begin());
        update_traverse_pre(node);
        }
        traverse.push_back(nullptr);
        }
void OrgChart::iterator::update_traverse_pre(Node *n) {
    if (n == nullptr) {
        return;
    }
    traverse.push_back(n);
    for (unsigned int i = 0; i < n->children.size(); ++i) {
        update_traverse_pre(n->children.at(i));
    }
}
void OrgChart::iterator::update_traverse_reverse(Node *n, unsigned int index) {
    if (traverse.at(index) == traverse.at(traverse.size() - 1) &&
        (traverse.at(index)->children.empty())) {
        for (unsigned int i = 0; i < traverse.size() / 2; i++) {
            Node *temp = traverse.at(i);
            traverse.at(i) = traverse.at(traverse.size() - 1 - i);
            traverse.at(traverse.size() - 1 - i) = temp;
        }
        return;
    }
    index++;
    for (unsigned int i = 0; i < n->children.size(); ++i) {
        traverse.push_back(n->children.at(n->children.size() - i - 1));
    }
    update_traverse_reverse(traverse.at(index), index);
}
void OrgChart::iterator::update_traverse_reg(Node *n, unsigned int index) {
    if (traverse.at(index) == traverse.at(traverse.size() - 1) &&
        (traverse.at(index)->children.empty())) {
        return;
    }
    index++;
    for (unsigned int i = 0; i < n->children.size(); ++i) {
        traverse.push_back(n->children.at(i));
    }
    update_traverse_reg(traverse.at(index), index);
}
string & OrgChart::iterator::operator*() const {
    return current->name;
}

string *OrgChart::iterator::operator->() const {
    return &(current->name);
}

bool OrgChart::iterator::operator==(const iterator &other) const {
    return this->current == other.current;
}

bool OrgChart::iterator::operator!=(const iterator &other) const {
    return !(*this == other);
}
OrgChart::iterator & OrgChart::iterator::operator++() {
    if (!traverse.empty()) {
        traverse.erase(traverse.begin());
        current = traverse.front();

    }
    return *this;
}

OrgChart::iterator OrgChart::iterator::operator++(int) {
    OrgChart::iterator t = *this;
    ++*this;
    return t;
}


OrgChart::iterator OrgChart::begin_level_order()const {
    return OrgChart::iterator{root, iterator::State::LEVEL_ORDER};
}

 OrgChart::iterator OrgChart::end_level_order() {
    return OrgChart::iterator{nullptr, iterator::State::END};
}

OrgChart::iterator OrgChart::begin_reverse_order()const {
    return OrgChart::iterator{root, iterator::State::REVERSE_LEVEL_ORDER};
}

 OrgChart::iterator OrgChart::reverse_order() {
    return OrgChart::iterator{nullptr, iterator::State::END};
}

OrgChart::iterator OrgChart::begin_preorder() const{
    return OrgChart::iterator{root, iterator::State::PRE_ORDER};
}

 OrgChart::iterator OrgChart::end_preorder() {
    return OrgChart::iterator{nullptr, iterator::State::END};
}

OrgChart::iterator OrgChart::begin() const{
    return OrgChart::iterator{root, iterator::State::LEVEL_ORDER};
}

 OrgChart::iterator OrgChart::end() {
     return OrgChart::iterator{nullptr,iterator::State::END};
 }