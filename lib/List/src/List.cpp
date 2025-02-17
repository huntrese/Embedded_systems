#include <stdlib.h>
#include "List.h"



LinkedList::~LinkedList() {
    Node* currentNode = head;
    while (currentNode != nullptr) {
        Node* nextNode = currentNode->next;
        free(currentNode);  
        currentNode = nextNode;
    }
}

void LinkedList::addPin(int pin) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->pin = pin;
    newNode->next = nullptr;

    if (head == nullptr) {
        head = newNode; 
    } else {
        Node* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;  
        }
        temp->next = newNode;  
    }
}

int LinkedList::isEmpty() {
    return head == nullptr;
}

int LinkedList::next() {
    if (current == nullptr) {
        current = head;  
    }
    if (current != nullptr) {
        int pin = current->pin;
        current = current->next;  
        return pin;
    }
    return -1;  
}

