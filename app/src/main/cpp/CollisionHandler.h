// CollisionHandler.h
#ifndef COLLISION_H
#define COLLISION_H

#include "raymob.h"
#include "Eggcken.h"

class CollisionHandler 
{
public:
    static bool CheckCollision(const Eggcken& eggcken);
};

#endif // COLLISION_H