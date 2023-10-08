#include <iostream>
#include <Engine.h>
#include <json.hpp>
#include <map>
#include <algorithm>

#include "SkiGame.h"
#include "MapLoader.h"
#include "Sprite.h"
#include "Player.h"
#include "GameCamera.h"

using namespace Engine;
using json = nlohmann::json;
using AnimationSet = std::map<std::string, FrameAnimation>;


namespace GameContent
{   
    Player SkiGame::nullPlayer = Player();
    Player SkiGame::drawPlayer = Player();

    Viewport gameViewport;
    Ref<GameCamera> camera;
    Ref<Spritebatch> batcher;
    Ref<Shader> shader;
    
    Ref<Texture2D> TILEMAP;
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
    Player* winningPlayer = &SkiGame::nullPlayer;

    Ref<BitmapFont> font;

    SkiGame::SkiGame(uint32_t screenWidth, uint32_t screenHeight, const char* windowTitle)
        : Engine::Game(screenWidth, screenHeight, windowTitle)
    {
        
    };

    SkiGame::~SkiGame() { };

    void SkiGame::Load()
    {
        // useRenderTarget = true;
        // renderTargetWidth  = 800 / 2;
        // renderTargetHeight = 600 / 2;
        // SetRenderTarget();
        
        map = MapLoader("assets/tiled/map.json");
        // json map_data = map.GetData();
        // std::cout << map_data["version"] << std::endl;


        // init the game viewport to default to window size
        gameViewport = Viewport(GetViewport().Width(), GetViewport().Height());

        //camera = std::make_shared<Camera2D>(gameViewport);
        camera = std::make_shared<GameCamera>(GetViewport(), map);
        camera->Zoom = 1;

        batcher = std::make_shared<Spritebatch>();
        shader = std::make_shared<Shader>("assets/shaders/vertex.vert", "assets/shaders/fragment.frag");

        TILEMAP = std::make_shared<Texture2D>("assets/kenney_tiny-ski/Tilemap/tilemap.png", TextureParams());
        tileset = std::make_shared<TileSet>(TILEMAP, 12, 0, 16, 16, 0, 1);

        font = std::make_shared<BitmapFont>("assets/font.png", 16, 16);

        // animation = std::make_shared<FrameAnimation>(TILEMAP, frames.data(), 3);
        // animation->loop = true;
        // animation->SetFps(6);

        // test sprites
        badguyAnimationsMap = {
            {"idle",   FrameAnimation(TILEMAP, badGuyIdleFrames.data(),   2)},
            {"attack", FrameAnimation(TILEMAP, badGuyAttackFrames.data(), 1)}
        };
        badguy = Sprite(8 * 16, 7 * 16, std::make_shared<AnimationSet>(badguyAnimationsMap));

        // test sprites
        playerOneAnimationsMap = {
            {"ski", FrameAnimation(TILEMAP, playerOneSkiFrames.data(), 2)}
        };
        playerTwoAnimationsMap = {
            {"ski", FrameAnimation(TILEMAP, playerTwoSkiFrames.data(), 2)}
        };
        
        // for a keyboard controller:
        Ref<InputCursor> playerTwoController = std::make_shared<InputCursor>(Key::Left, Key::Right, Key::Space);

        // for a gamepad controller:
        Ref<InputCursor> playerOneController = std::make_shared<InputCursor>(0, GamePadButton::DPadLeft, GamePadButton::DPadRight, GamePadButton::ButtonA);
        
        // for a gamepad controller with axis (sticks):
        //Ref<InputCursor> playerOneController = std::make_shared<InputCursor>(0, true, GamePadAxis::GamePadAxisLeftX, GamePadButton::ButtonA);
        

        playerOne = Player(14 * 16, 2 * 16, std::make_shared<AnimationSet>(playerOneAnimationsMap), playerOneController, &map);
        playerOne.SetAnimation("ski")->Play();
        playerOne.Name = "1";
        playerOne.Color = glm::vec4(0.819608f, 0.462745f, 0.815686, 1);        
        

        playerTwo = Player(16 * 16, 2 * 16, std::make_shared<AnimationSet>(playerTwoAnimationsMap), playerTwoController, &map);
        playerTwo.SetAnimation("ski")->Play();
        playerTwo.Name = "2";
        playerTwo.Color = glm::vec4(0.517647f, 0.776471f, 0.411765, 1);

        Input::SetDeadZone(0, 0.2f);

    }

    void SkiGame::Update(float delta)
    {   


        if(Input::IsKeyJustDown(Key::N))
        {
            std::cout << "Btn Idle" << std::endl;
            badguy.SetAnimation("idle")->Play();
        }
        if(Input::IsKeyJustDown(Key::M))
        {
            std::cout << "Btn Attack" << std::endl;
            badguy.SetAnimation("attack")->Play();
        }

        badguy.Update(delta);
        playerOne.Update(delta);
        playerTwo.Update(delta);

        camera->Update(delta, playerOne, playerTwo);


        winningPlayer = CheckWinningCondition(map, playerOne, playerTwo);
        
    };


