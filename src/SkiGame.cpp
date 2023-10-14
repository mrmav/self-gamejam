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
    
    SkiGame::SkiGame(uint32_t screenWidth, uint32_t screenHeight, const char* windowTitle)
        : Engine::Game(screenWidth, screenHeight, windowTitle)
    {
        
    };

    SkiGame::~SkiGame() { };

    void SkiGame::Load()
    {        
        // insert game states here:
        StateManager::GetSingleton().AddState("menuState", new MenuState(this));
        StateManager::GetSingleton().AddState("playState", new PlayState(this));
        StateManager::GetSingleton().ChangeState("menuState", false);

    }    

}