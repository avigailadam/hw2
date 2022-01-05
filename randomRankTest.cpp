//
// Created by avi on 03/12/2021.
//

#include "RankTree.h"
#include <random>
#include <iostream>

int main() {
    srand(1);

    RankTree<int> tree;
    for (int i = 0; i < 1000; i++) {
        int action = rand() % 2;
        cout << "====" << i << "====" << endl;
        tree.debugTree();
        try {
            int x = rand() % 10;
            switch (action) {
                case 0:
                    cout << "inserting: " << x << endl;
                    tree.insert(x);
                    break;
                case 1:
                    cout << "removing:" << x << endl;
                    tree.remove(x);
                    break;
            }
        } catch (...) {}
    }
}