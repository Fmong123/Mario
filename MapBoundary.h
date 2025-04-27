#ifndef MAPBOUNDARY_H_INCLUDED
#define MAPBOUNDARY_H_INCLUDED

#include "Component.h"
#include "ECS.h"
#include "defs.h"

class MapBoundary : public Component {
public:
    void init() override {
        Transform = &entity->getComponent<TransformComponent>();
        collider = &entity->getComponent<ColliderComponent>();
    }

    void update() override {
        int screenW = SCREEN_WIDTH;
        int screenH = SCREEN_HEIGHT;
        int colliderWidth = Transform->width * Transform->scale;
        int colliderHeight = Transform->height * Transform->scale;

        if (Transform->position.x < 0) {
            Transform->position.x = 0;
            Transform->velocity.x = 0;
        }
        if (Transform->position.x + colliderWidth > screenW) {
            Transform->position.x = screenW - colliderWidth;
            Transform->velocity.x = 0;
        }

        if (Transform->position.y < 0) {
            Transform->position.y = 0;
            Transform->velocity.y = 0;
        }
        if (Transform->position.y + colliderHeight > screenH) {
            Transform->position.y = screenH - colliderHeight;
            Transform->velocity.y = 0;
        }
    }

private:
    TransformComponent* Transform;
    ColliderComponent* collider;
};

#endif // MAPBOUNDARY_H_INCLUDED
