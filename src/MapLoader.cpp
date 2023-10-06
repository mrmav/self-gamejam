#include "MapLoader.h"

using json = nlohmann::json;

namespace GameContent
{
    MapLoader::MapLoader(const char* mapFilePath)
    {
        LoadContent(mapFilePath);
    }

    void MapLoader::LoadContent(const char* mapFilePath)
    {
        std::ifstream file(mapFilePath);
		if(file.good())
		{			
			_MapData = json::parse(file);
		}

        _WorldTileSize.x = _MapData["width"];
        _WorldTileSize.y = _MapData["height"];

        _TileSize.x = _MapData["tileheight"];
        _TileSize.y = _MapData["tilewidth"];

        _WorldSize = _WorldTileSize * _TileSize;

    }
}