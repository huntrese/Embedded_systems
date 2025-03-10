#pragma once
#include "stdlib.h"

class BlinkArray {
    public:
        // Constructor now takes the base timer interval (in ms)
        BlinkArray();
    
        // Destructor
        ~BlinkArray();
    
        // Add an item to the array.
        // The delay must be a multiple of the base timer interval.
        void add(int pin, int delay, int times_left) volatile;
    
        // Get the number of items in the array.
        size_t size() const volatile;
    
        // Get an item from the array.
        void get(size_t index, volatile int &pin, volatile int &delay, volatile int &times_left) const volatile;
    
        // Set/update an item in the array.
        // Parameters are now passed by value.
        void set(size_t index, volatile int pin, volatile int delay, volatile int times_left) volatile;
    
        // Remove an item from the array by shifting later items left.
        void remove(size_t index) volatile;
    
    private:
        // Internal data structure.
        struct BlinkItem {
            volatile int pin;
            volatile int delay;
            volatile int times_left;
        };
    
        // Dynamic array and its properties.
        BlinkItem *items;         // Pointer to the array of items.
        volatile size_t capacity; // Current capacity of the array.
        volatile size_t count;    // Number of items in the array.
    
        
    
        // Resize the array when needed.
        void resize() volatile;
};
