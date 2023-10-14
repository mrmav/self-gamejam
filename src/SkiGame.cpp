#include <iostream>
#include <Engine.h>
#include <json.hpp>
#include <map>
#include <algorithm>

#include "SkiGame.h"
#include "PlayState.h"

namespace GameContent
{   
    
    SkiGame::SkiGame(uint32_t screenWidth, uint32_t screenHeight, const char* windowTitle)
        : Engine::Game(screenWidth, screenHeight, windowTitle)
    {
        
    };

    SkiGame::~SkiGame() { };

    void SkiGame::Load()
    {        
        // insert game states here:
        StateManager::GetSingleton().AddState("menuState", std::make_shared<PlayState>(this));
        StateManager::GetSingleton().ChangeState("menuState", false);

    }    

}