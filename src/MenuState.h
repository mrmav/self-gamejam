#pragma once

#include <Engine.h>
#include "SkiGame.h"
#include "DebugCamera.h"

using namespace Engine;

namespace GameContent
{

    class MenuState : public GameState
    {
        private:

        public:

        MenuState(void* game);
        ~MenuState();

        void Init() override;
        void Update(float delta) override;
        void Render(float delta) override;
        void Shutdown() override;

        Ref<DebugCamera> camera;        
        Ref<Spritebatch> batcher;
        Ref<BitmapFont> font;
        Ref<Shader> shader;

    };

}