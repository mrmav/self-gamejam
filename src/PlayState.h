#pragma once

#include <Engine.h>

#include "SkiGame.h"
#include "MapLoader.h"
#include "Sprite.h"
#include "Player.h"
#include "GameCamera.h"
#include "PlayState.h"

using namespace Engine;
using json = nlohmann::json;
using AnimationSet = std::map<std::string, FrameAnimation>;

namespace GameContent
{

    class PlayState : public GameState
    {
        private:

        public:

        PlayState(void* game);
        ~PlayState();

        void Init() override;
        void Update(float delta) override;
        void Render(float delta) override;
        void Shutdown() override;

        Player* CheckWinningCondition(MapLoader& world, Player& pone, Player& ptwo);
        void DrawPlayerHealth(Ref<Spritebatch> batcher, Player const& player, glm::vec2 start, int dir) const;
        void DrawPlayerWorld(Ref<Spritebatch> batcher, Player& p, float delta);
        
        void Reset();

        Player nullPlayer = Player();
        Player drawPlayer = Player();

        Ref<GameCamera> camera;        
        Ref<Spritebatch> batcher;
        Ref<BitmapFont> font;
        Ref<Shader> shader;

        Ref<Texture2D> tilemap;
        Ref<FrameAnimation> animation;

        MapLoader map;
        Ref<TileSet> tileset;

        Sprite badguy;
        std::vector<Rectangle<int>> badGuyIdleFrames = 
        {
            Rectangle<int>(102, 102, 16, 16),
            Rectangle<int>(119, 102, 16, 16)
        };
        std::vector<Rectangle<int>> badGuyAttackFrames = 
        {
            Rectangle<int>(136, 102, 16, 16)
        };

        std::vector<Rectangle<int>> playerOneSkiFrames = 
        {
            Rectangle<int>(170, 102, 16, 16),
            Rectangle<int>(187, 102, 16, 16)
        };

        std::vector<Rectangle<int>> playerTwoSkiFrames = 
        {
            Rectangle<int>(170, 85, 16, 16),
            Rectangle<int>(187, 85, 16, 16)
        };

        AnimationSet badguyAnimationsMap;
        AnimationSet playerOneAnimationsMap;
        AnimationSet playerTwoAnimationsMap;

        Player playerOne;
        Player playerTwo;
        Player* winningPlayer = &nullPlayer;

    };
}