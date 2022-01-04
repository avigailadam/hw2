//
// Created by avi on 03/01/2022.
//

#ifndef HW2_HASHTABLE_H
#define HW2_HASHTABLE_H

#include <cassert>
#include "exceptions.h"
#include "LinkedList.h"
#define INITIAL_SIZE 2
#define INITIAL_PRIME 3


class HashTable {
    LinkedList* table;
    int maxSize;
    int actualSize;
    int modulo;

    void extendTable();
    void shrinkTable();
    void rehash(int prevSize);
    int hash(int num);

    int validLength();

public:

    HashTable() : table(new LinkedList[INITIAL_SIZE]), maxSize(INITIAL_SIZE), actualSize(0), modulo(INITIAL_PRIME) {}

    virtual ~HashTable() {
        delete[] table;
    }

    void insert(int playerID, int groupID, int score);

    void remove(int playerID);

    friend std::ostream &operator<<(std::ostream &os, const HashTable &hash);
};




#endif //HW2_HASHTABLE_H
