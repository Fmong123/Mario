#ifndef SPRITECOMPONENT_H_INCLUDED
#define SPRITECOMPONENT_H_INCLUDED

#include "Component.h"
#include <SDL.h>
#include "TextureManage.h"

enum class FacingDirection {
    Left,
    Right
};

class SpriteComponent : public Component {
public:
    FacingDirection facing = FacingDirection::Right;
    TransformComponent *Transform;
    SDL_Texture* texture;
    SDL_Rect srcRect;       // Rect nguồn trong sprite sheet (thường dùng để cắt hình ảnh từ sprite sheet)
    SDL_Rect destRect; // Rect đích (vị trí và kích thước của Mario trên màn hình)
    SDL_Renderer* ren;

    SpriteComponent() = default;
    SpriteComponent(const char* path, SDL_Renderer* renderer)
    {
        ren = renderer;
        setTex (path, ren);
    }

    ~SpriteComponent()
    {
        SDL_DestroyTexture(texture);
    }

    void setTex (const char* path, SDL_Renderer* renderer)
    {
        texture = TextureManager::Load(path, ren);
    }
    void init() override {
        Transform = &entity->getComponent<TransformComponent>();
        int texW = 0, texH = 0;
        SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);

        srcRect = {0, 0, texW, texH};

    }

    void update() override {
        Transform = &entity->getComponent<TransformComponent>();
        destRect.x = static_cast<int>(Transform->position.x);
        destRect.y = static_cast<int>(Transform->position.y);
        destRect.w = Transform->width * Transform->scale;
        destRect.h = Transform->height * Transform->scale;

        // Cập nhật hướng dựa trên vận tốc (ví dụ đơn giản)
        if (Transform->velocity.x > 0) {
            facing = FacingDirection::Right;
        } else if (Transform->velocity.x < 0) {
            facing = FacingDirection::Left;
        }
    }
    void draw() override {
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (facing == FacingDirection::Left) {
            flip = SDL_FLIP_HORIZONTAL;
        }
        TextureManager::Draw(texture, ren, srcRect, destRect, 0.0, nullptr, flip);
    }

};

#endif // SPRITECOMPONENT_H_INCLUDED
