
#ifndef RANK_TREE
#define RANK_TREE

#define FIELD_OR_DEFAULT(son, field, default) ((son) != nullptr ? (son)->field : default)
#define ASSERT_SON(son, method) if((son) != nullptr) son->method()
#define ASSERT_EQUALS(a, b) if ((a) != (b)) do { std::cout << "tree:" << std::endl; debugTree(0); std::cout << (#a) << ": " << (a) << std::endl << " != " << std::endl << (#b) << ": " <<(b) << std::endl; assert(0); } while(0)

#include "exceptions.h"
#include <cmath>
#include <iostream>
#include <cassert>
#include "my_vector.h"

using std::cout;
using std::endl;

template<class T>
my_vector<T> sliceVec(my_vector<T> &vector, int start, int end) {
    return vector.slice(start, end);
}


template<class T>
class InnerAvlTree {
    T data;
    InnerAvlTree<T> *max;
    int height;
    int size;
    InnerAvlTree<T> *father;
    InnerAvlTree<T> *rightSon;
    InnerAvlTree<T> *leftSon;


    void inOrderAux(InnerAvlTree<T> *root, my_vector<T *> &vec) {
        if (root == nullptr) return;
        inOrderAux(root->leftSon, vec);
        vec.push_back(&(root->data));
        inOrderAux(root->rightSon, vec);
    }

public:

    explicit InnerAvlTree(T x) : data(x) {
        father = nullptr;
        rightSon = nullptr;
        leftSon = nullptr;
        max = this;
        height = 0;
        size = 1;
    }


    InnerAvlTree<T> *getFather() {
        return father;
    }

    ~InnerAvlTree() {
        delete rightSon;
        delete leftSon;
    }

//todo: update size needed in vec const'

    explicit InnerAvlTree(my_vector<T> &vector) : data(vector.at(vector.size() / 2)), height(0), father(nullptr) {
        int size = vector.size();
        my_vector<T> rightVec = sliceVec(vector, (size / 2) + 1, size - 1);
        rightSon = rightVec.empty() ? nullptr : new InnerAvlTree<T>(rightVec);
        int rightHeight = rightSon == nullptr ? -1 : rightSon->height;
        my_vector<T> leftVec = sliceVec(vector, 0, (size / 2) - 1);
        leftSon = leftVec.empty() ? nullptr : new InnerAvlTree<T>(leftVec);
        int leftHeight = leftSon == nullptr ? -1 : leftSon->height;
        height = std::max(leftHeight, rightHeight) + 1;
        if (rightSon != nullptr) {
            rightSon->father = this;
            size += rightSon->size;
        }
        if (leftSon != nullptr) {
            leftSon->father = this;
            size += leftSon->size;
        }

        updateHeight();
        setMax();
    }

    void setMax() {
        max = this;
        while (max->rightSon != nullptr)
            max = max->rightSon;
    }

    InnerAvlTree<T> *getMax() const {
        return max;
    }


    T &getData() {
        return data;
    }

    void updateHeight() {
        int leftHeight = leftSon == nullptr ? -1 : leftSon->height;
        int rightHeight = rightSon == nullptr ? -1 : rightSon->height;
        height = std::max(leftHeight, rightHeight) + 1;
    }

private:

    int balanceFactor() const {
        int leftHeight = leftSon == nullptr ? -1 : leftSon->height;
        int rightHeight = rightSon == nullptr ? -1 : rightSon->height;
        return leftHeight - rightHeight;
    }

    InnerAvlTree<T> *leftRotate() {
        if (rightSon == nullptr)return this;
        InnerAvlTree<T> *newRightSon = rightSon;
        newRightSon->father = father;
        if (father != nullptr) {
            father->rightSon == this ? father->rightSon = newRightSon : father->leftSon = newRightSon;
            father->updateHeight();
        }
        InnerAvlTree<T> *temp = newRightSon->leftSon;
        if (temp != nullptr)
            temp->father = this;
        father = newRightSon;
        newRightSon->leftSon = this;
        rightSon = temp;
        updateHeight();
        newRightSon->updateHeight();
        if (newRightSon->father != nullptr) {
            newRightSon->father->updateHeight();
        }
        return newRightSon;
    }

