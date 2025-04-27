#ifndef ENEMYAICOMPONENT_H_INCLUDED
#define ENEMYAICOMPONENT_H_INCLUDED

#include "Component.h"
#include "ECS.h"

#include "Component.h"
#include "ECS.h"
#include "defs.h"

class EnemyAIComponent : public Component {
public:
    float moveSpeed;
    float direction; // 1 cho phải, -1 cho trái

    EnemyAIComponent(float speed) : moveSpeed(speed), direction(1.0f) {}

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override {
        if (transform) {
            // Kiểm tra nếu enemy ra ngoài màn hình và destroy **TRƯỚC KHI** di chuyển
            if (transform->position.x < -64 || transform->position.x > SCREEN_WIDTH + 64 || transform->position.y > SCREEN_HEIGHT + 64) {
                entity->destroy();
                return; // Thoát khỏi hàm update
            }

            transform->position.x += moveSpeed * direction * transform->speed;
            if (direction < 0 && transform->position.x < -32) {
                entity->destroy();
                return; // Thoát khỏi hàm update
            }
            if (transform->position.x < -32 && direction < 0) {
                direction = 1.0f;
            } else if (transform->position.x > SCREEN_WIDTH + 32 && direction > 0) {
                direction = -1.0f;
            }

        }
    }

private:
    TransformComponent* transform;
};
#endif // ENEMYAICOMPONENT_H_INCLUDED
