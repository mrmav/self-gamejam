#pragma once

#include <Engine.h>

using namespace Engine;

namespace GameContent
{

    class MenuState : public GameState
    {
        private:

        public:

        MenuState(Game* game);
        ~MenuState();

        void Init() override;
        void Update(float delta) override;
        void Render(float delta) override;
        void Shutdown() override;
    };

}