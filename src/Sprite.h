#pragma once

#include <Engine.h>
#include <map>

using namespace Engine;

using AnimationSet = std::map<std::string, FrameAnimation>;

namespace GameContent
{
    class Sprite
    {
    protected:

        FrameAnimation* _CurrentAnimation = nullptr;
        Ref<AnimationSet> _Animations;

    public:

        glm::vec2 Position;
        float Rotation = 0;

        Sprite() { };
        Sprite(int x, int y, Ref<AnimationSet> animations);
        ~Sprite() { };

        void virtual Update(float delta);
        void virtual Render(float delta, Ref<Spritebatch> batcher) const;

        FrameAnimation* SetAnimation(std::string animationName);

    };
    
}