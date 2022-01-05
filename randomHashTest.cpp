//
// Created by avi on 03/12/2021.
//

#include "HashTable.h"
#include <random>
#include <iostream>

using namespace std;
int main() {
    srand(1);

    HashTable hash;
    for (int i = 0; i < 20; i++) {
        int action = rand() % 2;
        cout << "====" << i << "====" << endl;
        try {
            int playerID = rand() % 150;
            int groupID = rand() % 38;
            int score = rand() % 38;
            switch (action) {
                case 0:
                    cout << "inserting: " << playerID << endl;
                    hash.insert(playerID,groupID,score);
                    cout << hash;
                    break;
                case 1:
                    cout << "removing:" << playerID << endl;
                    hash.remove(playerID);
                    cout << hash;
                    break;
            }
        }
        catch (NotExist &exist) {
            cout<<"Nothing to remove"<<endl;
        }
        catch (AlreadyExist &exist) {
            cout<<"Nothing to insert"<<endl;
        }
        catch (...) {}
    }
}