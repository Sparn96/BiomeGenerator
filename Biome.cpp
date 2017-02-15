#include "Biome.h"

//#define DEBUG

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


bool Biome::SaveBiome(std::fstream& biomeFile) // add safety checks later
{
	int strSize = 0;
	short idTemp = 0;
	strSize = _name.size();
	idTemp = _id;
	biomeFile.write((char*)&idTemp, sizeof(short));

	biomeFile.write((char*)&strSize, sizeof(int));
	biomeFile.write(_name.c_str(), strSize);

	strSize = _biomeType;

	biomeFile.write((char*)&strSize, sizeof(int));

	strSize = _x;
	biomeFile.write((char*)&strSize, sizeof(int));
	strSize = _y;
	biomeFile.write((char*)&strSize, sizeof(int));

	float tempFloat = _temperature;
	biomeFile.write((char*)&tempFloat, sizeof(float));
	tempFloat = _moisture;
	biomeFile.write((char*)&tempFloat, sizeof(float));
	//biome resource write

	strSize = _resources.type.size();
	biomeFile.write((char*)&strSize, sizeof(int));
	biomeFile.write(_resources.type.c_str(), strSize);   // string and size of string

	for (int re = 0; re < 5; re++) // each of the 5 contained resources
	{
		strSize = _resources.sources[re].name.size();
		biomeFile.write((char*)&strSize, sizeof(int));

		biomeFile.write(_resources.sources[re].name.c_str(), _resources.sources[re].name.size());

		biomeFile.write((char *)&_resources.sources[re].currentCount, sizeof(int));
		biomeFile.write((char *)&_resources.sources[re].replenishRate, sizeof(float));
		biomeFile.write((char *)&_resources.sources[re].max, sizeof(int));
	}
	return true;
}