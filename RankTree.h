
#ifndef RANK_TREE
#define RANK_TREE

#define FIELD_OR_DEFAULT(son, field, default) ((son) != nullptr ? (son)->field : default)
#define LEFT_OR_DEFAULT(field, default) FIELD_OR_DEFAULT(leftSon, field, default)
#define RIGHT_OR_DEFAULT(field, default) FIELD_OR_DEFAULT(rightSon, field, default)

#define ASSERT_SON(son, method) if((son) != nullptr) son->method()
#define ASSERT_EQUALS(a, b) if ((a) != (b)) do { std::cout << "tree:" << std::endl; debugTree(0); std::cout << (#a) << ": " << (a) << std::endl << " != " << std::endl << (#b) << ": " <<(b) << std::endl; assert(0); } while(0)
#define BALANCE_TMP(direction) \
    assert(tmp != nullptr); \
    size = sizeFromSons(); \
    tmp->size = size + tmp->direction##Size() + 1; \
    sum = rightSum() + leftSum() + selfSum; \
    tmp->sum = sum + tmp->direction##Sum() + tmp->selfSum; \
    peopleMultipliedByLevel = rightMultiplied() + leftMultiplied() + selfSum * level; \
    tmp->peopleMultipliedByLevel = peopleMultipliedByLevel + tmp->direction##Multiplied() + tmp->selfSum * tmp->level

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
class InnerRankTree {
    T data;
    InnerRankTree<T> *max;
    int height;
    int size;
    int sum;
    int peopleMultipliedByLevel;
    int level;
    int selfSum;
    InnerRankTree<T> *father;
    InnerRankTree<T> *rightSon;
    InnerRankTree<T> *leftSon;


    void inOrderAux(InnerRankTree<T> *root, my_vector<T *> &vec) {
        if (root == nullptr) return;
        inOrderAux(root->leftSon, vec);
        vec.push_back(&(root->data));
        inOrderAux(root->rightSon, vec);
    }

public:

    explicit InnerRankTree(T x) : data(x) {
        father = nullptr;
        rightSon = nullptr;
        leftSon = nullptr;
        max = this;
        height = 0;
        size = 1;
        sum = x.getSum();
        selfSum = x.getSum();
        level = x.getLevel();
        peopleMultipliedByLevel = selfSum * level;
    }


    InnerRankTree<T> *getFather() {
        return father;
    }

    ~InnerRankTree() {
        delete rightSon;
        delete leftSon;
    }

//todo: update size needed in vec const'
//todo: update self sum and peopleMultipliedByLevel
    explicit InnerRankTree(my_vector<T> &vector) : data(vector.at(vector.size() / 2)), height(0), father(nullptr) {
        int size = vector.size();
        my_vector<T> rightVec = sliceVec(vector, (size / 2) + 1, size - 1);
        rightSon = rightVec.empty() ? nullptr : new InnerRankTree<T>(rightVec);
        int rightHeight = rightSon == nullptr ? -1 : rightSon->height;
        my_vector<T> leftVec = sliceVec(vector, 0, (size / 2) - 1);
        leftSon = leftVec.empty() ? nullptr : new InnerRankTree<T>(leftVec);
        int leftHeight = leftSon == nullptr ? -1 : leftSon->height;
        height = std::max(leftHeight, rightHeight) + 1;
        if (rightSon != nullptr) {
            rightSon->father = this;
            size += rightSon->size;
            sum += rightSon->sum;
        }
        if (leftSon != nullptr) {
            leftSon->father = this;
            size += leftSon->size;
            sum += leftSon->sum;

        }

        updateHeight();
        setMax();
    }

    void setMax() {
        max = this;
        while (max->rightSon != nullptr)
            max = max->rightSon;
    }

    InnerRankTree<T> *getMax() const {
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
        int leftHeight = FIELD_OR_DEFAULT(leftSon, height, -1);
        int rightHeight = FIELD_OR_DEFAULT(rightSon, height, -1);
        return leftHeight - rightHeight;
    }

    InnerRankTree<T> *leftRotate() {
        if (rightSon == nullptr)
            return this;
        InnerRankTree<T> *newRightSon = rightSon;
        newRightSon->father = father;
        if (father != nullptr) {
            father->rightSon == this ? father->rightSon = newRightSon : father->leftSon = newRightSon;
            father->updateHeight();
        }
        InnerRankTree<T> *temp = newRightSon->leftSon;
        if (temp != nullptr)
            temp->father = this;
        father = newRightSon;
        newRightSon->leftSon = this;
        rightSon = temp;
        updateHeight();
        newRightSon->updateHeight();
        if (newRightSon->father != nullptr)
            newRightSon->father->updateHeight();
        return newRightSon;
    }