    InnerAvlTree<T> *rightRotate() {
        if (leftSon == nullptr) return this;
        InnerAvlTree<T> *newLeftSon = leftSon;
        newLeftSon->father = father;
        if (father != nullptr) {
            father->leftSon == this ? father->leftSon = newLeftSon : father->rightSon = newLeftSon;
            father->updateHeight();
        }
        InnerAvlTree<T> *temp = newLeftSon->rightSon;
        if (temp != nullptr)
            temp->father = this;
        father = newLeftSon;
        newLeftSon->rightSon = this;
        leftSon = temp;
        updateHeight();
        newLeftSon->updateHeight();
        if (newLeftSon->father != nullptr)
            newLeftSon->father->updateHeight();
        return newLeftSon;
    }

public:
    void nullifyFather() {
        father = nullptr;
    }

    InnerAvlTree<T> *internalFind(const T &info) {
        if (data == info) {
            return this;
        }
        if (info > data) {
            if (rightSon != nullptr) {
                return rightSon->internalFind(info);
            }
            return nullptr;
        }
        return leftSon == nullptr ? nullptr : leftSon->internalFind(info);
    }


    InnerAvlTree<T> *balance() {
        int balanceF = balanceFactor();
        if (balanceF > 1) {
            if (leftSon->balanceFactor() >= 0) {
                InnerAvlTree<T> *tmp = rightRotate();
                size = FIELD_OR_DEFAULT(rightSon, size, 0) +
                       FIELD_OR_DEFAULT(leftSon, size, 0) + 1;
                tmp->size = size + FIELD_OR_DEFAULT(tmp->leftSon, size, 0) + 1;
                return tmp;
            } else if (leftSon->balanceFactor() == -1) {
                leftSon = leftSon->leftRotate();
                InnerAvlTree<T> *tmp = rightRotate();
                if (tmp->rightSon != nullptr) {
                    tmp->rightSon->size = FIELD_OR_DEFAULT(tmp->rightSon->rightSon, size, 0) +
                                          FIELD_OR_DEFAULT(tmp->rightSon->leftSon, size, 0) + 1;
                }
                int rightSize = (tmp->rightSon != nullptr) ? tmp->rightSon->size : 0;
                if (tmp->leftSon != nullptr) {
                    tmp->leftSon->size = FIELD_OR_DEFAULT(tmp->leftSon->rightSon, size, 0) +
                                         FIELD_OR_DEFAULT(tmp->leftSon->leftSon, size, 0) + 1;
                }
                int leftSize = (tmp->leftSon != nullptr) ? tmp->leftSon->size : 0;
                tmp->size = 1 + leftSize + rightSize;
                return tmp;
            }
        }
        if (balanceF < -1) {
            if (rightSon->balanceFactor() <= 0) {
                InnerAvlTree<T> *tmp = leftRotate();
                size = FIELD_OR_DEFAULT(rightSon, size, 0) +
                       FIELD_OR_DEFAULT(leftSon, size, 0) + 1;
                tmp->size = size + FIELD_OR_DEFAULT(tmp->rightSon, size, 0) + 1;
                return tmp;
            } else if (rightSon->balanceFactor() == 1) {
                rightSon = rightSon->rightRotate();
                InnerAvlTree<T> *tmp = leftRotate();
                if (tmp->rightSon != nullptr) {
                    tmp->rightSon->size = FIELD_OR_DEFAULT(tmp->rightSon->rightSon, size, 0) +
                                          FIELD_OR_DEFAULT(tmp->rightSon->leftSon, size, 0) + 1;
                }
                int rightSize = (tmp->rightSon != nullptr) ? tmp->rightSon->size : 0;
                if (tmp->leftSon != nullptr) {
                    tmp->leftSon->size = FIELD_OR_DEFAULT(tmp->leftSon->rightSon, size, 0) +
                                         FIELD_OR_DEFAULT(tmp->leftSon->leftSon, size, 0) + 1;
                }
                int leftSize = (tmp->leftSon != nullptr) ? tmp->leftSon->size : 0;
                tmp->size = 1 + leftSize + rightSize;
                return tmp;
            }
        }
        return this;
    }

    InnerAvlTree<T> *insert(T x) {
        assert(x != data);
        size++;
        if (x > data) {
            if (rightSon == nullptr) {
                rightSon = new InnerAvlTree<T>(x);
                rightSon->father = this;
                return rightSon;
            } else {
                return rightSon->insert(x);
            }
        } else {
            assert(x < data);
            if (leftSon == nullptr) {
                leftSon = new InnerAvlTree<T>(x);
                leftSon->father = this;
                return leftSon;
            } else {
                return leftSon->insert(x);
            }
        }
    }

    T &find(const T &info) {
        InnerAvlTree<T> *result = internalFind(info);
        if (result == nullptr)
            throw NotExist();
        return result->data;
    }

    my_vector<T *> inOrder() {
        my_vector<T *> vec(countTree());
        inOrderAux(this, vec);
        return vec;
    }