    void SkiGame::Render(float delta)
    {

        glClearColor(0.392, 0.584, 0.929, 1);  // good ol' cornflower blue
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        // lets try to render the map
        json map_data = map.GetData();
        int mapWidth  = map_data["width"];
        int mapHeight = map_data["height"];        
        json layers_data = map_data.at("layers");

        // We area assuming that there is only one tileset.
        // Assuming this, we render each layer in a separate pass:        
        uint16_t depth = 0;
        for(auto& layer : layers_data)
        {
            
            if(layer["type"] != std::string("tilelayer")) continue;
            //if(layer["name"] == std::string("Terrain")) continue;

            int currentTileIndex = 0;

            batcher->Begin(shader.get(), camera.get(), glm::vec4(1), depth);

            std::string str = layer.at("name");
            bool is_p = str == std::string("PowerUps");
            //std::cout << "Drawing layer " << layer["name"] << ", at depth: " << std::to_string(depth) << std::endl;

            for(int y = 0; y < mapHeight; y++)
            {
                for(int x = 0; x < mapWidth; x++)
                {   
                    int tileid = layer["data"][currentTileIndex];
                    currentTileIndex++;

                    if(tileid == 0) continue;

                    tileset->DrawTile(batcher, glm::vec2(x * 16, y * 16), --tileid); // tiled assumes that a ZERO tile id, is a non existing tile.
                                                                                     // but in our system, we treat the ZERO id as the first one.
                                                                                     // To fix this, we subtract ONE before drawing.
                }
            }
            batcher->End();
            depth += 1;
            
        }

        DrawPlayerWorld(batcher, playerOne, delta);
        DrawPlayerWorld(batcher, playerTwo, delta);

        // Render UI stuff with the custom transform
        float UIScale = 4.0f;
        batcher->SetCustomView(glm::mat4(1) * glm::scale(glm::vec3(UIScale)));

        // render player health
        batcher->Begin(shader.get(), camera.get(), glm::vec4(1), 0, true);
        DrawPlayerHealth(batcher, playerOne, glm::vec2(8, 8), 1);
        DrawPlayerHealth(batcher, playerTwo, glm::vec2(GetViewport().Width() / UIScale - 9 - 8, 8), -1);
        batcher->End();


        // Render winning string:
        if(winningPlayer != &nullPlayer)
        {
            batcher->Begin(shader.get(), camera.get(), winningPlayer->Color, 0, true);

            std::string str = std::string("Player ") + winningPlayer->Name;
            glm::vec2 strSize = font->GetStringSize(str);
            glm::vec2 UIDisplaySize = glm::vec2(GetViewport().Width(), GetViewport().Height()) / UIScale;
            
            batcher->DrawString(font.get(), UIDisplaySize.x / 2.0f - strSize.x / 2.0f, UIDisplaySize.y / 2.0f - strSize.y / 2.0f, str.c_str());
            
            str = "Wins!";
            strSize = font->GetStringSize(str);
            batcher->DrawString(font.get(), UIDisplaySize.x / 2.0f - strSize.x / 2.0f, UIDisplaySize.y / 2.0f - strSize.y / 2.0f + 16.0f, str.c_str());

            batcher->End();
        }


    };

    void SkiGame::DrawPlayerHealth(Ref<Spritebatch> batcher, Player const& player, glm::vec2 start, int dir) const
    {      
        float offset = 1;

        for(int i = 0; i < player.GetHealth(); i++)
        {   
            float x = start.x + ((9 + offset) * i) * dir;
            batcher->Draw(TILEMAP.get(), x, start.y, Rectangle<int>(136, 53, 9, 9));            

        }
    }

    Player* SkiGame::CheckWinningCondition(MapLoader& world, Player& pone, Player& ptwo)
    {
        float maxY = glm::max(pone.Position.y, ptwo.Position.y);
        
        Player* result = &nullPlayer;

        // check win by player death;
        if(pone.GetHealth() == 0)
        {
            result = &ptwo;
        }
        if(ptwo.GetHealth() == 0)
        {
            if(result == &ptwo) return &drawPlayer;
            result = &pone;
        }

        if(result != &nullPlayer) return result;

        // check win by player finish
        if (maxY > world.GetWorldSize().y)
        {
            return pone.Position.y > ptwo.Position.y ? &pone : &ptwo;
        }

        return &nullPlayer;
    }

    void SkiGame::DrawPlayerWorld(Ref<Spritebatch> batcher, Player& p, float delta)
    {
        if(p.GetHealth() > 0)
        {
            batcher->Begin(shader.get(), camera.get(), p.TintColor);
            p.Render(delta, batcher);
            batcher->End();

            // Render players labels.
            // I have to render in separate batchs because i want different tints.
            // I can improve this in the future in the framework...
            batcher->Begin(shader.get(), camera.get(), glm::vec4(p.Color.r, p.Color.g, p.Color.b, 0.6f));
            batcher->DrawString(font.get(), p.Position.x, p.Position.y - 20, p.Name.c_str());
            batcher->End();
        }
    }

}