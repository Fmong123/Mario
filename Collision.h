#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED

#include "defs.h"
#include "Map.h"
#include "ECS.h"
#include "Component.h"

#include <SDL.h>

class ColliderComponent;

class Collision
{
public:
    static bool AABB(const SDL_Rect& a, const SDL_Rect& b) {
        return (
            a.x + a.w > b.x &&
            a.x < b.x + b.w &&
            a.y + a.h > b.y &&
            a.y < b.y + b.h
        );
    }

    static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB) {
        return AABB(colA.collider, colB.collider);
    }


};

#endif // COLLISION_H_INCLUDED
