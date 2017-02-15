#ifndef WORLD_H
#define WORLD_H

#include "Biome.h"

static const int WORLD_SIZEX = 8000;
static const int WORLD_SIZEY = 4000;

class World
{
public:
	World();
	World(int wCount);
	World(bool);
	~World();

	int _waterBiomes;

	bool checkOccupancy(int x, int y);

	void CreateWaterBiomes();

	WaterBiome** wb;

	int WorldX, WorldY;

};




#endif
