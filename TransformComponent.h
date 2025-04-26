#ifndef TRANSFORMCOMPONENT_H_INCLUDED
#define TRANSFORMCOMPONENT_H_INCLUDED

#include "Component.h"
#include "Vector2D.h"
#include "defs.h"

class TransformComponent : public Component
{


public:

    Vector2D position;
    Vector2D velocity;

    int height = 32;
    int width = 32;
    int scale = 1;

    int speed = 2;
    int cameraSpeed = 2;

    TransformComponent() {
        position.x = 0.0f;
        position.y = 0.0f;
    }

    TransformComponent(float x ,float y) {
        position.x = x;
        position.y = y;
    }
    TransformComponent(float x ,float y,int h, int w, int sc) {
        position.x = x;
        position.y = y;
        width = w;
        height = h;
        scale = sc;
    }

    void setVelocity(float x, float y) {
        velocity.x = x;
        velocity.y = y;
    }

    void init() override
    {
        velocity.x = 0;
        velocity.y = 0;
    }
    void update() override
    {
        position.x += velocity.x * speed ;
        position.y += velocity.y * speed ;
    }


};

#endif // TRANSFORMCOMPONENT_H_INCLUDED
