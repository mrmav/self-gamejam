#include <Engine.h>
#include "SkiGame.h"

using namespace Engine;

namespace GameContent
{   
    Ref<Camera2D> camera;
    Ref<Spritebatch> batcher;
    Ref<Shader> shader;
    
    Ref<Texture2D> TILEMAP;
    Ref<FrameAnimation> animation;
    std::vector<Rectangle<int>> frames = 
    {
        Rectangle<int>(102, 102, 16, 16),
        Rectangle<int>(119, 102, 16, 16),
        Rectangle<int>(136, 102, 16, 16)
    };

    SkiGame::SkiGame(uint32_t screenWidth, uint32_t screenHeight, const char* windowTitle)
        : Engine::Game(screenWidth, screenHeight, windowTitle)
    {
        
    };

    SkiGame::~SkiGame() { };

    void SkiGame::Load()
    {
        useRenderTarget = true;
        renderTargetHeight = 800 / 4;
        renderTargetWidth  = 600 / 4;
        SetRenderTarget();
        

        camera = std::make_shared<Camera2D>(GetViewport());
        batcher = std::make_shared<Spritebatch>();
        shader = std::make_shared<Shader>("assets/shaders/vertex.vert", "assets/shaders/fragment.frag");

        TILEMAP = std::make_shared<Texture2D>("assets/kenney_tiny-ski/Tilemap/tilemap.png", TextureParams());
        
        animation = std::make_shared<FrameAnimation>(TILEMAP, frames.data(), 3);
        animation->loop = true;
        animation->SetFps(12);
    }

    void SkiGame::Update(float delta)
    {   
        camera->Update(delta);

        if(Input::IsKeyJustDown(Key::Space))
        {
            animation->Play();
        }
        animation->Update(delta);
    };


    void SkiGame::Render(float delta)
    {

        glClearColor(0.392, 0.584, 0.929, 1);  // good ol' cornflower blue
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const Rectangle<int>* rect = animation->GetCurrentFrame();
        //std::cout << rect->X << std::endl;

        batcher->Begin(shader.get(), camera.get(), glm::vec4(1));
        batcher->Draw(animation->GetTexture().get(), 0, 0, *animation->GetCurrentFrame());
        batcher->End();
    };

}