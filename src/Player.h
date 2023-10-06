#pragma once

#include <Engine.h>
#include "Sprite.h"
#include "InputCursor.h"

namespace GameContent
{
    class Player : public Sprite
    {

        private:

            Ref<InputCursor> _Controller;

            float _SideSpeed = 50.0f;
            float _SlideSpeed = 30.0f;

        public:

            Player() { };
            Player(int x, int y, Ref<AnimationSet> animations, Ref<InputCursor> controller);
            ~Player();

            void Move(glm::vec2 distance)
            {
                Position += distance;
            }

            void Update(float delta);
    };
}