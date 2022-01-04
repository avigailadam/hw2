//
// Created by avi on 03/01/2022.
//

#ifndef HW2_LINKEDLIST_H
#define HW2_LINKEDLIST_H
#include "exceptions.h"

class LinkedList{
    class Node {
        int groupID;
        int playerID;
        int score;
        Node *next;
        friend class LinkedList;
        Node() = default;
    public:
        explicit Node(int playerID, int groupID,int score, Node *node_next = nullptr) : playerID(playerID),groupID(groupID),score(score), next(node_next) {};
        Node(Node &other) = default;
        ~Node() = default;
    };
    Node* head;
    int size;
public:
    class const_iterator;

    LinkedList() : size(0), head(nullptr) {}

    LinkedList(const LinkedList &other);

    LinkedList &operator=(const LinkedList &other);

    bool insert(int playerID, int groupID, int score);

    bool remove(int playerId);

    int getSize() const {
        return size;
    }

    const_iterator begin() const {
        const_iterator first(head);
        return first;
    }

    const_iterator end() const {
        const_iterator result(nullptr);
        return result;
    }

    class const_iterator {
    private:
        Node *current_node;

        explicit const_iterator(Node *node) :
                current_node(node) {}

        friend class LinkedList;

    public:
        const_iterator &operator=(Node *node) {
            current_node = node;
            return *this;
        }

        const_iterator &operator++() //++iterator
        {
            if (current_node) {
                current_node = current_node->next;
            }
            return *this;
        }

        const_iterator operator++(int) //iterator++
        {
            if (current_node == nullptr) {
                throw OutOfRange();
            }
            const_iterator iterator = *this;
            ++(*this);

            return iterator;
        }

        const_iterator(const const_iterator &other) = default;

        ~const_iterator() = default;

        bool operator!=(const const_iterator &other) const {
            return current_node != other.current_node;
        }

        bool operator==(const const_iterator &other) const {
            return !(*this != other);
        }

        int operator*() const {
            if (current_node == nullptr) {
                throw OutOfRange();
            }
            return current_node->playerID;
        }

        int getGroup() const {
            if (current_node == nullptr) {
                throw OutOfRange();
            }
            return current_node->groupID;
        }

        int getScore() const {
            if (current_node == nullptr) {
                throw OutOfRange();
            }
            return current_node->score;
        }
    };
    friend std::ostream &operator<<(std::ostream &os, const LinkedList &list);
};



#endif //HW2_LINKEDLIST_H
