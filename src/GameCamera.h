#pragma once

#include <Engine.h>
#include "MapLoader.h"
#include "Player.h"

using namespace Engine;


namespace GameContent
{

    class GameCamera : public Camera2D
    {
        public:

        GameCamera(Viewport& viewport, MapLoader& map);
        ~GameCamera();

        void Update(float delta, Player& pone, Player& ptwo);
        void UpdateInGameCamera(float delta, Player& pone, Player& ptwo);
        Rectangle<float> GetPlayersBoundingBox(Player& pone, Player& ptwo);
        glm::vec2 GetCenterPoint(glm::vec2& p1, glm::vec2& p2) const;

        private:

        MapLoader& _Map;
    };

} // namespace GameContent


