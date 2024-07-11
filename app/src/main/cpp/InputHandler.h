// InputHandler.h
#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "raymob.h"

class InputHandler {
public:
    InputHandler() : touchPressed(false), touchPosition({0, 0}) {}
    ~InputHandler() = default;

    void Update();
    bool IsTouchPressed() const { return touchPressed; }
    Vector2 GetTouchPosition() const { return touchPosition; }

private:
    bool touchPressed;
    Vector2 touchPosition;
};

#endif // INPUT_HANDLER_H