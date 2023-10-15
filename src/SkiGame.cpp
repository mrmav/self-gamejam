#include <iostream>
#include <Engine.h>
#include <json.hpp>
#include <map>
#include <algorithm>

#include "SkiGame.h"
#include "MenuState.h"
#include "PlayState.h"

namespace GameContent
{   

    // NOTE: It is important to understand that creating objects
    // in the GameContent namespace that use OpenGL (such as SpriteBatch
    // or Framebuffer), will cause a seg fault due to the order that the 
    // objects are destroyed. These types of objects should be created
    // inside the game class (SkiGame in this example). This way, they are 
    // destroyed in the correct order.
    
    SkiGame::SkiGame(uint32_t screenWidth, uint32_t screenHeight, const char* windowTitle)
        : Engine::Game(screenWidth, screenHeight, windowTitle)
    {
        
    };

    SkiGame::~SkiGame() { };

    void SkiGame::Load()
    {        
        // Game state initialization
        StateManager::GetSingleton().AddState("menuState", new MenuState(this));
        StateManager::GetSingleton().AddState("playState", new PlayState(this));
        StateManager::GetSingleton().ChangeState("menuState", false);


        // Input system
        Input::SetDeadZone(0, 0.2f);
        // a keyboard example of cursor controller
        playerTwoController = std::make_shared<InputCursor>(Key::Left, Key::Right, Key::Space);
        // for a gamepad controller:
        playerOneController = std::make_shared<InputCursor>(0, GamePadButton::DPadLeft, GamePadButton::DPadRight, GamePadButton::ButtonA);
        // for a gamepad controller with axis (sticks):
        //Ref<InputCursor> playerOneController = std::make_shared<InputCursor>(0, true, GamePadAxis::GamePadAxisLeftX, GamePadButton::ButtonA);

    }

    void SkiGame::Shutdown()
    {
        
    }

}