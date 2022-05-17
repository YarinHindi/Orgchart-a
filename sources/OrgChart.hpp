#pragma once
#include <vector>
#include "Node.hpp"
#include <deque>
#include <stdexcept>
#include <iostream>
using namespace std;
namespace ariel {
    class OrgChart {
        Node *root;
    public:
        OrgChart() ;
        OrgChart(const OrgChart & other);
        ~OrgChart() ;
        void traverse_for_delete();
        void traverse_for_copy(const OrgChart &other);
        OrgChart &add_root(const string &name) ;
        OrgChart &add_sub(const string &master, const string &sub) ;
        Node *find_node(const string &name)const ;
        static vector <vector<string>> pre_output(vector <vector<string>> &ans, Node *root) ;
        friend ostream &operator<<(ostream &out, const OrgChart &org);
        OrgChart& operator=(const OrgChart& other);
        OrgChart(OrgChart&& org)noexcept;
        OrgChart& operator=(OrgChart&& org)noexcept;
        class iterator {
        private:
            vector<Node *> traverse;
            Node *current;
        public:
            iterator(Node *node, const int &type) ;
            enum State{LEVEL_ORDER,REVERSE_LEVEL_ORDER,PRE_ORDER,END};
            void update_traverse_pre(Node *n) ;
            void update_traverse_reverse(Node *n, unsigned int index) ;
            void update_traverse_reg(Node *n, unsigned int index) ;
            bool operator==(const iterator &other) const ;
            bool operator!=(const iterator &other) const ;
            iterator &operator++() ;
            iterator operator++(int) ;
            string & operator*() const;
            string * operator->() const;
        };
        iterator begin_level_order()const ;
        static iterator end_level_order() ;
        iterator begin_reverse_order()const ;
        static iterator reverse_order() ;
        iterator begin_preorder() const;
        static iterator end_preorder() ;
        iterator begin() const;
        static iterator end() ;
    };
};