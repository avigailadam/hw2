//
// Created by avi on 03/01/2022.
//

#include "LinkedList.h"
#include <iostream>
#include <cassert>

void LinkedList::remove(int playerId) {
    const_iterator before = begin();
    if ((*before).getId() == playerId) {
        head = before.current_node->next;
        delete before.current_node;
        size--;
        return;
    }
    const_iterator current = before++;
    while (current != end()) {
        if ((*current).getId() != playerId) {
            before = current++;
            continue;
        }
        before.current_node->next = current.current_node->next;
        delete current.current_node;
        size--;
        return;
    }
}

LinkedList::LinkedList(const LinkedList &other) : size(other.size) {
    if (other.head == nullptr) {
        head = nullptr;
        return;
    }
    head = new Node(*other.head);
    Node *current = head;
    Node *current_other = other.head;
    while (current_other->next != nullptr) {
        current->next = new Node(*current_other->next);
        current_other = current_other->next;
        current = current->next;
    }
}

LinkedList &LinkedList::operator=(const LinkedList &other) {
    if (this == &other) {
        return *this;
    }

    Node *i = head;

    while (i != nullptr) {
        Node *temp = i->next;
        delete i;
        i = temp;
    }

    delete end().current_node;
    head = nullptr;
    size = other.size;
    if (other.head == nullptr) {
        return *this;
    }
    head = new Node(*other.head);
    Node *current = head;
    Node *current_other = other.head;
    while (current_other->next != nullptr) {
        current->next = new Node(*current_other->next);
        current_other = current_other->next;
        current = current->next;
    }
    return *this;
}

void LinkedList::insert(int playerId, int groupId, int score, int level) {
    Node *new_node = new Node(playerId, groupId, score, level);
    if (head == nullptr) {
        head = new_node;
        size++;
        return;
    }
    const_iterator current = begin();
    while (current != end()) {
        if ((*current).getId() == playerId) {
            delete new_node;
            return;
        }
        if (current.current_node->next == nullptr)
            break;
        current++;
    }
    assert(current.current_node != nullptr);
    current.current_node->next = new_node;
    size++;
}

std::ostream &operator<<(std::ostream &os, const LinkedList &list) {
    for (typename LinkedList::const_iterator i = list.begin(); i != list.end(); ++i) {
        os << "Player: " << (*i).getId() << ", Group: " << (*i).getGroupId() << ", Score: " << (*i).getScore() << ". ";
    }
    os << std::endl;
    return os;
}

LinkedList::~LinkedList() {
    Node* next = head;
    while (next != nullptr) {
        Node* temp = next->next;
        delete next;
        next = temp;
    }
}
