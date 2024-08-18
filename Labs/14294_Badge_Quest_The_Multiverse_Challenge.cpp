#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

struct Pokemon {
    int id;
    int level;
};
/*
functions
Insert int1 int2 int3: lst,id,lvl
Remove int1 int2:lst, id
Rotate int1 int2: lst ,rotation_num
Reorder int: lst
Reverse int1 int2 int3: lst ,start,end
MergeListsPreserveOrder int1 int2: ls1,ls2
*/
void insertPokemon(list<Pokemon>& lst, int id, int level) {
    lst.push_back({id, level});
}

void removePokemon(list<Pokemon>& lst, int id) {
    lst.remove_if([id](Pokemon& p) { 
        return p.id == id; 
        });
}

void rotateList(list<Pokemon>& lst, int rotations) {
    if (lst.empty()){ 
        return;
    }
    //cout<<rotations<<endl;
    rotations = rotations% lst.size();
    //cout<<rotations<<endl;
    for (int i = 0; i < rotations; i++) {
        //Remember to move first element
        lst.push_front(lst.back());
        lst.pop_back();
    }
}

void reorderList(list<Pokemon>& lst) {
    vector<Pokemon> evens, odds;
    int index = 0;
    for (auto& p : lst) {
        if (index % 2 == 0) 
            evens.push_back(p);
        else 
            odds.push_back(p);
        index++;
    }
    lst.clear();
    //new list --> even,odd
    lst.insert(lst.end(), evens.begin(), evens.end());
    lst.insert(lst.end(), odds.begin(), odds.end());
}

void reverseList(list<Pokemon>& lst, int start, int end){
    //list , index start , index end
    //cout<<"IDK how to do this"<<endl;
    /*
        for (int i = 0; i < 3; ++i) {
            for (const auto& p : lst[i]) {
                        cout << "ID: " << p.id << " Level: " << p.level << endl;
            }
        }
    */
}

void mergeLists(list<Pokemon>& lst1, list<Pokemon>& lst2){
    //cout<<"HI"<<endl;
}
int main() {
    int n, x;
    cin >> n >> x;//list_num,commands
    vector<list<Pokemon>> dimensions(n);
    string command;

    while (x > 0) {
        cin >> command;
        if (command == "Insert") {
            int index, id, level;
            cin >> index >> id >> level;
            insertPokemon(dimensions[index], id, level);
        } else if (command == "Remove") {
            int index, id;
            cin >> index >> id;
            //before remove
            /*
            cout<<"Before remove";
            for (int i = 0; i < n; ++i) {
                for (const auto& p : dimensions[i]) {
                    cout << "ID: " << p.id << " Level: " << p.level << endl;
                }
            }
            */
            removePokemon(dimensions[index], id);
            //after remove
            //cout<<"After remove"<<endl;
            /*
            for (int i = 0; i < n; ++i) {
                for (const auto& p : dimensions[i]) {
                    cout << "ID: " << p.id << " Level: " << p.level << endl;
                }
            }
            */
        } else if (command == "Rotate") {
            int index, rotations;
            cin >> index >> rotations;
            rotateList(dimensions[index], rotations);
        } else if (command == "Reorder") {
            int index;
            cin >> index;
            reorderList(dimensions[index]);
        } else if (command == "Reverse") {
            int index, start, end;
            cin >> index >> start >> end;
            //do later
            reverseList(dimensions[index], start, end);
            //cout << index << endl;
        } else if (command == "MergeListsPreserveOrder") {
            //do later
            int index1, index2;//lst1,lst2
            cin >> index1 >> index2;
            //mergeLists(dimensions[index1], dimensions[index2]);
        }
        x--;
        /*
        for (int i = 0; i < n; ++i) {
            for (const auto& p : dimensions[i]) {
                        cout << "ID: " << p.id << " Level: " << p.level << endl;
                }
            }
        */
    }

    //Print out
    for (int i = 0; i < n; ++i) {
        cout << "List " << i << endl;
        if (dimensions[i].empty()) {
            cout << "Empty" << endl;
        } else {
            for (const auto& p : dimensions[i]) {
                cout << "ID: " << p.id << " Level: " << p.level << endl;
            }
        }
    }

    return 0;
}

/*
3
11
Insert 0 1 1
Insert 0 1 1
Insert 0 1 1
Insert 1 3 2
Insert 1 2 1
Insert 1 2 2
Insert 1 1 2
Insert 2 1 1
Insert 2 1 1
Insert 1 1 1
Reverse 1 2 4

*/