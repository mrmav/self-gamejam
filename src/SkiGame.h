#pragma once
#include <Engine.h>
#include <iostream>
#include "Player.h"
#include "GameCamera.h"

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

        static Player nullPlayer;
        static Player drawPlayer;
        Player* CheckWinningCondition(MapLoader& world, Player& pone, Player& ptwo);
        void DrawPlayerHealth(Ref<Spritebatch> batcher, Player const& player, glm::vec2 start, int dir) const;
        void DrawPlayerWorld(Ref<Spritebatch> batcher, Player& p, float delta);

    };
}