// main.cpp

#include <iostream>
#include "raymob.h"
#include "Eggcken.h"
#include "InputHandler.h"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Eggcken Game");
    SetTargetFPS(60);

    Eggcken eggcken(4, 1);  // Start with 4 chickens and 1 gallus
    InputHandler inputHandler;

    while (!WindowShouldClose()) {
        inputHandler.Update();
        eggcken.Update(inputHandler);

        BeginDrawing();
        ClearBackground(BLACK);
        eggcken.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}