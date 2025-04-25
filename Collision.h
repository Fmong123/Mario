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

    /*static void checkPlayerCollisions(Manager& manager) {
        auto& players = manager.getGroup(groupPlayers);
        auto& colliders = manager.getGroup(groupColliders);

        for (auto* player : players) {
            if (!player->hasComponent<ColliderComponent>()) continue;

            auto& playerCollider = player->getComponent<ColliderComponent>();
            auto& playerTransform = player->getComponent<TransformComponent>();
            auto& vel = playerTransform.velocity;
            SDL_Rect& pRect = playerCollider.collider;

            // Reset onGround mỗi frame
            playerTransform.onGround = false;

            // ===== Va chạm trục X =====
            playerTransform.position.x += vel.x;
            playerCollider.collider.x = static_cast<int>(playerTransform.position.x);

            for (auto* tile : colliders) {
                if (!tile->hasComponent<ColliderComponent>()) continue;

                auto& tileCollider = tile->getComponent<ColliderComponent>();
                if (tileCollider.tag != "solid") continue;

                if (AABB(playerCollider.collider, tileCollider.collider)) {
                    if (vel.x > 0) {
                        playerTransform.position.x = tileCollider.collider.x - pRect.w;
                    } else if (vel.x < 0) {
                        playerTransform.position.x = tileCollider.collider.x + tileCollider.collider.w;
                    }

                    vel.x = 0;
                    playerCollider.collider.x = static_cast<int>(playerTransform.position.x);
                }
            }

            // ===== Va chạm trục Y — chống xuyên bằng chia nhỏ bước =====
            float totalY = vel.y;
            const float stepSize = 0.5f;  // từng bước 0.5px
            int steps = std::ceil(std::abs(totalY) / stepSize);

            for (int i = 0; i < steps; ++i) {
                float dy = totalY / steps;
                playerTransform.position.y += dy;
                playerCollider.collider.y = static_cast<int>(playerTransform.position.y);

                bool collided = false;

                for (auto* tile : colliders) {
                    if (!tile->hasComponent<ColliderComponent>()) continue;
                    auto& tileCollider = tile->getComponent<ColliderComponent>();
                    if (tileCollider.tag != "solid") continue;

                    if (AABB(playerCollider.collider, tileCollider.collider)) {
                        collided = true;

                        if (dy > 0) { // rơi xuống
                            playerTransform.position.y = tileCollider.collider.y - pRect.h;
                            playerTransform.onGround = true;
                        } else { // nhảy lên
                            playerTransform.position.y = tileCollider.collider.y + tileCollider.collider.h;
                        }

                        vel.y = 0;
                        playerCollider.collider.y = static_cast<int>(playerTransform.position.y);
                        break;
                    }
                }

                if (collided) break;
            }

            // ===== Giới hạn trong map =====
            int mapWidth = MAP_WIDTH * TILE_SIZE * SCALE;
            int mapHeight = MAP_HEIGHT * TILE_SIZE * SCALE;

            if (playerTransform.position.x < 0) {
                playerTransform.position.x = 0;
                vel.x = 0;
            }
            if (playerTransform.position.x + pRect.w > mapWidth) {
                playerTransform.position.x = mapWidth - pRect.w;
                vel.x = 0;
            }

            if (playerTransform.position.y + pRect.h > mapHeight) {
                playerTransform.position.y = mapHeight - pRect.h;
                vel.y = 0;
            }

            // ===== Debug Mario =====
            std::cout << "[Mario] y=" << playerTransform.position.y
                      << " | velY=" << vel.y
                      << " | onGround=" << playerTransform.onGround << std::endl;
        }
    }*/


};

#endif // COLLISION_H_INCLUDED
