#include "MenuState.h"

namespace GameContent
{

    Ref<Texture2D> tilemap;

    MenuState::MenuState(void* game)
        : GameState(game)
    {
        
    }

    MenuState::~MenuState()
    {

    }

    void MenuState::Init()
    {   
        camera = std::make_shared<DebugCamera>(GetGame<SkiGame>()->GetViewport());
        camera->Zoom = 1;
        batcher = std::make_shared<Spritebatch>();
        shader = std::make_shared<Shader>("assets/shaders/vertex.vert", "assets/shaders/fragment.frag");
        font = std::make_shared<BitmapFont>("assets/font.png", 16, 16);

        tilemap = std::make_shared<Texture2D>("assets/kenney_tiny-ski/Tilemap/tilemap.png", TextureParams());
        
    }

    void MenuState::Update(float delta)
    {
        camera->Update(delta);

        if(Input::IsKeyJustDown(Key::Space))
        {
            _ENGINE_LOG("MenuState", "Queueing state change")
            StateManager::GetSingleton().ChangeState("playState", false);
        }
    }

    void MenuState::Render(float delta)
    {
        glClearColor(0.392, 0.584, 0.929, 1);  // good ol' cornflower blue
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        batcher->Begin(shader.get(), camera.get(), glm::vec4(1, 0, 0, 1));
        batcher->DrawString(font.get(), 0, 18, "Press space bar to start the game!");
        batcher->End();

    }

    void MenuState::Shutdown()
    {

    }


}