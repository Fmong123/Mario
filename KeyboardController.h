#ifndef KEYBOARDCONTROLLER_H_INCLUDED
#define KEYBOARDCONTROLLER_H_INCLUDED

#include <SDL.h>
#include "defs.h"
#include "Component.h"
#include "ECS.h"


class PhysicsComponent;

class KeyboardController : public Component
{


public:


    bool moveLeft = false;
    bool moveRight = false;
    bool jump = false;
    bool run = false;
    bool jumpHeld = false;


    PhysicsComponent* physics = nullptr;
    TransformComponent *Transform;

    void init() override
    {
        Transform = &entity->getComponent<TransformComponent>();
        if (entity->hasComponent<PhysicsComponent>()) {
            physics = &entity->getComponent<PhysicsComponent>();
        }
    }
    void update(SDL_Event& event)
    {
        if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
            switch (event.key.keysym.sym) {
                case SDLK_a:
                    moveLeft = true;
                    break;
                case SDLK_d:
                    moveRight = true;
                    break;
                case SDLK_SPACE:
                    jumpHeld = true;
                    Transform->velocity.y += -0.5; // optional: dùng tạm nhưng cần thêm flag kiểm

                    break;
                case SDLK_LSHIFT:
                    run = true;
                    break;
            }
        }

        if (event.type == SDL_KEYUP && event.key.repeat == 0) {
            switch (event.key.keysym.sym) {
                case SDLK_a:
                    moveLeft = false;
                    break;
                case SDLK_d:
                    moveRight = false;
                    break;
                case SDLK_LSHIFT:
                    run = false;
                    break;
                case SDLK_SPACE:
                    jumpHeld = false;
                    if (physics && Transform->velocity.y < 0) {
                        Transform->velocity.y *= 0.5f;  // ngắt nhảy sớm
                    }
                    break;
            }
        }

        if (jumpHeld) {
            if (Transform->velocity.y > -2.5) {
                Transform->velocity.y -= 0.5f; // Gradual increase
            } else {
                jumpHeld = false; // Prevent further jumping
            }
        }



        // Cập nhật velocity.x
        if (Transform) {
            if (moveLeft) {
                Transform->velocity.x = run ? -1.5f : -1.0f;
            } else if (moveRight) {
                Transform->velocity.x = run ? 1.5f : 1.0f;
            } else {
                Transform->velocity.x = 0.0f;
            }
        }



        /*if (jump && Transform->onGround) {
            Transform->jump = true;
        }*/


        /*if (event.type == SDL_KEYUP && event.key.repeat == 0) {
            switch (event.key.keysym.sym) {
                case SDLK_a:
                case SDLK_d:
                    Transform->velocity.x = 0;
                    break;
            }

            //if (Transform->velocity.x == 0)
                //sprite->play("idle");
        }*/
    }


};


#endif // KEYBOARDCONTROLLER_H_INCLUDED
