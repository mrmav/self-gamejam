#include <iostream>
#include <Engine.h>
#include <json.hpp>
#include <map>

#include "SkiGame.h"
#include "MapLoader.h"
#include "Sprite.h"

using namespace Engine;
using json = nlohmann::json;
using AnimationSet = std::map<std::string, FrameAnimation>;


namespace GameContent
{   

    Ref<Camera2D> camera;
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
    AnimationSet badguyAnimationsMap;

    void UpdateDebugCamera(float delta, Ref<Camera2D> camera)
    {
        float speed = 1000.0f;
        float zoomSpeed = 1.0f;
        camera->Position.x += Input::GetAxisStrength(0, GamePadAxis::GamePadAxisLeftX) * speed * delta;
        camera->Position.y += Input::GetAxisStrength(0, GamePadAxis::GamePadAxisLeftY) * speed * delta;
        
        static Cursor mpos;
        static bool dragging = false;

        if(Input::IsKeyPressed(Key::A))
        {
            camera->Position.x -= speed * delta;
        } else if (Input::IsKeyPressed(Key::D))
        {
            camera->Position.x += speed * delta;
        }
        if(Input::IsKeyPressed(Key::W))
        {
            camera->Position.y -= speed * delta;
        } else if (Input::IsKeyPressed(Key::S))
        {
            camera->Position.y += speed * delta;
        }
                        
        if (!dragging && Input::IsMouseButtonPressed(Mouse::MouseButtonLeft))
        {   
            mpos = Input::GetCursorPosition();
            dragging = true;
        }
        
        if (Input::IsKeyPressed(Key::LeftControl) && Input::IsMouseButtonPressed(Mouse::MouseButtonRight))
        {        
            camera->Zoom -= max(0.0f, (camera->Zoom / zoomSpeed)) * delta;            
        }
        else if (Input::IsMouseButtonPressed(Mouse::MouseButtonRight))
        {        
            camera->Zoom += max(0.0f, (camera->Zoom / zoomSpeed)) * delta;
        }

        if(dragging)
        {
            if (Input::IsMouseButtonUp(Mouse::MouseButtonLeft))
            {
                dragging = false;
            }
            glm::vec3 diff = (glm::vec3(Input::GetCursorPosition().x, Input::GetCursorPosition().y, 0) - glm::vec3(mpos.x, mpos.y, 0));
            //std::cout << Input::GetCursorPosition().x << ", " << Input::GetCursorPosition().y << std::endl;
            //std::cout << glm::to_string(diff) << std::endl;
            camera->Position += diff * delta;
            //camera->Position.x += 10 * delta;
            //mpos = Input::GetCursorPosition();
        }

        // clamp zoom
        //camera->Zoom = max(0.02f, min(30.0f, camera->Zoom));
    }

    SkiGame::SkiGame(uint32_t screenWidth, uint32_t screenHeight, const char* windowTitle)
        : Engine::Game(screenWidth, screenHeight, windowTitle)
    {
        
    };

    SkiGame::~SkiGame() { };

    void SkiGame::Load()
    {
        // useRenderTarget = true;
        // renderTargetWidth  = 800 / 4;
        // renderTargetHeight = 600 / 4;
        // SetRenderTarget();
        
        map = MapLoader("assets/tiled/map.json");
        // json map_data = map.GetData();
        // std::cout << map_data["version"] << std::endl;

        camera = std::make_shared<Camera2D>(GetViewport());
        camera->Position.z = -100;

        batcher = std::make_shared<Spritebatch>();
        shader = std::make_shared<Shader>("assets/shaders/vertex.vert", "assets/shaders/fragment.frag");

        TILEMAP = std::make_shared<Texture2D>("assets/kenney_tiny-ski/Tilemap/tilemap.png", TextureParams());
        tileset = std::make_shared<TileSet>(TILEMAP, 12, 0, 16, 16, 0, 1);

        // animation = std::make_shared<FrameAnimation>(TILEMAP, frames.data(), 3);
        // animation->loop = true;
        // animation->SetFps(6);

        // test sprites
        badguyAnimationsMap = {
            {"idle",   FrameAnimation(TILEMAP, badGuyIdleFrames.data(),   2)},
            {"attack", FrameAnimation(TILEMAP, badGuyAttackFrames.data(), 1)}
        };
        badguy = Sprite(8 * 16, 7 * 16, std::make_shared<AnimationSet>(badguyAnimationsMap));

        Input::SetDeadZone(0, 0.2f);

    }

    void SkiGame::Update(float delta)
    {   

        UpdateDebugCamera(delta, camera);
        camera->Update(delta);

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
            
        batcher->Begin(shader.get(), camera.get(), glm::vec4(1), -100);

        //batcher->Draw(animation->GetTexture().get(), -8, 0, *animation->GetCurrentFrame());
        badguy.Render(delta, batcher);

        batcher->End();
    };

    

}