    InnerRankTree<T> *rightRotate() {
        if (leftSon == nullptr) return this;
        InnerRankTree<T> *newLeftSon = leftSon;
        newLeftSon->father = father;
        if (father != nullptr) {
            father->leftSon == this ? father->leftSon = newLeftSon : father->rightSon = newLeftSon;
            father->updateHeight();
        }
        InnerRankTree<T> *temp = newLeftSon->rightSon;
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

    InnerRankTree<T> *internalFind(const T &info) {
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


    int rightSize() {
        return RIGHT_OR_DEFAULT(size, 0);
    }

    int leftSize() {
        return LEFT_OR_DEFAULT(size, 0);
    }

    int rightSum() {
        return RIGHT_OR_DEFAULT(sum, 0);
    }

    int leftSum() {
        return LEFT_OR_DEFAULT(sum, 0);
    }

    int rightMultiplied() {
        return RIGHT_OR_DEFAULT(peopleMultipliedByLevel, 0);
    }

    int leftMultiplied() {
        return LEFT_OR_DEFAULT(peopleMultipliedByLevel, 0);
    }

    int sizeFromSons() {
        return leftSize() + rightSize() + 1;
    }

    int sumFromSons() {
        return leftSum() + rightSum() + selfSum;
    }

    int multipliedFromSons() {
        return leftMultiplied() + rightMultiplied() + selfSum * level;
    }

    void updateAllFromSons() {
        size = sizeFromSons();
        sum = sumFromSons();
        peopleMultipliedByLevel = multipliedFromSons();
    }

    void updateAllSonsAndAllFromSons() {
        if (rightSon)
            rightSon->updateAllFromSons();
        if (leftSon)
            leftSon->updateAllFromSons();
        updateAllFromSons();
    }

    InnerRankTree<T> *balance() {
        int balanceF = balanceFactor();
        if (balanceF > 1) {
            if (leftSon->balanceFactor() >= 0) {
                auto tmp = rightRotate();
                BALANCE_TMP(left);
                return tmp;
            } else if (leftSon->balanceFactor() == -1) {
                leftSon = leftSon->leftRotate();
                InnerRankTree<T> *tmp = rightRotate();
                tmp->updateAllSonsAndAllFromSons();
                return tmp;
            }
        }
        if (balanceF < -1) {
            if (rightSon->balanceFactor() <= 0) {
                InnerRankTree<T> *tmp = leftRotate();
                BALANCE_TMP(right);
                return tmp;
            } else if (rightSon->balanceFactor() == 1) {
                rightSon = rightSon->rightRotate();
                InnerRankTree<T> *tmp = leftRotate();
                tmp->updateAllSonsAndAllFromSons();
                return tmp;
            }
        }
        return this;
    }

    InnerRankTree<T> *insert(T x) {
        assert(x != data);
        size++;
        sum += x.getSum();
        peopleMultipliedByLevel += x.getSum() * x.getLevel();
        if (x > data) {
            if (rightSon == nullptr) {
                rightSon = new InnerRankTree<T>(x);
                rightSon->father = this;
                return rightSon;
            } else {
                return rightSon->insert(x);
            }
        } else {
            assert(x < data);
            if (leftSon != nullptr)
                return leftSon->insert(x);
            leftSon = new InnerRankTree<T>(x);
            leftSon->father = this;
            return leftSon;

        }
    }

    T &find(const T &info) {
        InnerRankTree<T> *result = internalFind(info);
        if (result == nullptr)
            throw NotExist();
        return result->data;
    }

    my_vector<T *> inOrder() {
        my_vector<T *> vec(countTree());
        inOrderAux(this, vec);
        return vec;
    }

    void updateSizeAndSum() {
#ifndef DEBUG
        validatePointers();
#endif
        size = 1 + leftSize() + rightSize();

        sum = selfSum + leftSum() + rightSum();
        if (father != nullptr)
            father->updateSizeAndSum();
        peopleMultipliedByLevel = leftMultiplied() +
                                  rightMultiplied() + selfSum * level;
    }

    InnerRankTree<T> *remove(const T &info) {
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

                InnerRankTree<T> *fatherToUpdate = next->father;
                while (fatherToUpdate != nullptr && fatherToUpdate != this) {
                    fatherToUpdate->updateHeight();
                    fatherToUpdate = fatherToUpdate->father;
                }
                rightSon->father = next;
            }
            next->leftSon = leftSon;
            if (leftSon)
                leftSon->father = next;
            next->updateHeight();
            rightSon = nullptr;
            leftSon = nullptr;
            InnerRankTree<T> *fatherToUpdate = next->father;
            next->father = father;
            while (fatherToUpdate != nullptr && fatherToUpdate != this && fatherToUpdate != next) {
                fatherToUpdate->updateHeight();
                fatherToUpdate = fatherToUpdate->balance();
                fatherToUpdate->updateSizeAndSum();
                fatherToUpdate = fatherToUpdate->father;
            }
            next->updateSizeAndSum();
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
                leftSon->updateSizeAndSum();
            }
            updateHeight();
            auto toReturn = balance();
            updateSizeAndSum();
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
            rightSon->updateSizeAndSum();
        }
        updateHeight();
        auto toReturn = balance();
        updateSizeAndSum();
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
        ASSERT_EQUALS(leftSize() + rightSize() + 1, size);
    }

    void validatePeopleByLevel() {
        ASSERT_SON(leftSon, validatePeopleByLevel);
        ASSERT_SON(rightSon, validatePeopleByLevel);
        ASSERT_EQUALS(leftMultiplied() +
                      rightMultiplied() + level * selfSum,
                      peopleMultipliedByLevel);
    }

    void validateSum() {
        ASSERT_SON(leftSon, validateSum);
        ASSERT_SON(rightSon, validateSum);
        ASSERT_EQUALS(leftSum() + rightSum() + selfSum, sum);
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
        validateSum();
        validatePeopleByLevel();
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

    T &closestFromAbove(T &x) {
        if (data == x)
            return x;
        if (data > x) {
            if (leftSon == nullptr || leftSon->data < x)
                return data;
            assert(leftSon->data > x);
            return leftSon->closestFromAbove(x);
        }
        assert(data < x);
        if (rightSon == nullptr)
            return data;
        assert(rightSon != nullptr);
        if (rightSon->data > x)
            return rightSon->data;
        return rightSon->closestFromAbove(x);
    }

    T &closestFromBelow(T &x) {
        if (data == x)
            return x;
        if (data < x) {
            if (rightSon == nullptr || rightSon->data > x)
                return data;
            assert(leftSon->data > x);
            return rightSon->closestFromBelow(x);
        }
        assert(data > x);
        if (leftSon == nullptr)
            return data;
        assert(rightSon != nullptr);
        if (leftSon->data < x)
            return leftSon->data;
        return leftSon->closestFromBelow(x);
    }

    double findTopMMult(int m) const {
        InnerRankTree<T> *current = this;
        while (current->sum > m && current->rightSon) {
            current = current->rightSon;
        }
        double curr_sum = 0;
        int people_left_to_sum = m;
        if (current->rightSon) {
            curr_sum = current->rightSon->peopleMultipliedByLevel;
            people_left_to_sum = m - current->rightSon->sum;
        }
        if(people_left_to_sum<= current->sum){
            curr_sum+= (current->level*current->sum);
            return curr_sum;
        }
        else{
            curr_sum= curr_sum + (current->size*current->level) +
                    current->leftSon.internal_findTopMMult(people_left_to_sum - current->sum);
            return curr_sum;
        }

    }

    int totalSumOver() {
        int fromFather = 0;
        if (father != nullptr && father->data > data)
            fromFather = father->totalSumOver();
        return selfSum + rightSum() + fromFather;
    }

    int totalSumUnder() {
        int fromFather = 0;
        if (father != nullptr && father->data < data)
            fromFather = father->totalSumUnder();
        return selfSum + leftSum() + fromFather;
    }
    int getSum() const{
        return sum;
    }
};

