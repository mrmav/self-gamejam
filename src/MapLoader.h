#pragma once
#include <Engine.h>
#include "json.hpp"

using json = nlohmann::json;

namespace GameContent
{
    class MapLoader
    {
        public:
        MapLoader() { };
        MapLoader(const char* mapFilePath);
        ~MapLoader() { };

        void LoadContent(const char* mapFilePath);

        const json& GetData() const {return m_MapData; };

        private:
        json m_MapData;
    };
}