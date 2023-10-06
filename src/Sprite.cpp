#include "Sprite.h"

using namespace Engine;
using AnimationSet = std::map<std::string, FrameAnimation>;

namespace GameContent
{
    Sprite::Sprite(int x, int y, Ref<AnimationSet> animations)
        : _Animations(animations)
    {
        Position.x = x;
        Position.y = y;
    }

    void Sprite::Render(float delta, Ref<Spritebatch> batcher) const
    {
        if(!_CurrentAnimation) return;

        // assuming begin was called
        batcher->Draw(_CurrentAnimation->GetTexture().get(), Position.x, Position.y, *_CurrentAnimation->currentFramePtr, Rotation);

    }

    void Sprite::Update(float delta) const
    {
        if(_CurrentAnimation)
        {
            _CurrentAnimation->Update(delta);
        }        
    }

    FrameAnimation* Sprite::SetAnimation(std::string animationName)
    {
        auto it = _Animations->find(animationName);

        if (it != _Animations->end())
        {
            // animation exists, return that
            _ENGINE_LOG("Sprite", "Animation key found! " << animationName)

            _CurrentAnimation = &(it->second);
            return _CurrentAnimation;
        } else
        {
            _ENGINE_LOG("Sprite", "Animation key not found")
            return nullptr;
        }
    }

}