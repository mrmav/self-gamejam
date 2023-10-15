#include "DebugCamera.h"

namespace GameContent
{   
    DebugCamera::DebugCamera(Viewport& viewport)
        : Camera2D(viewport)
    {

    }

    DebugCamera::~DebugCamera() { }
    
    void DebugCamera::Update(float delta)
    {
        UpdateDebug(delta);

        Camera2D::Update(delta);
    }


    void DebugCamera::UpdateDebug(float delta)
    {
        float speed = 1000.0f;
        float zoomSpeed = 1.0f;
        // camera->Position.x += Input::GetAxisStrength(0, GamePadAxis::GamePadAxisLeftX) * speed * delta;
        // camera->Position.y += Input::GetAxisStrength(0, GamePadAxis::GamePadAxisLeftY) * speed * delta;
        
        static Cursor mpos;
        static bool dragging = false;

        if(Input::IsKeyPressed(Key::A))
        {
            Position.x -= speed * delta;
        } else if (Input::IsKeyPressed(Key::D))
        {
            Position.x += speed * delta;
        }
        if(Input::IsKeyPressed(Key::W))
        {
            Position.y -= speed * delta;
        } else if (Input::IsKeyPressed(Key::S))
        {
            Position.y += speed * delta;
        }
                        
        if (!dragging && Input::IsMouseButtonPressed(Mouse::MouseButtonLeft))
        {   
            mpos = Input::GetCursorPosition();
            dragging = true;
        }
        
        if (Input::IsKeyPressed(Key::LeftControl) && Input::IsMouseButtonPressed(Mouse::MouseButtonRight))
        {        
            Zoom -= max(0.0f, (Zoom / zoomSpeed)) * delta;            
        }
        else if (Input::IsMouseButtonPressed(Mouse::MouseButtonRight))
        {        
            Zoom += max(0.0f, (Zoom / zoomSpeed)) * delta;
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
            Position += diff * delta;
            //camera->Position.x += 10 * delta;
            //mpos = Input::GetCursorPosition();
        }

        // clamp zoom
        //camera->Zoom = max(0.02f, min(30.0f, camera->Zoom));
    }

}