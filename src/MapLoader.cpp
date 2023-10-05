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
			m_MapData = json::parse(file);
		}
    }
}