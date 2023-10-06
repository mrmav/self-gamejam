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

        const json& GetData() const {return _MapData; };

        const glm::vec2 GetWorldTileSize() const {return _WorldTileSize; };
        const glm::vec2 GetWorldSize() const {return _WorldSize; };
        const glm::vec2 GetTileSize() const {return _TileSize; };

        private:
        json _MapData;

        glm::vec2 _WorldTileSize;
        glm::vec2 _WorldSize;
        glm::vec2 _TileSize;
    };
}