    void updateSize() {
        validatePointers();
        size = 1 + FIELD_OR_DEFAULT(leftSon, size, 0) + FIELD_OR_DEFAULT(rightSon, size, 0);
        if (father != nullptr)
            father->updateSize();
    }

//todo: update inner size is needed here
    InnerAvlTree<T> *remove(const T &info) {
        if (data == info) {
            auto next = rightSon;
            if (next == nullptr) {
                auto result = leftSon;
                leftSon = nullptr;
                return result;
            }
            while (next->leftSon != nullptr)
                next = next->leftSon;
            if (next != rightSon) {
                if (next->rightSon != nullptr) {
                    next->rightSon->father = next->father;
                }
                next->father->leftSon = next->rightSon;
                next->rightSon = rightSon;

                InnerAvlTree<T> *fatherToUpdate = next->father;
                while (fatherToUpdate != nullptr && fatherToUpdate != this) {
                    fatherToUpdate->updateHeight();
                    fatherToUpdate = fatherToUpdate->father;
                }
                rightSon->father = next;
            }
            next->leftSon = leftSon;
            if (leftSon) {
                leftSon->father = next;
            }
            next->updateHeight();
            rightSon = nullptr;
            leftSon = nullptr;
            InnerAvlTree<T> *fatherToUpdate = next->father;
            next->father = father;
            next->updateSize();
            while (fatherToUpdate != nullptr && fatherToUpdate != this && fatherToUpdate != next) {
                fatherToUpdate->updateHeight();
                fatherToUpdate = fatherToUpdate->balance();
                fatherToUpdate->updateSize();
                fatherToUpdate = fatherToUpdate->father;
            }
            return next;
        }
        if (data > info) {
            if (leftSon == nullptr)
                throw NotExist();
            auto newRoot = this->leftSon->remove(info);
            if (newRoot != leftSon)
                delete leftSon;
            leftSon = newRoot;
            if (leftSon != nullptr) {
                leftSon->updateHeight();
                leftSon->father = this;
                leftSon = leftSon->balance();
                leftSon->updateSize();
            }
            updateHeight();
            auto toReturn = balance();
            updateSize();
            return toReturn;
        }
        if (rightSon == nullptr)
            throw NotExist();
        auto newRoot = rightSon->remove(info);
        if (newRoot != rightSon)
            delete rightSon;
        rightSon = newRoot;
        if (rightSon != nullptr) {
            rightSon->updateHeight();
            rightSon->father = this;
            rightSon = rightSon->balance();
            rightSon->updateSize();
        }
        updateHeight();
        auto toReturn = balance();
        updateSize();
        return toReturn;
    }

    void getNLowest(int n, my_vector<T> *vec) {
        if (vec->size() == n)
            return;
        if (leftSon != nullptr)
            leftSon->getNLowest(n, vec);
        if (n == vec->size())
            return;
        vec->push_back(data);
        if (rightSon != nullptr)
            rightSon->getNLowest(n, vec);
    }

    int countTree() {
        int counter = 1;
        if (rightSon != nullptr)
            counter += rightSon->countTree();
        if (leftSon != nullptr)
            counter += leftSon->countTree();
        return counter;
    }

    void validateHeight() {
        ASSERT_SON(leftSon, validateHeight);
        ASSERT_SON(rightSon, validateHeight);
        ASSERT_EQUALS(std::max(FIELD_OR_DEFAULT(leftSon, height, -1), FIELD_OR_DEFAULT(rightSon, height, -1)) + 1,
                      height);
    }

    void validateSize() {
        ASSERT_SON(leftSon, validateSize);
        ASSERT_SON(rightSon, validateSize);
        ASSERT_EQUALS(FIELD_OR_DEFAULT(leftSon, size, 0) + FIELD_OR_DEFAULT(rightSon, size, 0) + 1, size);
    }

    void validateBalance() {
        ASSERT_SON(leftSon, validateBalance);
        ASSERT_SON(rightSon, validateBalance);
        int leftHeight = FIELD_OR_DEFAULT(leftSon, height, -1);
        int rightHeight = FIELD_OR_DEFAULT(rightSon, height, -1);
        if (abs(leftHeight - rightHeight) > 1) {
            std::cout << "Unbalanced tree; left height: " << leftHeight << "; right height: " << rightHeight
                      << std::endl;
            assert(0);
        }
    }

    void validateUnique() {
        auto vec = inOrder();
        for (int i = 0; i < vec.size() - 1; ++i) {
            if (false == (*(vec.at(i)) < *(vec.at(i + 1)))) {
                debugTree(0);
                cout << vec << endl;
            }
        }
    }

