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


    /*bool onGround;
    bool jump= false;
    float gravity = 0.08f;
    float jumpSpeed = 1.5f;
    float maxFallSpeed = 4.5f;*/

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




    }

    /*void update() override {
        // Di chuyển trục X
        position.x += velocity.x * speed;

        // Di chuyển trục Y
        position.y += velocity.y * speed;

        // Hiệu ứng trọng lực khi Mario không ở trên mặt đất
        /*velocity.y += gravity;

        if (onGround) {
            velocity.y = 0; // Ngừng tác động của gravity
        }

        if (velocity.y > maxFallSpeed) {
            velocity.y = maxFallSpeed; // Giới hạn tốc độ rơi
        }

        // Kiểm tra nhảy
        if (jump) {
            if (onGround) {
                velocity.y = -jumpSpeed; // chỉ nhảy khi đang trên đất
                onGround = false;
            }
            jump = false; // reset flag nhảy sau khi xử lý
        }

        // Giới hạn tốc độ nhảy
        if (velocity.y < -jumpSpeed) {
            velocity.y = -jumpSpeed;
        }

        jump = false; // reset cờ jump
    }*/



};

#endif // TRANSFORMCOMPONENT_H_INCLUDED
