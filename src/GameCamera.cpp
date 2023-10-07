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

        //glm::vec2 center = (pone.Position + ptwo.Position) * 0.5f;

        // Get the bounding rect
        Rectangle<float> boundingRect = GetPlayersBoundingBox(pone, ptwo);
        glm::vec2 midPoint = glm::vec2(boundingRect.X + boundingRect.HalfWidth(), boundingRect.Y + boundingRect.HalfHeight());

        // Calculate the width of the display viewport.
        // This will make the camera zoom in and out dynamically:
        uint32_t cameraMinViewportWidth = MIN_DISPLAY_WIDTH_TILES * _Map.GetTileSize().x;
        uint32_t cameraMaxViewportWidth = MAX_DISPLAY_WIDTH_TILES * _Map.GetTileSize().x;

        uint32_t cameraViewportWidth = std::max(boundingRect.Width(), boundingRect.Height());
        uint32_t cameraViewportHeight;
                
        // Constrain to min and max
        cameraViewportWidth = std::max(static_cast<uint32_t>(cameraViewportWidth), cameraMinViewportWidth);
        cameraViewportWidth = std::min(cameraViewportWidth, cameraMaxViewportWidth);

        // based on the calculated width, calculate the camera display viewport height:
        cameraViewportHeight = cameraViewportWidth / GetViewport().AspectRatio();

        // Take world edges into account, not allowing to show outside of map
        // But maintain dynamic zoom:        
        // uint32_t worldWidth  = _Map.GetWorldSize().x;
        // uint32_t worldHeight = _Map.GetWorldSize().y;
        // if (midPoint.x - cameraViewportWidth / 2.0f < 0)
        // {
        //     float diff = cameraViewportWidth / 2.0f - midPoint.x;
        //     midPoint.x += std::abs(diff);
        // }
        // else if (midPoint.x + cameraViewportWidth / 2.0f > worldWidth)
        // {
        //     float diff = worldWidth - (midPoint.x + cameraViewportWidth / 2.0f);
        //     midPoint.x -= std::abs(diff);
        // }

        // if (midPoint.y - cameraViewportHeight / 2.0f < 0)
        // {
        //     float diff = cameraViewportHeight / 2.0f - midPoint.y;
        //     midPoint.y += std::abs(diff);
        // }
        // else if (midPoint.y + cameraViewportHeight / 2.0f > worldHeight)
        // {
        //     float diff = worldHeight - (midPoint.y + cameraViewportHeight / 2.0f);
        //     midPoint.y -= std::abs(diff);
        // }

        // sets the new zoom based on the calculated new rect
        //Zoom = static_cast<float>(GetViewport().Width()) / static_cast<float>(cameraViewportWidth);
        
        
        glm::vec2 center = (pone.Position + ptwo.Position) * 0.5f;
        float distance = glm::distance(pone.Position, ptwo.Position);
        float desiredZoom = static_cast<float>(GetViewport().Width()) / (distance * 2.0f);
        Zoom = desiredZoom;

        Position = glm::vec3(center.x, center.y, 0);

    }

    Rectangle<float> GameCamera::GetPlayersBoundingBox(Player& pone, Player& ptwo)
    {
        glm::vec2 margin = _Map.GetTileSize();  // a margin around the players
        margin *= PLAYER_DISPLAY_BLEED;

        Rectangle<float> bounds;
        glm::vec2 topLeft;
        glm::vec2 bottomRight;
        
        topLeft.x = std::min(pone.Position.x, ptwo.Position.x);
        topLeft.y = std::min(pone.Position.y, ptwo.Position.y);
        topLeft -= margin;

        bottomRight.x = std::max(pone.Position.x + _Map.GetTileSize().x, ptwo.Position.x + _Map.GetTileSize().x);
        bottomRight.y = std::max(pone.Position.y + _Map.GetTileSize().y, ptwo.Position.y + _Map.GetTileSize().y);
        bottomRight += margin;

        bounds.X = topLeft.x;
        bounds.Y = topLeft.y;
        bounds.Width(std::abs(bottomRight.x  - topLeft.x));
        bounds.Height(std::abs(bottomRight.y - topLeft.y));

        return bounds;
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