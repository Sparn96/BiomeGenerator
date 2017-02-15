#ifndef BIOME_H
#define BIOME_H

//#define DEBUG

#include <string>
#include <fstream>


enum BiomeResourceFileOrder
{
	BARREN,TEMPDECFOREST,PRARIE,BOREALFOREST,TUNDRA,POLAR,WARMDESERT,TROPGRASSLAND,COLDDESERT,SAVANNA,TROPDECFOREST,TROPRAINFOREST,HEAVYSNOWFOREST
};


struct Resource
{
	std::string name;
	int currentCount;
	float replenishRate;
	int max;
};

struct BiomeInfo
{
	std::string type;
	Resource sources[5];
};


class Biome
{
public:
	Biome();
	Biome(int x, int y, int _ID);
	~Biome();

	const std::string getName() { return _name; }

	bool SaveBiome(std::fstream&);

	int getX() { return _x; }
	int getY() { return _y; }

	bool setBiomeType(int);

	int getBiomeType() { return _biomeType; } // how the game will know what kind of biome it is

	const short getID() { return _id; }

	int FindTileID();

	//bool setX(int x);
	//bool setY(int y);

	bool setTemperature(float t) { _temperature = t; return true; }
	bool setMoisture(float m) { _moisture = m; return true; }

	bool setResources(BiomeInfo);

	const float getTemp() { return _temperature; }
	const float getMoist() { return _moisture; }

	BiomeInfo getResources() { return _resources; }

	//bool CreateNew();

	//fill with useful info later

protected:
	short _id;
	std::string _name;

	int _biomeType;

	int _x;
	int _y;

	float _temperature;
	float _moisture;

	BiomeInfo _resources;


};

class WaterBiome : public Biome
{
public:
	WaterBiome();
	WaterBiome(int x, int y);
	~WaterBiome();

	bool setR(float radius) { _radius = radius; return true; }

	float getR() { return _radius; }

	

protected:
	float _radius;

};



#endif