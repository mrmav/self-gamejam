#include "GameCamera.h"

namespace GameContent
{   
    // keeps the camera bounds this many tiles outwards from the player
    const int PLAYER_DISPLAY_BLEED = 2;  
    const int MIN_DISPLAY_WIDTH_TILES = 12;
    const int MAX_DISPLAY_WIDTH_TILES = 30;

    // keeps track of the camera center distance to the player in front;
    float offsetCenterY = 0;

    /*
    
    The camera would ideally never leave the world bounds.
    When a player is ahead of another player, the player follows it.
    If a player gets to behind, the player ahead wins.

    */


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
        Rectangle<float> boundingRect = GetPlayersBoundingBox(pone, ptwo);
        glm::vec2 cameraCenter = glm::vec2(boundingRect.X + boundingRect.HalfWidth(), boundingRect.Y + boundingRect.HalfHeight());

        ZoomToFit(boundingRect);

        // If after this, the camera display width is bigger than the world width,
        // it means the players are too far away.
        // How to solve this?
        float maxZoomOut = GetViewport().Width() / _Map.GetWorldSize().x;
        if(Zoom < maxZoomOut)
        {
            Zoom = maxZoomOut;
            glm::vec2 displaySize = GetCameraDisplaySize();

            glm::vec2 margin = _Map.GetTileSize();  // a margin around the players
            margin *= PLAYER_DISPLAY_BLEED;
            
            Position.y = glm::max(pone.Position.y, ptwo.Position.y) - offsetCenterY;

        } else
        {
            Position.y = cameraCenter.y;
        }
        Position.x = cameraCenter.x;

        HandleWorldBounds();

        offsetCenterY = glm::max(pone.Position.y, ptwo.Position.y) - Position.y;

    }

    void GameCamera::ZoomToFit(Rectangle<float>& rect)
    {
        float hZoom = static_cast<float>(GetViewport().Width()) / rect.Width();
        float vZoom = static_cast<float>(GetViewport().Height()) / rect.Height();

        Zoom = glm::min(hZoom, vZoom);
    }

    void GameCamera::HandleWorldBounds()
    {
        glm::vec2 cameraRectSize = GetCameraDisplaySize();

        uint32_t worldWidth  = _Map.GetWorldSize().x;
        uint32_t worldHeight = _Map.GetWorldSize().y;
        if (Position.x - cameraRectSize.x / 2.0f < 0)
        {
            float diff = cameraRectSize.x / 2.0f - Position.x;
            Position.x = cameraRectSize.x / 2.0f;          

        }
        else if (Position.x + cameraRectSize.x / 2.0f > worldWidth)
        {
            //float diff = worldWidth - (Position.x + cameraRectSize.x / 2.0f);
            Position.x = worldWidth - cameraRectSize.x / 2.0f;
        }

        if (Position.y - cameraRectSize.y / 2.0f < 0)
        {
            float diff = cameraRectSize.y / 2.0f - Position.y;
            Position.y = cameraRectSize.y / 2.0f;

        }
        else if (Position.y + cameraRectSize.y / 2.0f > worldHeight)
        {
            float diff = worldHeight - (Position.y + cameraRectSize.y / 2.0f);
            Position.y = worldHeight - cameraRectSize.y / 2.0f;
        }

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