#pragma once
#include <Engine.h>
#include <iostream>
#include "Player.h"
#include "GameCamera.h"

namespace GameContent
{
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


        void Shutdown() override;

        // these controllers will be refactored so the player can
        // change from a list which they want to use.
        Ref<InputCursor> playerOneController;
        Ref<InputCursor> playerTwoController;

    };
}