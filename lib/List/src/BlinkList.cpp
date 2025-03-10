#include "BlinkList.h"
#include "stdlib.h"  // For malloc, realloc, free
#include <assert.h>  // For assert()

// Constructor: Save the base timer interval
BlinkArray::BlinkArray() : items(nullptr), capacity(4), count(0) {
    items = (BlinkItem *)malloc(capacity * sizeof(BlinkItem));
    if (!items) {
        // Handle memory allocation failure
        while (1); // Halt or handle error
    }
}

// Destructor
BlinkArray::~BlinkArray() {
    if (items) {
        free((void *)items); // Free the allocated memory
    }
}

// Add an item to the array
void BlinkArray::add(int pin, int delay, int times_left) volatile {
    // Ensure that the delay is a multiple of baseInterval.
    
    if (count >= capacity) {
        resize(); // Resize the array if it's full
    }
    
    // Add the new item.
    // (Note: times_left is multiplied by 2 as per your original logic.)
    items[count].pin = pin;
    items[count].delay = delay;
    items[count].times_left = 2 * times_left;
    count++;
}

// Get the number of items in the array
size_t BlinkArray::size() const volatile {
    return count;
}

// Get an item from the array
void BlinkArray::get(size_t index, volatile int &pin, volatile int &delay, volatile int &times_left) const volatile {
    if (index >= count) {
        // Handle out-of-bounds access
        while (1); // Halt or handle error
    }
    pin = items[index].pin;
    delay = items[index].delay;
    times_left = items[index].times_left;
}

// Set/update an item in the array (parameters passed by value)
void BlinkArray::set(size_t index, volatile int pin, volatile int delay, volatile int times_left) volatile {
    if (index >= count) {
        while (1); // Halt or handle error
    }
    items[index].pin = pin;
    items[index].delay = delay;
    items[index].times_left = times_left;
}

// Remove an item from the array by shifting later items left.
void BlinkArray::remove(size_t index) volatile {
    if (index >= count) {
        return;
    }
    for (size_t j = index; j < count - 1; j++) {
        items[j] = items[j + 1];
    }
    count--;
}

// Resize the array
void BlinkArray::resize() volatile {
    capacity *= 2; // Double the capacity.
    items = (BlinkItem *)realloc((void *)items, capacity * sizeof(BlinkItem));
    if (!items) {
        while (1); // Halt or handle error.
    }
}
