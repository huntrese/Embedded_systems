#pragma once

class LinkedList {
public:
    // Constructor using variadic template
    template <typename... Args>
    LinkedList(Args... pins);

    ~LinkedList();
    int isEmpty();
    int next();

    // Template method definition
    template <typename Func, typename... Args>
    void forEach(Func func, Args... args) {
        Node* currentNode = head;
        while (currentNode != nullptr) {
            func(currentNode->pin, args...);  // Call the function with the pin and additional arguments
            currentNode = currentNode->next;
        }
    }

private:
    struct Node {
        int pin;
        Node* next;
    };

    Node* head;
    Node* current;

    void addPin(int pin);
};

// Constructor implementation
template <typename... Args>
LinkedList::LinkedList(Args... pins) : head(nullptr), current(nullptr) {
    (addPin(pins), ...);
}