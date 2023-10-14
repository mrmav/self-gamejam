#include "MenuState.h"

namespace GameContent
{

    MenuState::MenuState(Game* game)
        : GameState(game)
    {
        
    }

    MenuState::~MenuState()
    {

    }

    void MenuState::Init()
    {

    }

    void MenuState::Update(float delta)
    {
        if(Input::IsKeyJustDown(Key::F))
        {
            _ENGINE_LOG("MenuState", "Queueing state change")
            StateManager::GetSingleton().ChangeState("playState", false);
        }
    }

    void MenuState::Render(float delta)
    {

    }

    void MenuState::Shutdown()
    {

    }


}