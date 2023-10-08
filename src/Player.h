#pragma once

#include <Engine.h>
#include "Sprite.h"
#include "InputCursor.h"
#include "MapLoader.h"

namespace GameContent
{
    struct KinematicMaterial
    {
        const float SideMoveForce;
        const float SlideForce;
        const float Friction;
        const float ChangingSpeedBreakDamping;

        const float MaxSideMoveSpeed;
        const float MaxSlideSpeed;        

        KinematicMaterial(float sideForce, float slideForce, float friction, float changeSpeedDamp, float maxSideMoveSpeed, float maxSlideSpeed)
            : SideMoveForce(sideForce), SlideForce(slideForce), Friction(friction), ChangingSpeedBreakDamping(changeSpeedDamp),
            MaxSideMoveSpeed(maxSideMoveSpeed), MaxSlideSpeed(maxSlideSpeed)
        {

        }

        void CalculateContribution(float delta, int direction, glm::vec2& acceleration, glm::vec2 const& velocity)
        {   
            acceleration.x += direction * SideMoveForce;
            acceleration.y += SlideForce;

            acceleration *= delta;
        }

        void CapVelocity(glm::vec2& velocity)
        {

            // cap
            if(glm::abs(velocity.x) > MaxSideMoveSpeed)
            {   
                int sign = velocity.x > 0 ? 1 : -1;
                velocity.x = MaxSideMoveSpeed * sign;
            }
            if(glm::abs(velocity.y) > MaxSlideSpeed)
            {
                int sign = velocity.y > 0 ? 1 : -1;
                velocity.y = MaxSlideSpeed * sign;
            }
        }

    };

    class Player : public Sprite
    {

        private:

            Ref<InputCursor> _Controller;
            
            const MapLoader* _LoadedMap;
            
            KinematicMaterial* _Kinematics;
            glm::vec2 _Acceleration = glm::vec2(0.0f, 0.0f);
            glm::vec2 _Velocity     = glm::vec2(0.0f, 0.0f);

        public:

            Player() { };
            Player(int x, int y, Ref<AnimationSet> animations, Ref<InputCursor> controller, MapLoader* map);
            ~Player();

            void Move(glm::vec2 distance)
            {
                Position += distance;
            }

            std::string Name;
            glm::vec4 Color;

            void Update(float delta);
    };

}