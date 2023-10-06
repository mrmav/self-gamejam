#include "GameCamera.h"

namespace GameContent
{   
    // keeps the camera bounds this many tiles outwards from the player
    const int PLAYER_DISPLAY_BLEED = 2;  
    const int MIN_DISPLAY_WIDTH_TILES = 12;
    const int MAX_DISPLAY_WIDTH_TILES = 30;

    GameCamera::GameCamera(Viewport& viewport, MapLoader& map)
        : Camera2D(viewport), _Map(map)
    {

    }

    GameCamera::~GameCamera() { }
    
    void GameCamera::Update(float delta, Player& pone, Player& ptwo)
    {
        UpdateInGameCamera(delta, pone, ptwo);
        //UpdateDebugCamera(delta, camera);

        Camera2D::Update(delta);
    }

    void GameCamera::UpdateInGameCamera(float delta, Player& pone, Player& ptwo)
    {
        // Calculate the desired camera center: (this would be inbetween the players)
        glm::vec2 deltaDistance = pone.Position - ptwo.Position + _Map.GetTileSize(); // must add the tile offset
        glm::vec2 midPoint = ptwo.Position + deltaDistance * 0.5f;

        // Calculate the width of the display viewport.
        // This will make the camera zoom in and out dynamically:
        uint32_t margin = _Map.GetTileSize().x * PLAYER_DISPLAY_BLEED * 2;  // a margin around the players
        uint32_t cameraMinViewportWidth = MIN_DISPLAY_WIDTH_TILES * _Map.GetTileSize().x;
        uint32_t cameraMaxViewportWidth = MAX_DISPLAY_WIDTH_TILES * _Map.GetTileSize().x;
        uint32_t cameraViewportWidth = glm::abs(deltaDistance.x) + margin;
        
        // Constrain to min and max
        cameraViewportWidth = std::max(static_cast<uint32_t>(cameraViewportWidth), cameraMinViewportWidth);
        cameraViewportWidth = std::min(cameraViewportWidth, cameraMaxViewportWidth);

        // based on the calculated width, calculate the camera display viewport height:
        uint32_t cameraViewportHeight = cameraViewportWidth / GetViewport().AspectRatio();

        // Take world edges into acount, not allowing to show outside of map
        // But maintain dynamic zoom:
        
        uint32_t worldWidth  = _Map.GetWorldSize().x;
        uint32_t worldHeight = _Map.GetWorldSize().y;
        if (midPoint.x - cameraViewportWidth / 2.0f < 0)
        {
            float diff = cameraViewportWidth / 2.0f - midPoint.x;
            midPoint.x += std::abs(diff);
        }
        else if (midPoint.x + cameraViewportWidth / 2.0f > worldWidth)
        {
            float diff = worldWidth - (midPoint.x + cameraViewportWidth / 2.0f);
            midPoint.x -= std::abs(diff);
        }

        if (midPoint.y - cameraViewportHeight / 2.0f < 0)
        {
            float diff = cameraViewportHeight / 2.0f - midPoint.y;
            midPoint.y += std::abs(diff);
        }
        else if (midPoint.y + cameraViewportHeight / 2.0f > worldHeight)
        {
            float diff = worldHeight - (midPoint.y + cameraViewportHeight / 2.0f);
            midPoint.y -= std::abs(diff);
        }

        // sets the new zoom based on the calculated new rect
        Zoom = static_cast<float>(GetViewport().Width()) / static_cast<float>(cameraViewportWidth);
        Position = glm::vec3(midPoint.x, midPoint.y, 0);

    }

    // void UpdateDebugCamera(float delta, Ref<Camera2D> camera)
    // {
    //     float speed = 1000.0f;
    //     float zoomSpeed = 1.0f;
    //     // camera->Position.x += Input::GetAxisStrength(0, GamePadAxis::GamePadAxisLeftX) * speed * delta;
    //     // camera->Position.y += Input::GetAxisStrength(0, GamePadAxis::GamePadAxisLeftY) * speed * delta;
        
    //     static Cursor mpos;
    //     static bool dragging = false;

    //     if(Input::IsKeyPressed(Key::A))
    //     {
    //         camera->Position.x -= speed * delta;
    //     } else if (Input::IsKeyPressed(Key::D))
    //     {
    //         camera->Position.x += speed * delta;
    //     }
    //     if(Input::IsKeyPressed(Key::W))
    //     {
    //         camera->Position.y -= speed * delta;
    //     } else if (Input::IsKeyPressed(Key::S))
    //     {
    //         camera->Position.y += speed * delta;
    //     }
                        
    //     if (!dragging && Input::IsMouseButtonPressed(Mouse::MouseButtonLeft))
    //     {   
    //         mpos = Input::GetCursorPosition();
    //         dragging = true;
    //     }
        
    //     if (Input::IsKeyPressed(Key::LeftControl) && Input::IsMouseButtonPressed(Mouse::MouseButtonRight))
    //     {        
    //         camera->Zoom -= max(0.0f, (camera->Zoom / zoomSpeed)) * delta;            
    //     }
    //     else if (Input::IsMouseButtonPressed(Mouse::MouseButtonRight))
    //     {        
    //         camera->Zoom += max(0.0f, (camera->Zoom / zoomSpeed)) * delta;
    //     }

    //     if(dragging)
    //     {
    //         if (Input::IsMouseButtonUp(Mouse::MouseButtonLeft))
    //         {
    //             dragging = false;
    //         }
    //         glm::vec3 diff = (glm::vec3(Input::GetCursorPosition().x, Input::GetCursorPosition().y, 0) - glm::vec3(mpos.x, mpos.y, 0));
    //         //std::cout << Input::GetCursorPosition().x << ", " << Input::GetCursorPosition().y << std::endl;
    //         //std::cout << glm::to_string(diff) << std::endl;
    //         camera->Position += diff * delta;
    //         //camera->Position.x += 10 * delta;
    //         //mpos = Input::GetCursorPosition();
    //     }

    //     // clamp zoom
    //     //camera->Zoom = max(0.02f, min(30.0f, camera->Zoom));
    // }

}