    void validateFathers() {
        if (father != nullptr)
            father->validateFathers();
    }

    void validatePointers() {
        validateFathers();
        if (leftSon != nullptr) {
            leftSon->validatePointers();
            leftSon->father == this;
        }
        if (rightSon != nullptr) {
            rightSon->validatePointers();
            rightSon->father == this;
        }
    }

    void validate() {
        validateHeight();
        validateSize();
        validateBalance();
        validateUnique();
        validatePointers();
    }

    void debugTree(int depth) {
        for (int i = 0; i < depth; ++i) {
            cout << "  ";
        }
        cout << data << endl;
        if (leftSon != nullptr) {
            for (int i = 0; i < depth; ++i) {
                cout << "  ";
            }
            cout << "Left son" << endl;
            leftSon->debugTree(depth + 1);
        }
        if (rightSon != nullptr) {
            for (int i = 0; i < depth; ++i) {
                cout << "  ";
            }
            cout << "Right son" << endl;
            rightSon->debugTree(depth + 1);
        }
    }

    bool exists(const T &x) {
        return internalFind(x) != nullptr;
    }
};

template<class T>
class RankTree {
    InnerAvlTree<T> *tree;

    void validate() {
        if (tree) {
            tree->validate();
            tree->getFather() == nullptr;
        }
    }

public:
    RankTree() {
        tree = nullptr;
    }

    ~RankTree() {
        delete tree;
    }

    T getMax() const {
        if (tree == nullptr)
            throw NotExist();
        InnerAvlTree<T> *maxNode = tree->getMax();
        return maxNode->getData();
    }

    bool isEmpty() const {
        return tree == nullptr;
    }

    void insert(T x) {
        if (tree == nullptr) {
            tree = new InnerAvlTree<T>(x);
            return;
        }
        if (tree->exists(x))
            throw AlreadyExist();
        auto inserted = tree->insert(x);
        balance(inserted);
        tree->setMax();
        validate();
    }


    T &find(const T &info) {
        if (tree == nullptr)
            throw NotExist();
        return tree->find(info);
    }

    my_vector<T *> inOrder() {
        if (tree == nullptr)
            return my_vector<T *>();
        auto result = tree->inOrder();
        if (!result.is_sorted_ptr()) {
            for (int i = 0; i < result.size(); ++i) {
                cout << *result.at(i) << ", ";
            }
            cout << endl;
        }
        return result;
    }

    void remove(const T &info) {
        if (tree == nullptr)
            throw NotExist();
        bool willRootBeDeleted = info == tree->getData();
        InnerAvlTree<T> *newTree = tree->remove(info);
        if (willRootBeDeleted) {
            if (newTree != nullptr) {
                newTree->nullifyFather();
                newTree = newTree->balance();
            }
            delete tree;
        }
        tree = newTree;
        if (tree != nullptr) {
            tree->updateHeight();
            tree->setMax();
        }
        validate();
    }

    void recursiveAvl(my_vector<T> &vector) {
        if (vector.empty())
            return;
        tree = new InnerAvlTree<T>(vector);
    }


    void balance(InnerAvlTree<T> *current) {
        while (current != nullptr) {
            current = current->balance();
            if (current->getFather() == nullptr) {
                break;
            }
            current = current->getFather();
            current->updateHeight();
        }
        tree = current;
        validate();
    }

    my_vector<T> getNLowest(int n) {
        my_vector<T> vec(n);
        if (tree != nullptr)
            tree->getNLowest(n, &vec);
        return vec;
    }

    void debugTree() {
        if (tree == nullptr) {
            cout << "null" << endl;
            return;
        }
        tree->debugTree(0);
    }

    bool isEmpty() {
        return tree == nullptr;
    }

    void merge(RankTree<T> &other) {
        my_vector<T *> vec1 = tree->inOrder();
        my_vector<T *> vec2 = other.tree->inOrder();
        my_vector<T> merged = merge(vec1, vec2);
        delete tree;
        tree(merged);
    }

    my_vector<T> merge(my_vector<T *> v1, my_vector<T *> v2) {
        my_vector<T> res(v1.size() + v2.size());
        auto i1 = 0;
        auto i2 = 0;
        while (i1 < v1.size() || i2 < v2.size()) {
            if (i1 != v1.size() && (i2 == v2.size() || *v1.at(i1) < *v2.at(i2))) {
                T *pLevel = v1.at(i1);
                res.push_back(*pLevel);
                i1++;
                continue;
            }
            T *pLevel = v2.at(i2);
            res.push_back(*pLevel);
            i2++;
        }
        return res;
    }
};

#endif