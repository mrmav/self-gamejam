#include <Engine.h>
#include <iostream>

namespace GameContent
{

    void UpdateDebugCamera(float delta, Ref<Engine::Camera2D> camera);

    class SkiGame : public Engine::Game
    {
        public:

        SkiGame(uint32_t screenWidth, uint32_t screenHeight, const char* windowTitle);

        ~SkiGame();

        void Run() override { Engine::Game::Run(); };

        /*
        * Use this function to load and create the game elements
        */
        void Load() override;

        /*
        * This function will be called once per frame.
        * Use this to update you game logic.
        * delta is the amount of time since the last frame
        */
        void Update(float delta) override;

        /*
        * Use this function to render your game
        * delta is the amount of time since the last frame
        */
        void Render(float delta) override;
    };
}