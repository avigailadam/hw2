
#include "exceptions.h"
#include <cmath>
#include <iostream>
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
    int innerSize;
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
        innerSize = 1;
    }


    InnerAvlTree<T> *getFather() {
        return father;
    }

    int getInnerSize() {
        return innerSize;
    }

    ~InnerAvlTree() {
        delete rightSon;
        delete leftSon;
    }
//todo: update innerSize needed in vec const'
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
        }
        if (leftSon != nullptr) {
            leftSon->father = this;
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

    InnerAvlTree<T> *Left_rotate() {
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
        newRightSon->innerSize =
                2 + newRightSon->RightSon->innerSize + newRightSon->LeftSon->innerSize + leftSon->innerSize;
        return newRightSon;
    }

    InnerAvlTree<T> *Right_rotate() {
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
        newLeftSon->innerSize =
                2 + newLeftSon->leftSon->innerSize + newLeftSon->RightSon->innerSize + rightSon->innerSize;
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
                return Right_rotate();
            } else if (leftSon->balanceFactor() == -1) {
                leftSon = leftSon->Left_rotate();
                return Right_rotate();
            }
        }
        if (balanceF < -1) {
            if (rightSon->balanceFactor() <= 0) {
                return Left_rotate();
            } else if (rightSon->balanceFactor() == 1) {
                rightSon = rightSon->Right_rotate();
                return Left_rotate();
            }
        }
        return this;
    }

    InnerAvlTree<T> *insert(T x) {
        if (x == data)
            throw AlreadyExist();
        if (x > data) {
            if (rightSon == nullptr) {
                rightSon = new InnerAvlTree<T>(x);
                rightSon->father = this;
                return rightSon;
            } else {
                return rightSon->insert(x);
            }
        } else if (x < data) {
            if (leftSon == nullptr) {
                leftSon = new InnerAvlTree<T>(x);
                leftSon->father = this;
                return leftSon;
            } else {
                return leftSon->insert(x);
            }
        }
        updateHeight();
        setMax();
        return this;
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
            while (fatherToUpdate != nullptr && fatherToUpdate != this && fatherToUpdate != next) {
                fatherToUpdate->updateHeight();
                fatherToUpdate = fatherToUpdate->balance();
                fatherToUpdate = fatherToUpdate->father;
            }
            next->father = father;
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
            }
            updateHeight();
            return balance();
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
        }
        updateHeight();
        return balance();
    }

    bool notExists(InnerAvlTree<T> *check) const {
        if (rightSon != nullptr)
            rightSon->notExists(check);
        if (leftSon != nullptr)
            leftSon->notExists(check);
        return true;
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


};

template<class T>
class RankTree {
    InnerAvlTree<T> *tree;

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

        auto inserted = tree->insert(x);
        InnerAvlTree<T> *obj = internalFind(x);
        while (obj != nullptr) {
            obj = obj->getFather();
            obj->innerSize++;
        }
        balance(inserted);
        tree->setMax();
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
    }

    my_vector<T> getNLowest(int n) {
        my_vector<T> vec(n);
        if (tree != nullptr)
            tree->getNLowest(n, &vec);
        return vec;
    }

};

#endif