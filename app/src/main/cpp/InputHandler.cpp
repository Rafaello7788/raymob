// InputHandler.cpp

#include "InputHandler.h"
#include "raymob.h"

// Remove the constructor and destructor definitions if they're already in the header

void InputHandler::Update() {
    // Check for touch input
    if (IsGestureDetected(GESTURE_TAP)) {
        touchPressed = true;
        touchPosition = ::GetTouchPosition(0); // Use the global GetTouchPosition function from raylib
    } else {
        touchPressed = false;
    }
}
