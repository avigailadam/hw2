//
// Created by avi on 03/01/2022.
//

#include <cmath>
#include <iostream>
#include "HashTable.h"

int HashTable::realSize() {
    int counter = 0;
    for (int i = 0; i < maxSize; ++i) {
        counter += table[i].getSize();;
    }
    return counter;
}

void HashTable::insert(int playerID, int groupID, int score) {
    if (actualSize >= maxSize)
        extendTable();
    if (exists(playerID))
        throw AlreadyExist();
    LinkedList *list = &table[hash(playerID)];
    list->insert(playerID, groupID, score);
    actualSize++;
    assert(actualSize == realSize());
}

void HashTable::extendTable() {
    maxSize *= 2;
    modulo = maxSize + 1;
    rehash(maxSize / 2);
}

void HashTable::remove(int playerID) {
    LinkedList *list = &table[hash(playerID)];
    if (!exists(playerID))
        throw NotExist();
    list->remove(playerID);
    actualSize--;
    if (actualSize < maxSize / 2)
        shrinkTable();
    assert(actualSize == realSize());
}

std::ostream &operator<<(std::ostream &os, const HashTable &hash) {
    os << "MaxSize: " << hash.maxSize << ", ActualSize:" << hash.actualSize << std::endl;
    for (int i = 0; i < hash.maxSize; ++i) {
        if (hash.table[i].getSize() != 0)
            os << i << ": " << hash.table[i];
    }
    os << std::endl;
    return os;
}

void HashTable::shrinkTable() {
    maxSize /= 2;
    modulo = maxSize + 1;
    rehash(maxSize * 2);
}

void HashTable::rehash(int prevSize) {
    auto *newTable = new LinkedList[maxSize];
    for (int i = 0; i < prevSize; ++i) {
        LinkedList *currList = &table[i];
        for (LinkedList::const_iterator it = currList->begin(); it != currList->end(); it++)
            newTable[hash(*it)].insert(*it, it.getGroup(), it.getScore());
    }
    delete[] table;
    table = newTable;
}

int HashTable::hash(int num) const {
    return (num % modulo) % maxSize;
}

int HashTable::getPlayerGroup(int playerID) {
    LinkedList *list = &table[hash(playerID)];
    for (LinkedList::const_iterator it = list->begin(); it != list->end(); it++)
        if (*it == playerID)
            return it.getGroup();
    throw NotExist();
}

int HashTable::getPlayerScore(int playerID) {
    LinkedList *list = &table[hash(playerID)];
    for (LinkedList::const_iterator it = list->begin(); it != list->end(); it++)
        if (*it == playerID)
            return it.getScore();
    throw NotExist();
}

bool HashTable::exists(int playerID) {
    LinkedList *list = &table[hash(playerID)];
    for (LinkedList::const_iterator it = list->begin(); it != list->end(); it++)
        if (*it == playerID)
            return true;
    return false;
}


