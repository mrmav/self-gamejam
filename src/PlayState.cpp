#include "PlayState.h"

namespace GameContent
{
    PlayState::PlayState(void* game)
        : GameState(game)
    {
        _ENGINE_LOG("PlayState", "Constructor")
    }

    PlayState::~PlayState()
    {
        _ENGINE_LOG("PlayState", "Destructor")
    }

    void PlayState::Init()
    {
        _ENGINE_LOG("PlayState", "Init")

        map = MapLoader("assets/tiled/map.json");

        Viewport& v = GetGame<SkiGame>()->GetViewport();

        camera = std::make_shared<GameCamera>(v, map);
        camera->Zoom = 1;
        batcher = std::make_shared<Spritebatch>();
        shader = std::make_shared<Shader>("assets/shaders/vertex.vert", "assets/shaders/fragment.frag");
        font = std::make_shared<BitmapFont>("assets/font.png", 16, 16);

        tilemap = std::make_shared<Texture2D>("assets/kenney_tiny-ski/Tilemap/tilemap.png", TextureParams());
        tileset = std::make_shared<TileSet>(tilemap, 12, 0, 16, 16, 0, 1);

        // test sprites
        badguyAnimationsMap = {
            {"idle",   FrameAnimation(tilemap, badGuyIdleFrames.data(),   2)},
            {"attack", FrameAnimation(tilemap, badGuyAttackFrames.data(), 1)}
        };
        badguy = Sprite(8 * 16, 7 * 16, std::make_shared<AnimationSet>(badguyAnimationsMap));

        // test sprites
        playerOneAnimationsMap = {
            {"ski", FrameAnimation(tilemap, playerOneSkiFrames.data(), 2)}
        };
        playerTwoAnimationsMap = {
            {"ski", FrameAnimation(tilemap, playerTwoSkiFrames.data(), 2)}
        };
        
        playerOne = Player(14 * 16, 2 * 16, std::make_shared<AnimationSet>(playerOneAnimationsMap), GetGame<SkiGame>()->playerOneController, &map);
        playerOne.SetAnimation("ski")->Play();
        playerOne.Name = "1";
        playerOne.Color = glm::vec4(0.819608f, 0.462745f, 0.815686, 1);

        playerTwo = Player(16 * 16, 2 * 16, std::make_shared<AnimationSet>(playerTwoAnimationsMap), GetGame<SkiGame>()->playerTwoController, &map);
        playerTwo.SetAnimation("ski")->Play();
        playerTwo.Name = "2";
        playerTwo.Color = glm::vec4(0.517647f, 0.776471f, 0.411765, 1);

        
    }

    void PlayState::Update(float delta)
    {
        //_ENGINE_LOG("PlayState", "Update")

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
        if(Input::IsKeyJustDown(Key::Escape))
        {
            StateManager::GetSingleton().ChangeState("menuState", false);
        }

        badguy.Update(delta);
        playerOne.Update(delta);
        playerTwo.Update(delta);

        camera->Update(delta, playerOne, playerTwo);


        if(winningPlayer != &nullPlayer && (Input::IsKeyJustDown(Key::R) || Input::IsButtonJustDown(0, GamePadButton::Start)))
        {
            Reset();
        }

        if(winningPlayer == &nullPlayer)
            winningPlayer = CheckWinningCondition(map, playerOne, playerTwo);
    }

