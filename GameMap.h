#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <Windows.h>

#include "Equations.h"
#include "WORLD.h"
/*
	Small Map  2000x1000y
	200 biomes

	Medium Map 3500x2000y
	676 biomes  // 700

	Large Map  5200x3000y
	1521 biomes //1560
 
	WORLD SIZE
	8000x4000y
*/



struct MYRECT
{
	short tall, wide;
};



struct Loc
{
	Loc() { tileID = 0; biomeID = 0; }
	unsigned char tileID;
	unsigned short biomeID;
};
class visualMap
{

public:
	visualMap();
	visualMap(int, int);
	visualMap(std::string mapName);  // load map chunk from file! 

	~visualMap();

	Loc getLocation(int,int);  // returns the loc object holding the ID

	void displaySqare(int x, int y, MYRECT); // THIS IS FOR CONSOLE ONLY

	void setTileID(int x, int y, char tileID);
	void setBiomeID(int x, int y, char biomeID);

	bool setChunkSize(int x, int y);
	
	//bool loadChunk(int globalX, int globalY); // uses global positioning to load a specific chunk from the map file 

	void saveMap(std::string mapName);

	bool RenderInitalMap();

private:
	
	bool isRendered() { return _rendered; }

	Loc** Map;  // this is the large chunk that contains 4x4 set of chunks that resuse the allocated space!

	void DeleteMap();

	bool _rendered;

	int sizeX, sizeY;

};


class GameMap
{
public:
	GameMap();
	GameMap(std::string fileName); // load from file

	const int getBiomeCount() { return _biomeCount; }

	//when engine saves the game, these funcitons will be called to handle saving the map!
	//these functions ensure all dynamically allocated pieces are saved accordingly!
	void saveMap(); // This will save any active visual chunks!
	bool loadMap(char mapName);

	void displayWorldCoords(); // this is a cheat for debug!

	bool CreateNewMap(std::string mapName); // this needs to create the visual map and create a biome file! 

	void displayMap(int, int, MYRECT);

	~GameMap();
private:

	void DetermineBiome(Biome&, std::vector<BiomeInfo>*);

	bool CreateBiomeFile();
	std::vector<BiomeInfo>* loadResourceInfo();  //returns pointer to an array of biomeInfo

	int _biomeCount;

	int _worldX;
	int _worldY;

	int _sizeX;
	int _sizeY;

	int _worSizeX;
	int _worSizeY;

	std::string _mapName;

	visualMap* _visualMap;

	World* _refWorld;

};

#endif