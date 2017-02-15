#include "WORLD.h"

//float GaussCurve(float x, float curveHeight, float posCenter, float standDev)
//{
//	const float e = 2.71828f;
//	float result = 0.0f;
//
//	result = pow((curveHeight * e), (-((pow((x - posCenter), 2)) / (2 * pow(standDev, 2)))));  // Gaussian Equation
//
//	return result;
//}
//float calcPointDist(float x1, float y1, float x2, float y2)
//{
//	float result = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
//	return abs(result);
//}
//int random(int a, int b) //Returns a random number between a and b
//{
//	if (a == b) return a;
//	return (a < b) ? (rand() % b + a) : (rand() % a + b);
//}

World::World()
{
	//create x(based on a dry/moderate/wet slider for the map that will determine the # of water biomes) random biomes of moisture and dynamically allocate them into an array.
	_waterBiomes = 0;  
	WorldX = WORLD_SIZEX;
	WorldY = WORLD_SIZEY;
	//no water biomes to create!
	
}

World::World(bool makeOne)
{
	_waterBiomes = 1;
	WorldX = WORLD_SIZEX;
	WorldY = WORLD_SIZEY;

	wb = new WaterBiome*[1];
	
	wb[0] = new WaterBiome(4000, 2000);
	
	wb[0]->setR(2000);

}

World::World(int wCount)
{
	_waterBiomes = wCount;
	WorldX = WORLD_SIZEX;
	WorldY = WORLD_SIZEY;
	CreateWaterBiomes();
}


World::~World()
{
	for (int i = 0; i < _waterBiomes; i++)  // water biome cleanup
	{
		delete wb[i];
	}
	delete [] wb; 
}


void World::CreateWaterBiomes()
{
	wb = new WaterBiome*[_waterBiomes];
	int waterX(0), waterY(0);
	for (int i = 0; i < _waterBiomes; i++)
	{
		waterX = rand() % WorldX;
		waterY = rand() % WorldY;

		wb[i] = new WaterBiome(waterX, waterY);
	}

	//create the Radius of each biome by searching for their closest neighbors
	WaterBiome* closer;
	for (int i = 0; i < _waterBiomes; i++)
	{
		float curr = 0.0f;
		float closest = 0.0f;

		closer = wb[i];
		for (int j = 0; j < _waterBiomes; j++)
		{

			curr = sqrt(pow(((float)wb[j]->getX() - (float)(wb[i]->getX())), 2) + pow(((float)wb[j]->getX() - (float)wb[i]->getY()), 2));

			if (closer == wb[j]); // check if self

			else if (curr < closest && closest != 0)  // if the current is less than the closest  it is the new closest 
			{
				closest = curr;
			}
			else if (closest == 0)   // this means this is first it has found
			{
				closest = curr;
			}

		}
		wb[i]->setR(closest + 10.0f);   //define each biomes moisture radius (+10 spaces)
	}

	std::ofstream waterFile;
	waterFile.open("waterBiome.wbm", std::ios::out | std::ios::binary);
	for (int i = 0; i < _waterBiomes; i++)
	{
		waterFile.write(reinterpret_cast<const char*>(wb[i]), sizeof(WaterBiome));            ///THIS MAY NOT WORK !!!!
	}

}

bool World::checkOccupancy(int x, int y)
{
	for (int i = 0; i < _waterBiomes; i++)
	{
		if (wb[i]->getX() == x)
		{
			if (wb[i]->getY() == y)
				return true;
		}
	}
	return false;
}