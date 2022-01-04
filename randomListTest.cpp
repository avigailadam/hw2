//
// Created by avi on 03/12/2021.
//

#include "HashTable.h"
#include <random>
#include <iostream>

using namespace std;
int main() {
    srand(1);

    LinkedList list;
    int prevLength = 0;
    for (int i = 0; i < 2000; i++) {
        int action = rand() % 2;
        cout << "====" << i << "====" << endl;
        try {
            cout << list;
            int playerID = rand() % 100;
            int groupID = rand() % 108;
            int score = rand() % 138;
            switch (action) {
                case 0:
                    cout << "inserting: " << playerID << endl;
                    if(list.insert(playerID,groupID,score))
                        prevLength++;
                    assert(list.getSize() == prevLength);
                    break;
                case 1:
                    cout << "removing:" << playerID << endl;
                    if(list.remove(playerID))
                        prevLength--;
                    assert(list.getSize() == prevLength);
                    break;
            }
        } catch (...) {}
    }
}