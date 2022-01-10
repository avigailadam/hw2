//
// Created by avi on 03/12/2021.
//

#include "RankTree.h"
#include <random>
#include <iostream>

class Hagai {
    int level;
    int sum;

public:
    Hagai(int level, int sum) : level(level), sum(sum) {}

    ~Hagai() = default;

    Hagai &operator=(const Hagai &other) {
        level = other.level;
        sum = other.sum;
        return *this;
    }

    Hagai(const Hagai &other) {
        level = other.level;
        sum = other.sum;
    }

    int getSum() const {
        return sum;
    }

    int getLevel() const {
        return level;
    }

    bool operator>(const Hagai &other) const {
        return level > other.level;
    }

    bool operator<(const Hagai &other) const {
        return level < other.level;
    }

    bool operator==(const Hagai &other) const {
        return level == other.level;
    }

    bool operator!=(const Hagai &other) const {
        return level != other.level;
    }

    friend std::ostream &operator<<(std::ostream &os, const Hagai &hagai) {
        os << "level: " << hagai.level << ", sum: " << hagai.sum;
        return os;
    }
};


int main() {
    srand(1);

    RankTree<Hagai> tree;
    for (int i = 0; i < 1000; i++) {
        int action = rand() % 2;
        cout << "====" << i << "====" << endl;
        tree.debugTree();
        try {
            int y = rand() % 10 + 1;
            int z = rand() % 10 + 1;
            Hagai x(y, z);
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