#include"sources/OrgChart.hpp"
#include "doctest.h"
#include <sstream>
#include <fstream>
#include <ctime>

using namespace ariel;

#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

TEST_CASE("add root couple of times"){
   OrgChart org;
   CHECK_NOTHROW(org.add_root("VICE MANGER 1"));
//   CHECK_NOTHROW(cout<<org<<endl);
   CHECK_NOTHROW(org.add_root("MANGER"));
//   CHECK_NOTHROW(cout<<org<<endl);
   CHECK_THROWS(org.add_sub("EMPLOYEE","LOW EMPLOYEE"));
   CHECK_NOTHROW(org.add_sub("MANGER","VICE MANGER 2"));
   CHECK_NOTHROW(org.add_sub("MANGER","VICE MANGER 3"));
   CHECK_NOTHROW(org.add_sub("MANGER","VICE MANGER 4"));
    CHECK_NOTHROW(org.add_sub("MANGER","VICE MANGER 5"));
    CHECK_NOTHROW(org.add_sub("MANGER","VICE MANGER 6"));
    CHECK_NOTHROW(org.add_sub("MANGER","VICE MANGER 7"));
    CHECK_NOTHROW(org.add_sub("MANGER","VICE MANGER 8"));
    CHECK_NOTHROW(org.add_sub("MANGER","VICE MANGER 9"));
   char counter = '0';
    for (string element: org)
    {
        if(counter=='0'){
            CHECK(element=="MANGER");
        }else {
            string s = "VICE MANGER ";
            s+=counter;
            CHECK(element==s);
        }
        counter++;
    }
    CHECK_NOTHROW(org.add_root("BIG BOSS"));
}
TEST_CASE("try to add sub to a node that not exist"){
    OrgChart org1;
    CHECK_NOTHROW(org1.add_root("MANGER"));
    CHECK_THROWS(org1.add_sub("DAVID","RACHEL"));
    CHECK_THROWS(org1.add_sub("MANGER2","MANGER"));
    CHECK_NOTHROW(org1.add_sub("MANGER","WORKER"));
    CHECK_NOTHROW(org1.add_sub("WORKER","MANGER"));
//    CHECK_NOTHROW(cout<<org1<<endl);
}
TEST_CASE("adding node with same names and check not throwing"){
    OrgChart org1;
    CHECK_NOTHROW(org1.add_root("YARIN"));
    CHECK_NOTHROW(org1.add_sub("YARIN","DAVID"));
    CHECK_NOTHROW(org1.add_sub("DAVID","YARIN"));
    CHECK_NOTHROW(org1.add_sub("DAVID","SHALOM"));
    CHECK_NOTHROW(org1.add_sub("SHALOM","SHALOM"));
}
TEST_CASE("GENERATE BIG TREE WITH DIFFERENT NAMES TO SEE THAT WE CAN HANDLE IT"){
    OrgChart org2;
    org2.add_root("YARIN");
    //the tree with 1000 nodes;
    //the tree will be in this form -->        root
    //                                    -----------
    //                                    |    |    |
    //                                  node node node..(lenght 10)
    //                                            ------
    //                                            |  |  |
    //                                        node  node   node ..(lenght 10)
    string s ="a";
    unsigned int count =0;
    string sub = "YARIN";
    while(count<100){
        s+="a";
        for (int i = 0; i < 10; ++i) {
            s.at(count)++;
            CHECK_NOTHROW(org2.add_sub(sub,s));
        }
        count++;
        sub = s;
    }
}
TEST_CASE("CHECK OPERATORS IF ASSIGMENT WORKING GOOD"){
    OrgChart org3;
    CHECK_NOTHROW(org3.add_root("HEAD"));
    CHECK_NOTHROW(org3.add_sub("HEAD","ARM"));
    CHECK_NOTHROW(org3.add_sub("ARM","HEAD"));
    CHECK_NOTHROW(org3.add_sub("ARM","FINGER"));
    CHECK_NOTHROW(org3.add_sub("FINGER","FINGER"));
    OrgChart org4(org3);
    ostringstream os3;
    ostringstream os4;
    os3 << org3;
    os4 <<org4;
    CHECK(os3.str()==os4.str());
    CHECK_NOTHROW(org3 = org3);
    OrgChart org5;
    org5.add_root("SAMI");
    //check for memory leak if in the assignment operator we didnt delete org 3 memory before, we're
    //going to get a leak.
    CHECK_NOTHROW(org3 = org5);
}
TEST_CASE("CHECK ITERATOR NO THROW AND RIGHT OUTPUT"){
    OrgChart org6;
    //check will not crush for applying the iterator on empty orgchart
    //should do nothing cause org6->root is nullptr
    CHECK_NOTHROW(auto it=org6.begin_level_order());
    CHECK_NOTHROW (auto it1 = org6.end_level_order());

    CHECK_NOTHROW(org6.add_root("HEAD"));
    CHECK_NOTHROW(org6.add_sub("HEAD","LEG"));
    CHECK_NOTHROW(org6.add_sub("LEG","KNEE"));
    CHECK_NOTHROW(org6.add_sub("KNEE","FOOT"));
    CHECK_NOTHROW(org6.add_sub("FOOT","FINGER"));
    //org6 look like linkedlsit so revrse_level_order and level_order should be the reverse list of one
    //to another
    string siter2[5];
    string siter3[5];
    string siter4[5];
    int counter =0;
    for (auto it2 = org6.begin_level_order(); it2 != org6.end_level_order(); ++it2)
    {
        siter2[counter] = *it2;
        counter++;
    }
    counter=0;
    for (auto it3 = org6.begin_reverse_order(); it3 != org6.reverse_order(); it3++)
    {
        siter3[counter] = *it3;
        counter++;
    }
    counter=0;
    for (auto it4 = org6.begin_preorder(); it4 != org6.end_preorder(); ++it4)
    {
        siter4[counter] = *it4;
//        cout<<*it4<<endl;
        counter++;
    }
    for (int i = 0; i < 5; ++i) {
                CHECK(siter2[i] == siter3[5 - i - 1]);
                CHECK(siter2[i]==siter4[i]);
    }
}
TEST_CASE("GENRATE RANDOM TREE AND  CHECK ITERATORS WILL NOT CRUSH"){
    OrgChart org2;
    org2.add_root("YARIN");
    srand(time(0));
    vector<string> nodes;
    string s ="a";
    unsigned int count =0;
    nodes.push_back("YARIN");
    string sub = "YARIN";
    while(count<100){
        s+="a";
        for (int i = 0; i < 10; ++i) {
            s.at(count)++;
            unsigned int pos = (unsigned int)rand()%nodes.size();
            string sub = nodes.at(pos);
            CHECK_NOTHROW(org2.add_sub(sub,s));
            nodes.push_back(sub);
        }
        count++;
        sub = s;
    }
    for (auto it2 = org2.begin_preorder(); it2 != org2.end_preorder(); it2++)
    {
        CHECK_NOTHROW(*it2);
    }
    for (auto it3 = org2.begin_level_order(); it3 != org2.end_level_order(); ++it3)
    {
        CHECK_NOTHROW(*it3);
    }
    for (auto it4 = org2.begin_reverse_order(); it4 != org2.reverse_order(); ++it4)
    {
        CHECK_NOTHROW(*it4);
    }

}
TEST_CASE("test iterator -> oprator"){

    OrgChart org;
    //should look like a linked list
    vector<int> word_len = {8,7,6,5,4,3,2,1};
    vector<int> word_len2 = {1,2,3,4,5,6,7,8};
    org.add_root("A");
    org.add_root("AA");
    org.add_root("AAA");
    org.add_root("AAAA");
    org.add_root("AAAAA");
    org.add_root("AAAAAA");
    org.add_root("AAAAAAA");
    org.add_root("AAAAAAAA");
    unsigned int pos = 0;
    for (auto it = org.begin_level_order(); it != org.end_level_order(); ++it)
    {
        CHECK(it->size()==word_len.at(pos));
        pos++;
    }
    pos=0;
    for (auto it = org.begin_reverse_order(); it != org.reverse_order(); ++it)
    {
                CHECK(it->size()==word_len2.at(pos));
        pos++;
    }
    pos=0;
    for (auto it = org.begin_preorder(); it != org.end_preorder(); it++)
    {
                CHECK(it->size()==word_len.at(pos));
        pos++;
    }

}

