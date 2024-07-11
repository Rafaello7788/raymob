// CollisionHandler.cpp
#include "raymob.h"
#include "CollisionHandler.h"
#include "Eggcken.h"
#include "raymath.h"

bool CollisionHandler::CheckCollision(const Eggcken& eggcken) {
    auto gallusPositions = eggcken.GetGallusDomesticusPositions();
    auto gallusRadii = eggcken.GetGallusDomesticusRadii();
    Vector2 interactGallusPos = eggcken.GetInteractGallusDomesticusPosition();
    float interactGallusRadius = eggcken.GetInteractGallusDomesticusRadius();

    for (size_t i = 0; i < gallusPositions.size(); ++i) {
        if (CheckCollisionCircles(
            gallusPositions[i], gallusRadii[i],
            interactGallusPos, interactGallusRadius)) {
            return true;
        }
    }
    return false;
}