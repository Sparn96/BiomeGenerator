#include "biome.h"


Biome::Biome()
{
	_x = 0;
	_y = 0;
	_id = 0;
	_temperature = 0;
	_moisture = 0;
}
Biome::Biome(int x, int y, int ID)
{
	_x = x;
	_y = y;
	_id = ID;
}




Biome::~Biome()
{
}


bool Biome::setResources(BiomeInfo b)  // ERROR HANDLE HERE
{
	_resources = b;
	return true;
}

bool Biome::setBiomeType(int type)          // FIGURE OUT A BETTER WAY TO HANDLE THIS!
{
	if (type <= HEAVYSNOWFOREST)
	{
		_biomeType = type;
		return true;
	}
	else return false;
}

int Biome::FindTileID()  // to be later determined!
{
	//BARREN,TEMPDECFOREST,PRARIE,BOREALFOREST,TUNDRA,POLAR,WARMDESERT,TROPGRASSLAND,COLDDESERT,SAVANNA,TROPDECFOREST,TROPRAINFOREST,HEAVYSNOWFOREST
	return _biomeType;

}



WaterBiome::WaterBiome()
{
	_moisture = 100.00f;
}
WaterBiome::WaterBiome(int x, int y)
{
	_x = x;
	_y = y;
	_moisture = 100.0f;
}

WaterBiome::~WaterBiome()
{
	//
}


