#ifndef COLLIDERCOMPONENT_H_INCLUDED
#define COLLIDERCOMPONENT_H_INCLUDED

#include <string>
#include "ECS.h"
#include "Component.h"
#include <SDL.h>

class ColliderComponent : public Component
{
public:
    SDL_Rect collider;
    std::string tag;
    TransformComponent* Transform;

    ColliderComponent();

    ColliderComponent(std::string t)
    {
        tag = t;
    }
    ColliderComponent(std::string t, int xpos, int ypos, int Size)
    {
        tag = t;
        collider = {xpos, ypos, Size, Size};
    }

    void init() override
    {
        if(!entity->hasComponent<TransformComponent>())
            entity->addComponent<TransformComponent>();

        Transform = &entity->getComponent<TransformComponent>();
    }

    void update() override
    {
        collider.x = static_cast<int>(Transform->position.x);
        collider.y = static_cast<int>(Transform->position.y);
        collider.w = Transform->width * Transform->scale;
        collider.h = Transform->height * Transform->scale;
        // DEBUG
        // std::cout << "[Collider] " << tag << ": " << collider.x << "," << collider.y << " (" << collider.w << "x" << collider.h << ")\n";
    }
    void draw() override {
        SDL_Renderer* renderer = entity->getManager().getRenderer();
        if (!renderer) return;

        // Màu debug: xanh cho Player, đỏ cho block
        if (tag == "Player") {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // xanh lá
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // đỏ
        }

        SDL_RenderDrawRect(renderer, &collider);
    }
};

#endif // COLLIDERCOMPONENT_H_INCLUDED
