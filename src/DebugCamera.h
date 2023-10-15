#pragma once

#include <Engine.h>

using namespace Engine;

namespace GameContent
{

    class DebugCamera : public Camera2D
    {
        public:

        DebugCamera(Viewport& viewport);
        ~DebugCamera();

        void Update(float delta);
        void UpdateDebug(float delta);

    };

} // namespace GameContent