template<class T>
class RankTree {
    InnerRankTree<T> *tree;

    void validate() {
#ifndef DEBUG
        if (tree) {
            tree->validate();
            tree->getFather() == nullptr;
        }
#endif
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
        InnerRankTree<T> *maxNode = tree->getMax();
        return maxNode->getData();
    }

    bool isEmpty() const {
        return tree == nullptr;
    }

    void insert(T x) {
        if (tree == nullptr) {
            tree = new InnerRankTree<T>(x);
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
        InnerRankTree<T> *newTree = tree->remove(info);
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
        tree = new InnerRankTree<T>(vector);
    }


    void balance(InnerRankTree<T> *current) {
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

    void merge(RankTree<T> *other) {
        assert(tree != nullptr);
        my_vector<T *> vec1 = tree->inOrder();
        my_vector<T *> vec2 = other->tree->inOrder();
        my_vector<T> merged = merge(vec1, vec2);
        delete tree;
        tree(merged);
    }

    int totalSumOver(const T &x) {
        if (tree == nullptr)
            return -1;
        T &info = tree->closestFromAbove(info);
        if (info < x)
            return 0;
        return (tree->internalFind(info))->totalSumOver();
    }

    int totalSumUnder(const T &x) {
        if (tree == nullptr)
            return -1;
        T &info = tree->closestFromBelow(info);
        if (info > x)
            return 0;
        return (tree->internalFind(info))->totalSumUnder();
    }
    int getSum() const{
        return tree->getSum();
    }
    double findTopMMult(int m) const{
        return tree->findTopMMult(m);
    }
};

#endif