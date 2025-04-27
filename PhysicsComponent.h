#ifndef PHYSICSCOMPONENT_H_INCLUDED
#define PHYSICSCOMPONENT_H_INCLUDED

#include "Component.h"
#include "ECS.h"

class KeyboardController;
class PhysicsComponent : public Component {
public:

    float gravity = 0.1f;
    float maxFallSpeed = 1.5f;
    float jumpSpeed = 2.0f;

    bool isGrounded = false;
    bool jump = false;
    bool isPlayer = false;
    void init() override {
        Transform = &entity->getComponent<TransformComponent>();
        collider = &entity->getComponent<ColliderComponent>();
        if (entity->hasComponent<KeyboardController>()) { // Kiểm tra nếu có KeyboardController (chỉ Player)
            isPlayer = true;
        }
    }

    void update() override {
        auto& entities = entity->getManager().getGroup(groupColliders);

        int colliderWidth = Transform->width * Transform->scale;
        int colliderHeight = Transform->height * Transform->scale;

        // --- Kiểm tra đứng trên block ---
        isGrounded = false;
        SDL_Rect footCheck = {
            static_cast<int>(Transform->position.x),
            static_cast<int>(Transform->position.y + colliderHeight + 1), // điểm dưới chân
            colliderWidth,
            1
        };

        for (auto& e : entities) {
            if (!e->hasComponent<ColliderComponent>()) continue;
            auto& other = e->getComponent<ColliderComponent>();

            if (Collision::AABB(footCheck, other.collider)) {
                isGrounded = true;
                break;
            }
        }

        // --- Xử lý nhảy và trọng lực ---

        if (!isGrounded) {
            Transform->velocity.y += gravity;
            if (Transform->velocity.y > maxFallSpeed) {
                Transform->velocity.y = maxFallSpeed;
            }
        }

        // --- Va chạm trục X ---
        if (Transform->velocity.x != 0) {
            Transform->position.x += Transform->velocity.x * Transform->speed;
            collider->update();

            for (auto& e : entities) {
                if (!e->hasComponent<ColliderComponent>()) continue;
                auto& other = e->getComponent<ColliderComponent>();

                if (Collision::AABB(*collider, other)) {
                    if (Transform->velocity.x > 0) {
                        Transform->position.x = other.collider.x - colliderWidth;
                    } else if (Transform->velocity.x < 0) {
                        Transform->position.x = other.collider.x + other.collider.w;
                    }
                    Transform->velocity.x = 0;
                    collider->update();
                }
            }
        }

        // --- Va chạm trục Y ---
        Transform->position.y += Transform->velocity.y * Transform->speed;
        collider->update();

        for (auto& e : entities) {
            if (!e->hasComponent<ColliderComponent>()) continue;
            auto& other = e->getComponent<ColliderComponent>();

            if (Collision::AABB(*collider, other)) {
                //if (Collision::AABB(*collider, other)) {
                    //std::cout << "[Va cham] Mario va cham block tai (" << other.collider.x << "," << other.collider.y << ")\n";
                //}
                if (Transform->velocity.y > 0) {
                    Transform->position.y = other.collider.y - colliderHeight;
                } else if (Transform->velocity.y < 0) {
                    Transform->position.y = other.collider.y + other.collider.h;
                }
                Transform->velocity.y = 0;
                collider->update();
            }
        }


    }
    void setJump(bool value) {
        jump = value;
    }

private:
    TransformComponent* Transform;
    ColliderComponent* collider;
};

#endif // PHYSICSCOMPONENT_H_INCLUDED
