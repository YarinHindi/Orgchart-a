#include "OrgChart.hpp"
#include <stdexcept>
#include <iostream>
using namespace ariel;
using namespace std;
int main(){
    OrgChart org;
    cout<<"Hello here you can make an OrgChart as you wish\n"
          "first you need to give the head of the chart\n"
          "after that you can put the subordinate to one another\n"
          "but dont forget if you pick someone to be subordinate of another "
          "you should know he is already exist"<<endl;
    string name1;
    string name2;
    int act;
    cout<<"please enter the head of the chart :"<<endl;
    cin>>name1;
    org.add_root(name1);
    cout<<"from now on you can chose to replace the head or add subtenant to item that exist in the chart\n"
          "to replace head type 0\n"
          "to add sub type 1\n"
          "to exit type 2\n";
    while(act!=2){
        cout<<"please enter a number :"<<endl;
        cin>>act;
        while(act!=1&&act!=0&&act!=2) {
            cout<<"please enter 0 or 1 or 2 as you told";
            cin >> act;
        }
        if(act==2){
            break;
        }else if(act==1){
            cout<<"please enter the master and than the sub"<<endl;
            cin>>name1;
            cin>>name2;
            try {
                org.add_sub(name1, name2);
            }catch (exception ex) {
                cout<<"please add master that exsit in chart lets try again"<<endl;
            }
        }else{
            cout<<"please enter the new head of chart"<<endl;
            cin>>name1;
            org.add_root(name1);
        }
    }
    cout<<"this is your new chart have fun\n"<<org<<endl;
    cout<<"excellent if we got to here it means that your chart is ready\n"
          "if you want to get all items in chart in preorder way type 0\n"
          "if you want to get all items in chart in level_order way type 1\n"
          "if you want to get all items in chart in reverse_level_order way type 2\n";
    cin>>act;
    switch (act) {
        case 0:
            for (auto it = org.begin_level_order(); it != org.end_level_order(); ++it) {
                cout << *it<<" ";
            }
            break;
        case 1:
            for (auto it1 = org.begin_level_order(); it1 != org.end_level_order(); ++it1) {
                cout << *it1<<" ";
            }
            break;
        case 2:
            for (auto it2 = org.begin_level_order(); it2 != org.end_level_order(); ++it2) {
                cout << *it2<<" ";
            }
            break;
        default:
            cout<<"you didnt enter the one of the number as you told"<<endl;
    }
    cout<<"\ngoodbye"<<endl;

}