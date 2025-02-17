#pragma once

class Clickable {
public:
    typedef void (*ClickHandler)();
    
    Clickable(ClickHandler handler);
    
    ClickHandler onClick;
};