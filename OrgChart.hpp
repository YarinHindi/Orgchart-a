#include <vector>
#pragma once
#include "Node.cpp"
#include <deque>
namespace ariel {
    class OrgChart {
    public:
        Node *root;
        OrgChart(){
            this->root= nullptr;
        }
        ~OrgChart(){

        }
        OrgChart& add_root(const string &name ){
            if(this->root== nullptr){
                this->root = new Node(name);
            }else{
                Node *new_root = new Node(name);
                new_root->children.push_back(root);
                this->root = new_root;
            }
            return *this;
        }
        OrgChart& add_sub(const string &master,const string &sub){
            Node *dad = find_node(master);
            if(dad!= nullptr){
                Node *new_node = new Node(sub);
                dad->children.push_back(new_node);
                return *(this);
            }
            throw invalid_argument("there is no master node");
        }
         Node* find_node(const string &name){
            deque<Node*> list_to_find;
            if(root!= nullptr) {
                list_to_find.push_back(root);
            }
            while (list_to_find.size()!=0){
                Node *temp = list_to_find.front();
                if(temp->name==name){
                    return temp;
                }
                list_to_find.pop_front();
                for ( unsigned int i =0; i<temp->children.size();i++){
                    list_to_find.push_back(temp->children.at(i));
                }
            }
             return nullptr;
        }
        static vector<vector<string>> pre_output(vector<vector<string>>& ans,Node *root){
            deque<Node*> beforeans;
            vector<string>templist;
            if(root==nullptr){
                return ans;
            }
            beforeans.push_back(root);
            Node* encounter = root;
            Node *temp = root;
            bool update = true;
            bool start = false;
            while(beforeans.size()!=0){
                temp = (Node*)beforeans.front();
                if(encounter==temp&&start){
                    ans.push_back(templist);
                    templist.clear();
                    update=true;
                }
                start =true;
                beforeans.pop_front();
                templist.push_back(temp->name);
                for ( unsigned int i =0; i<temp->children.size();i++){
                    if(i==0&&update){
                        encounter = temp->children.at(i);
                        update=false;
                    }
                    beforeans.push_back(temp->children.at(i));
                }
            }
            ans.push_back(templist);
            return ans;
        }
        friend ostream &operator<<(ostream &out, const OrgChart &org){
            vector<vector<string>> ans ;
            org.pre_output(ans,org.root);
            for (unsigned int i=0; i<ans.size();i++){
                if(i!=0) {
                    out << endl;
                }
                for (unsigned j=0; j<ans.at(i).size();j++){
                    out<<"--"<<ans[i][j]<<"--";
                }
            }
            return out;
        }

        class iterator {
        private:
            vector<Node *> traverse;
            Node *current;
        public:
            iterator(Node *node, const char &type) {
                this->current = node;
                if (node != nullptr) {
                    traverse.push_back(node);
                }
                if (type == 'R') {
                    update_traverse_reverse(node, 0);
                    current = traverse.at(0);
                }
                if (type == 'L') {
                    update_traverse_reg(node, 0);
                }
                if (type == 'P') {
                    traverse.erase(traverse.begin());
                    update_traverse_pre(node);
                }
                traverse.push_back(nullptr);
            }
            void update_traverse_pre(Node *n){
                if(n== nullptr){
                    return;
                }
                traverse.push_back(n);
                for (unsigned int i = 0; i <n->children.size() ; ++i) {
                    update_traverse_pre(n->children.at(i));
                }
            }
            void update_traverse_reverse(Node *n, unsigned int index) {
                if (traverse.at(index) == traverse.at(traverse.size() - 1) &&
                    (traverse.at(index)->children.size() == 0)) {
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

            void update_traverse_reg(Node *n, unsigned int index) {
                if (traverse.at(index) == traverse.at(traverse.size() - 1) &&
                    (traverse.at(index)->children.size() == 0)) {
                    return;
                }
                index++;
                for (unsigned int i = 0; i < n->children.size(); ++i) {
                    traverse.push_back(n->children.at(i));
                }
                update_traverse_reg(traverse.at(index), index);
            }

            string &operator*() const {
                return current->name;
            }

            string* operator->() const {
                return &(current->name);
            }

            bool operator==(const iterator &other) const {
                if (current == nullptr || other.current == nullptr) {
                    return other.current == current;
                }
                return this->current->name == other.current->name;

            }

            bool operator!=(const iterator &other) const {
                return !(*this == other);
            }

            iterator &operator++() {
                if (traverse.size() > 0) {
                    traverse.erase(traverse.begin());
                    current = traverse.front();

                }
                return *this;
            }

            iterator operator++(int) {
                iterator t = *this;
                ++*this;
                return t;
            }
        };

        iterator begin_level_order() {
            return iterator{root, 'L'};
        }

        iterator end_level_order() {
            return iterator{nullptr, 'N'};
        }

        iterator begin_reverse_order() {
            return iterator{root, 'R'};
        }

        iterator reverse_order() {
            return iterator{nullptr, 'N'};
        }

        iterator begin_preorder() {
            return iterator{root, 'P'};
        }

        iterator end_preorder() {
            return iterator{nullptr, 'N'};
        }
    };
};