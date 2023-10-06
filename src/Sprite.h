#include <Engine.h>
#include <map>

using namespace Engine;

using AnimationSet = std::map<std::string, FrameAnimation>;

namespace GameContent
{
    class Sprite
    {
    private:

        FrameAnimation* _CurrentAnimation;
        Ref<AnimationSet> _Animations;

    public:

        glm::vec2 Position;
        float Rotation = 0;

        Sprite() { };
        Sprite(int x, int y, Ref<AnimationSet> animations);
        ~Sprite() { };

        void Update(float delta) const;
        void Render(float delta, Ref<Spritebatch> batcher) const;

        FrameAnimation* SetAnimation(std::string animationName);

    };
    
}