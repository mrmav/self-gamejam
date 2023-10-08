#include <iostream>
#include <Engine.h>

#include "SkiGame.h"

int main()
{
    _ENGINE_LOG("Launcher", "Hello configured project!.")
    
    GameContent::SkiGame game = GameContent::SkiGame(720, 1280, "Ski Game");
    game.Run();

    return 0;
}