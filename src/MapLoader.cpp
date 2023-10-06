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

        // store the terrain data
        json layers_data = _MapData.at("layers");

        for(auto& layer : layers_data)
        {
            if(layer["name"] == std::string("Terrain"))
            {
                _TerrainLayerData = layer.at("data").get<std::vector<int>>();
            }
            
        }

    }

    bool MapLoader::CheckTilesIndex(const std::vector<uint32_t>& tiles, uint32_t value) const
    {
        for(int i = 0; i < tiles.size(); i++)
        {
            if(tiles[i] == value) return true;
        }
        return false;
    };
}