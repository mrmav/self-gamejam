#include <Engine.h>
#include <json.hpp>
#include "SkiGame.h"
#include "MapLoader.h"
#include <iostream>

using namespace Engine;
using json = nlohmann::json;

namespace GameContent
{   
    Ref<Camera2D> camera;
    Ref<Spritebatch> batcher;
    Ref<Shader> shader;
    
    Ref<Texture2D> TILEMAP;
    Ref<FrameAnimation> animation;
    std::vector<Rectangle<int>> frames = 
    {
        Rectangle<int>(102, 102, 16, 16),
        Rectangle<int>(119, 102, 16, 16),
        Rectangle<int>(136, 102, 16, 16)
    };

    MapLoader map;
    Ref<TileSet> tileset;

    void UpdateDebugCamera(float delta, Ref<Camera2D> camera)
    {
        float speed = 1000.0f;
        float zoomSpeed = 1.0f;
        camera->Position.x += Input::GetAxisStrength(0, GamePadAxis::GamePadAxisLeftX) * speed * delta;
        camera->Position.y += Input::GetAxisStrength(0, GamePadAxis::GamePadAxisLeftY) * speed * delta;
        
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
                        
        if (Input::IsMouseButtonPressed(Mouse::MouseButtonLeft))
        {
            camera->Zoom -= max(0.0f, (camera->Zoom / zoomSpeed)) * delta;
        }
        else if (Input::IsMouseButtonPressed(Mouse::MouseButtonRight))
        {        
            camera->Zoom += max(0.0f, (camera->Zoom / zoomSpeed)) * delta;
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
        useRenderTarget = true;
        renderTargetWidth  = 800 / 4;
        renderTargetHeight = 600 / 4;
        SetRenderTarget();
        
        map = MapLoader("assets/tiled/map.json");
        // json map_data = map.GetData();
        // std::cout << map_data["version"] << std::endl;

        camera = std::make_shared<Camera2D>(GetViewport());
        batcher = std::make_shared<Spritebatch>();
        shader = std::make_shared<Shader>("assets/shaders/vertex.vert", "assets/shaders/fragment.frag");

        TILEMAP = std::make_shared<Texture2D>("assets/kenney_tiny-ski/Tilemap/tilemap.png", TextureParams());
        tileset = std::make_shared<TileSet>(TILEMAP, 12, 0, 16, 16, 0, 1);

        animation = std::make_shared<FrameAnimation>(TILEMAP, frames.data(), 3);
        animation->loop = true;
        animation->SetFps(6);

        Input::SetDeadZone(0, 0.2f);

    }

    void SkiGame::Update(float delta)
    {   

        UpdateDebugCamera(delta, camera);
        camera->Update(delta);

        if(Input::IsKeyJustDown(Key::Space))
        {
            animation->Play();
        }
        animation->Update(delta);

        if(Input::IsButtonJustDown(0, GamePadButton::ButtonA))
        {
            std::cout << "Btn down" << std::endl;
        }
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

        // get the terrain layer
        json terrain_layer = nullptr;
        for(auto& el : layers_data)
        {
            //std::cout << "Layer: " << el.at("name") << std::endl;
            if(el.at("name") == std::string("Terrain"))
            {
                terrain_layer = el;
                break;
            }
        }
    
        int currentTileIndex = 0;
        batcher->Begin(shader.get(), camera.get(), glm::vec4(1));
        tileset->DrawTile(batcher, glm::vec2(16,16), 0);
        
        for(int y = 0; y < mapHeight; y++)
        {
            for(int x = 0; x < mapWidth; x++)
            {   
                int tileid = terrain_layer["data"][currentTileIndex];
                currentTileIndex++;

                if(tileid == 0) continue;

                tileset->DrawTile(batcher, glm::vec2(x * 16, y * 16), --tileid); // tiled assumes that a ZERO tile id, is a non existing tile.
                                                                                 // but in our system, we treat the ZERO id as the first one.
                                                                                 // To fix this, we subtract ONE before drawing.
            }
        }
        batcher->End();
        //std::cout << "Tilemap draw: " << currentTileIndex << std::endl;
        
        batcher->Begin(shader.get(), camera.get(), glm::vec4(1));
        batcher->Draw(animation->GetTexture().get(), 0, 0, *animation->GetCurrentFrame());
        batcher->End();
    };

    

}