    void PlayState::Render(float delta)
    {
        //_ENGINE_LOG("PlayState", "Draw")
        glClearColor(0.392, 0.584, 0.929, 1);  // good ol' cornflower blue
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Viewport& v = GetGame<SkiGame>()->GetViewport();
        
        // lets try to render the map
        json map_data = map.GetData();
        int mapWidth  = map_data["width"];
        int mapHeight = map_data["height"];        
        json layers_data = map_data.at("layers");

        // We area assuming that there is only one tileset.
        // Assuming this, we render each layer in a separate pass:        
        uint16_t depth = 0;
        batcher->Begin(shader.get(), camera.get());
        for(auto& layer : layers_data)
        {
            
            if(layer["type"] != std::string("tilelayer")) continue;
            //if(layer["name"] == std::string("Terrain")) continue;

            int currentTileIndex = 0;


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
            depth += 1;
            
        }

        badguy.Render(delta, batcher);
        DrawPlayerWorld(batcher, playerOne, delta);
        DrawPlayerWorld(batcher, playerTwo, delta);
        
        batcher->End();



        // Render UI stuff with the custom transform
        float UIScale = 4.0f;
        batcher->SetCustomView(glm::mat4(1) * glm::scale(glm::vec3(UIScale)));

        // render player health
        batcher->Begin(shader.get(), camera.get(), 0, true);
        DrawPlayerHealth(batcher, playerOne, glm::vec2(8, 8), 1);
        DrawPlayerHealth(batcher, playerTwo, glm::vec2(v.Width() / UIScale - 9 - 8, 8), -1);


        // Render winning string:
        if(winningPlayer != &nullPlayer)
        {
            std::string str = std::string("Player ") + winningPlayer->Name;
            glm::vec2 strSize = font->GetStringSize(str);
            glm::vec2 UIDisplaySize = glm::vec2(v.Width(), v.Height()) / UIScale;
            
            batcher->DrawString(font.get(), UIDisplaySize.x / 2.0f - strSize.x / 2.0f, UIDisplaySize.y / 2.0f - strSize.y / 2.0f, str.c_str(), winningPlayer->Color);
            
            str = "Wins!";
            strSize = font->GetStringSize(str);
            batcher->DrawString(font.get(), UIDisplaySize.x / 2.0f - strSize.x / 2.0f, UIDisplaySize.y / 2.0f - strSize.y / 2.0f + 16.0f, str.c_str(), winningPlayer->Color);

            str = "Restart: r";
            strSize = font->GetStringSize(str);
            batcher->DrawString(font.get(), UIDisplaySize.x / 2.0f - strSize.x / 2.0f, UIDisplaySize.y / 2.0f - strSize.y / 2.0f + 48.0f, str.c_str(), winningPlayer->Color);

            str = "or gamepad";
            strSize = font->GetStringSize(str);
            batcher->DrawString(font.get(), UIDisplaySize.x / 2.0f - strSize.x / 2.0f, UIDisplaySize.y / 2.0f - strSize.y / 2.0f + 48.0f + 16.0f, str.c_str(), winningPlayer->Color);

            str = "start";
            strSize = font->GetStringSize(str);
            batcher->DrawString(font.get(), UIDisplaySize.x / 2.0f - strSize.x / 2.0f, UIDisplaySize.y / 2.0f - strSize.y / 2.0f + 48.0f + 32.0f, str.c_str(), winningPlayer->Color);

        }

        batcher->End();
    }

    void PlayState::Shutdown()
    {
        _ENGINE_LOG("PlayState", "Shutdown")
    }

    void PlayState::DrawPlayerHealth(Ref<Spritebatch> batcher, Player const& player, glm::vec2 start, int dir) const
    {      
        float offset = 1;

        for(int i = 0; i < player.GetHealth(); i++)
        {   
            float x = start.x + ((9 + offset) * i) * dir;
            batcher->Draw(tilemap.get(), x, start.y, Rectangle<int>(136, 53, 9, 9));            

        }
    }

    Player* PlayState::CheckWinningCondition(MapLoader& world, Player& pone, Player& ptwo)
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

    void PlayState::DrawPlayerWorld(Ref<Spritebatch> batcher, Player& p, float delta)
    {
        if(p.GetHealth() > 0)
        {
            p.Render(delta, batcher);
            batcher->DrawString(font.get(), p.Position.x, p.Position.y - 20, p.Name.c_str(), glm::vec4(p.Color.r, p.Color.g, p.Color.b, 0.6f));
        }
    }

    void PlayState::Reset()
    {
        playerOne.Reset();
        playerTwo.Reset();

        playerOne.SetAnimation("ski")->Play();
        playerTwo.SetAnimation("ski")->Play();

        playerOne.Position = glm::vec2(14 * 16, 2 * 16);
        playerTwo.Position = glm::vec2(16 * 16, 2 * 16);

        winningPlayer = &nullPlayer;
    }

}