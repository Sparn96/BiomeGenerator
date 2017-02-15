#include "GameMap.h"


//#define DEBUG
static const int AVERAGETEMP = 16; // in Celcius

static const int MAP_SIZEX = 3500;   //med map for now
static const int MAP_SIZEY = 2000;
static const int BIOME_COUNT = 200;

static const int CHUNK_X = 50;
static const int CHUNK_Y = 50;

//float GaussCurve(float x, float curveHeight, float posCenter, float standDev)
//{
//	const float e = 2.71828f;
//	float result = 0.0f;
//
//	result = pow((curveHeight * e), (-((pow((x - posCenter), 2)) / (2 * pow(standDev, 2)))));  // Gaussian Equation
//
//	return result;
//}
//
//int random(int a, int b) //Returns a random number between a and b
//{
//	if (a == b) return a;
//	return (a < b) ? (rand() % b + a) : (rand() % a + b);
//}
//
//float calcPointDist(float x1, float y1, float x2, float y2)
//{
//	float result = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
//	return abs(result);
//}

visualMap::visualMap()
{
	Map = nullptr;
	_rendered = false;
	sizeX = 0;
	sizeY = 0;
}

visualMap::visualMap(int x, int y)
{
	sizeX = x;
	sizeY = y;

	Loc** ary = new Loc*[sizeX];
	for (int i = 0; i < sizeX; ++i)
		ary[i] = new Loc[sizeY];

	_rendered = false;
	Map = ary;
}
visualMap::~visualMap()
{
#ifdef DEBUG
	std::cout << std::endl;
	std::cout << "DEALLOCATING CHUNK" << endl;
	std::cout << std::endl;
#endif

	for (int i = 0; i < sizeX; ++i)
		delete[] Map[i];
	delete[] Map;
}

visualMap::visualMap(std::string mapName)
{
	_rendered = true;
	//load filename (mapname)

	//allocate the 2D array space and assign accordingly
}

void visualMap::DeleteMap()
{
	for (int i = 0; i < sizeX; ++i)
		delete[] Map[i];
	delete[] Map;

	_rendered = false;
}

Loc visualMap::getLocation(int x, int y) // error check beforehand 
{
	return Map[x][y];
}

void visualMap::setTileID(int x, int y, char id)
{
	if (x < sizeX && x > 0)
	{
		if (y < sizeY && y > 0)
		{
			Map[x][y].tileID = id;
		}
	}
}

void visualMap::setBiomeID(int x, int y, char id)
{
	if (x < sizeX && x > 0)
	{
		if (y < sizeY && y > 0)
		{
			Map[x][y].biomeID = id;
		}
	}
}

bool visualMap::setChunkSize(int x, int y) // NEED ERROR CHECK! 
{
	//check for min/max chunk size
	//else return false

	DeleteMap();

	//assign new size

	sizeX = x;
	sizeY = y;

	//reallocate new size

	Loc** ary = new Loc*[sizeX];
	for (int i = 0; i < sizeX; ++i)
		ary[i] = new Loc[sizeY];

	Map = ary;

	return true;
	
}

void visualMap::saveMap(std::string mapName)
{
	std::fstream mapFile(mapName + (".map"), std::ios::out | std::ios::binary);
	mapFile.write((char*)&sizeX, sizeof(int));
	mapFile.write((char*)&sizeY, sizeof(int));

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.write((char*)&Map[x][y], sizeof(Loc)); 
		}
	}
	mapFile.close();
}

//still requires blending! 
bool visualMap::RenderInitalMap()   // if returns false, then exit program  
{
	if (isRendered())  // make sure it isnt already rendered! 
	{
		return true;
	}
	if (Map == nullptr)  // make sure that map has been rendered!   HANDLE THIS DIFFERENTLY LATER
	{
		return false;
	}
	int x1, y1, x2, y2, tempInt, biomeType;
	short biomeID;
	//load the .tmt template file and load the corner and top pieces
	//FORMAT: 1st position is the cornerTemplate(TL), the rest are sideTemplates(top)
	std::fstream tmtFile("biomeTemplate.tmt", std::ios::in | std::ios::binary);
	if (tmtFile.eof())
	{
		return false;
	}
	int size = 0;
	tmtFile.read((char*)&size, sizeof(int));
	char** renderTemplates;
	renderTemplates = new char*[size];
	for (int i = 0; i < size; i++)
	{
		renderTemplates[i] = new char[100];
	}

	while (!tmtFile.eof())
	{
		for (int n = 0; n < size; n++) // number of tile templates
		{
			for (int i = 0; i < 100; i++)
			{
				tmtFile.read((char*)&renderTemplates[n][i], sizeof(char));
			}
		}
	}

	//create a new map object the same size as Map named copymap
	
	Loc** copyMap = new Loc*[sizeX];/// make sure internal LOC objects values' are all initlizaed to zero!
	for (int i = 0; i < sizeX; ++i)
		copyMap[i] = new Loc[sizeY];

	//iterate through the entire visual map (not the copy);
	for (int i = 0; i < sizeY; i++)
	{
		for (int j = 0; j < sizeX; j++)
		{
			///all changes are made in the copymap! the map itself isnt changed at all!

			if (Map[j][i].biomeID != 0) // a valid position has been found! 
			{
				//save the biome ID and the biome type(stored temp in the biomeTileID (the unsigned char)
				biomeID = Map[j][i].biomeID;
				biomeType = Map[j][i].tileID;
				
				//once a valid ID has been found it positions that dot in the center of the 100x100 biome, to moving it left 50 and up 50 (in the copymap)
				x1 = (j - 50);
				y1 = (i - 50);
				if (x1 < 10)
				{
					x1 = 10;
				}
				if (y1 < 10)
				{
					y1 = 10;
				}
				if (y1 > (sizeY - 60))
				{
					y1 = (sizeY - 60);
				}
				if (x1 > (sizeX - 60))
				{
					x1 = (sizeX - 60);
				}
				
				//use x2y2 to keep track of current position in the area
				x2 = x1;
				y2 = y1;
				/// if a char is 1(from template), it assigns a biome tile randomly. if the char is 0, then pass over the Loc object and dont change it

				//GENERATION (check if it is occupied, if it is then skip, then check the copy, if its zero, then skip, if its a 1 then calcualte the biomeTileID)
				//generate corners
				int ty = 0;
				int tx = 0;
				for (int y = y2; y < (y2 + 10); y++) //TL
				{
					for (int x = x2; x < (x2 + 10); x++)
					{
						tempInt = (ty * 10) + tx;
						if (renderTemplates[0][tempInt] == 1 && copyMap[x][y].biomeID == 0) // if the template registers this location as a point that isnt already occupied
						{
							copyMap[x][y].biomeID = biomeID;

							if (biomeType > 51) // error
							{
								return false; // there was an error in associating biome types! 
							}

							if (biomeType % 2 == 0) // even
							{
								copyMap[x][y].tileID = (biomeType * 5) + mrandom(0,4);
							}
							else
							{
								copyMap[x][y].tileID = (biomeType * 5) + 5 + mrandom(0,4);
							}
							
						}
						 tx++;//else there is no render!    ALSO TX AND TY are for the template to remain within its bounds
					}
					ty++;
					tx = 0;
				}

			
			
				x2 = (x1);    
				y2 = (y1 + 90);
				tx = 9;
				ty = 0;
				for (int y = y2; y < (y2 + 10); y++)
				{
					for (int x = x2; x < (x2 + 10); x++)//BL
					{
						tempInt = (ty * 10) + tx;
						if (renderTemplates[0][tempInt] == 1 && copyMap[x][y].biomeID == 0) // if the template registers this location as a point that isnt already occupied
						{
							copyMap[x][y].biomeID = biomeID; 

							if (biomeType > 51) // error
							{
								return false; // there was an error in associating biome types! 
							}

							if (biomeType % 2 == 0) // even
							{
								copyMap[x][y].tileID = (biomeType * 5) + mrandom(0, 4);
							}
							else
							{
								copyMap[x][y].tileID = (biomeType * 5) + 5 + mrandom(0, 4);
							}

						}
						ty++;//else there is no render!    ALSO TX AND TY are for the template to remain within its bounds
					}
					tx--;
					ty = 0;
				}
				x2 = (x1 + 90);
				y2 = (y1 + 90);
				tx = 9;
				ty = 9;
				for (int y = y2; y < (y2 + 10); y++)
				{
					for (int x = x2; x < (x2 + 10); x++)//BR
					{
						tempInt = (ty * 10) + tx;
						if (renderTemplates[0][tempInt] == 1 && copyMap[x][y].biomeID == 0) // if the template registers this location as a point that isnt already occupied
						{
							copyMap[x][y].biomeID = biomeID;

							if (biomeType > 51) // error
							{
								return false; // there was an error in associating biome types! 
							}

							if (biomeType % 2 == 0) // even
							{
								copyMap[x][y].tileID = (biomeType * 5) + mrandom(0, 4);
							}
							else
							{
								copyMap[x][y].tileID = (biomeType * 5) + 5 + mrandom(0, 4);
							}

						}
						tx--;//else there is no render!    ALSO TX AND TY are for the template to remain within its bounds
					}
					ty--;
					tx = 9;
				}
				x2 = (x1 + 90);
				y2 = (y1);
				tx = 0;
				ty = 9;
				for (int y = y2; y < (y2 + 10); y++)
				{
					for (int x = x2; x < (x2 + 10); x++)//TR
					{
						tempInt = (ty * 10) + tx;
						if (renderTemplates[0][tempInt] == 1 && copyMap[x][y].biomeID == 0) // if the template registers this location as a point that isnt already occupied
						{
							copyMap[x][y].biomeID = biomeID;

							if (biomeType > 51) // error
							{
								return false; // there was an error in associating biome types! 
							}

							if (biomeType % 2 == 0) // even
							{
								copyMap[x][y].tileID = (biomeType * 5) + mrandom(0, 4);
							}
							else
							{
								copyMap[x][y].tileID = (biomeType * 5) + 5 + mrandom(0, 4);
							}

						}
						ty--;//else there is no render!    ALSO TX AND TY are for the template to remain within its bounds
					}
					tx++;
					ty = 9;
				}
				//generate sides

				x2 = (x1 + 10);
				y2 = (y1);
				for (int top = 0; top < 8; top++) // entire top
				{
					x2 = x2 + (top * 10);
					tx = 0;
					ty = 0;
					for (int y = y2; y < (y2 + 10); y++)
					{
						for (int x = x2; x < (x2 + 10); x++)//TOP
						{
							tempInt = (ty * 10) + tx;
							if (renderTemplates[mrandom(1, (size - 1))][tempInt] == 1 && copyMap[x][y].biomeID == 0) // if the template registers this location as a point that isnt already occupied
							{
								copyMap[x][y].biomeID = biomeID;

								if (biomeType > 51) // error
								{
									return false; // there was an error in associating biome types! 
								}

								if (biomeType % 2 == 0) // even
								{
									copyMap[x][y].tileID = (biomeType * 5) + mrandom(0, 4);
								}
								else
								{
									copyMap[x][y].tileID = (biomeType * 5) + 5 + mrandom(0, 4);
								}

							}
							tx++;//else there is no render!    ALSO TX AND TY are for the template to remain within its bounds
						}
						ty++;
						tx = 0;
					}
				}
				
				x2 = (x1 + 90);
				y2 = (y1 + 10);
				
				for (int right = 0; right < 8; right++) // entire right side
				{
					y2 = y2 + (right * 10);
					tx = 0;
					ty = 9;
					for (int y = y2; y < (y2 + 10); y++)
					{
						for (int x = x2; x < (x2 + 10); x++)//right
						{
							tempInt = (ty * 10) + tx;
							if (renderTemplates[mrandom(1, (size - 1))][tempInt] == 1 && copyMap[x][y].biomeID == 0) // if the template registers this location as a point that isnt already occupied
							{
								copyMap[x][y].biomeID = biomeID;

								if (biomeType > 51) // error
								{
									return false;
								}

								if (biomeType % 2 == 0) // even
								{
									copyMap[x][y].tileID = (biomeType * 5) + mrandom(0, 4);
								}
								else
								{
									copyMap[x][y].tileID = (biomeType * 5) + 5 + mrandom(0, 4);
								}

							}
							ty--;
						}
						tx++;
						ty = 9;
					}
				}

				x2 = (x1 + 10);
				y2 = (y1 + 90);

				for (int bot = 0; bot < 8; bot++) // entire bottom side
				{
					x2 = x2 + (bot * 10);
					tx = 0;
					ty = 9;
					for (int y = y2; y < (y2 + 10); y++)
					{
						for (int x = x2; x < (x2 + 10); x++)//bot
						{
							tempInt = (ty * 10) + tx;
							if (renderTemplates[mrandom(1, (size - 1))][tempInt] == 1 && copyMap[x][y].biomeID == 0) // if the template registers this location as a point that isnt already occupied
							{
								copyMap[x][y].biomeID = biomeID;

								if (biomeType > 51) // error
								{
									return false;
								}

								if (biomeType % 2 == 0) // even
								{
									copyMap[x][y].tileID = (biomeType * 5) + mrandom(0, 4);
								}
								else
								{
									copyMap[x][y].tileID = (biomeType * 5) + 5 + mrandom(0, 4);
								}

							}
							tx++;
						}
						ty--;
						tx = 0;
					}
				}
				x2 = (x1);
				y2 = (y1 + 10);

				for (int left = 0; left < 8; left++) // entire left side
				{
					y2 = y2 + (left * 10);
					tx = 0;
					ty = 0;
					for (int y = y2; y < (y2 + 10); y++)
					{
						for (int x = x2; x < (x2 + 10); x++)//left
						{
							tempInt = (ty * 10) + tx;
							if (renderTemplates[mrandom(1, (size - 1))][tempInt] == 1 && copyMap[x][y].biomeID == 0) // if the template registers this location as a point that isnt already occupied
							{
								copyMap[x][y].biomeID = biomeID;

								if (biomeType > 51) // error
								{
									return false;
								}

								if (biomeType % 2 == 0) // even
								{
									copyMap[x][y].tileID = (biomeType * 5) + mrandom(0, 4);
								}
								else
								{
									copyMap[x][y].tileID = (biomeType * 5) + 5 + mrandom(0, 4);
								}

							}
							ty++;
						}
						tx++;
						ty = 0;
					}
				}
				//generate interior
				x2 = (x1 + 10);
				y2 = (y1 + 10);

				int finX, finY;

				for (int y = y2; y < (y1 + 90); y++)
				{
					for (int x = x2; x < (x1 + 90); x++)//inside
					{
						finX = x;
						finY = y;

						if (finX >= sizeX)
						{
							finX = sizeX - 1;
						}
						if (finY >= sizeY)
						{
							finY = sizeY - 1;
						}
						
						if (copyMap[finX][finY].biomeID == 0) // if the template registers this location as a point that isnt already occupied
						{
							copyMap[finX][finY].biomeID = biomeID;

							if (biomeType > 51) // error
							{
								return false;
							}

							if (biomeType % 2 == 0) // even
							{
								copyMap[finX][finY].tileID = (biomeType * 5) + mrandom(0, 4);
							}
							else
							{
								copyMap[finX][finY].tileID = (biomeType * 5) + 5 + mrandom(0, 4);
							}

						}
					}
				}
				}//end of the biome creation
			}
		}

		///biomes beyond this point should ALL be rendered but not blended
	
	//once map has been looked through and copymap is done, perform the blending/empty space funcitons


		//delete the current Map
		DeleteMap();

	//rearrange pointer of this->map to point to copyMap's map
		Map = copyMap;

	//save map  (int)sizeX (int)sizeY  then the map L-R  T-B
		saveMap("visual");

		//CHUNK MIGht be added in later   100mb isnt a TON ...
		//set new chunk holder size to be 4* the chunkX and chunkY
		//setChunkSize(CHUNK_X * 4, CHUNK_Y * 4);

	_rendered = true;     //so the render function cannot be called again
	return true;
}

//couts position x,y in RECT
void visualMap::displaySqare(int x, int y, MYRECT viewBox)  
{
	int finalX, finalY;

	for (int i = y; i < y + viewBox.tall; i++)
	{
		if (i > sizeY)
		{
			std::cout << std::endl;
		}
		else
		{
			for (int j = x; j < x + viewBox.wide; j++)
			{
				finalX = j;
				finalY = i;
				if (finalX > sizeX)
				{
					std::cout << " ";
				}
				else
				{
					std::cout << (Map[finalX][finalY].tileID + 33); // the 33 offsets the char to be a readable symbol
				}

			}
			std::cout << std::endl;
		}
		
		
	}

}


GameMap::GameMap()
{
	_sizeX = MAP_SIZEX;
	_sizeY = MAP_SIZEY;

	_refWorld = new World(true); // add method for determining # of biomes in the world based on size! 

	_visualMap = nullptr;

	_worSizeX = WORLD_SIZEX;
	_worSizeY = WORLD_SIZEY;

	//determine world coordinates
	_worldX = mrandom(0, _worSizeX - _sizeX);
	_worldY = mrandom(0, _worSizeY - _sizeY);

	_biomeCount = ((_sizeX / 100) * (_sizeY / 100)); 
}

GameMap::GameMap(std::string filename) // NEEDS COMPLETION
{
	//load the selected filename (after checking its existance)

	//load mapsize 
	//load world
	//load visualmap
	_visualMap = new visualMap(filename);
	//...
	 
}

GameMap::~GameMap()
{
	//check if saved!
	// delete all the components! 
	delete _visualMap;
}


std::vector<BiomeInfo>* GameMap::loadResourceInfo()
{
	int count = 0;
	int strSize;
	char* charList;
	BiomeInfo temp;

	std::ifstream InFile;
	InFile.open("biomeStats.bin", std::ios::in | std::ios::binary);

	InFile.read((char*)&count, sizeof(int));


	std::vector<BiomeInfo> *myArray = new std::vector<BiomeInfo>;

	for (int i = 0; i < count; i++)  // you have to read as a char*[] then set to a string(char*[], size)
	{

		InFile.read((char*)&strSize, sizeof(int));
		charList = new char[strSize];

		for (int len = 0; len < strSize; len++)
		{
			InFile.read(&charList[len], sizeof(char));
		}
		std::string ts(charList, strSize);
		temp.type = ts;

		for (int re = 0; re < 5; re++)
		{
			delete[] charList;
			InFile.read((char*)&strSize, sizeof(int));
			charList = new char[strSize];

			for (int len = 0; len < strSize; len++)
			{
				InFile.read(&charList[len], sizeof(char));
			}
			std::string tempString(charList, strSize);
			temp.sources[re].name = tempString;


			InFile.read((char*)&temp.sources[re].currentCount, sizeof(int));
			InFile.read((char*)&temp.sources[re].replenishRate, sizeof(int));
			InFile.read((char*)&temp.sources[re].max, sizeof(int));
		}
		delete[] charList;
		myArray->push_back(temp);
	}

	InFile.close();

	return myArray;



}

void GameMap::DetermineBiome(Biome& b, std::vector<BiomeInfo>* referenceVector)  // assume passed biome coords are within the map boundries!
{
	int bx, by, wx, wy, tempCoord;
	//set biome temperature
	bx = b.getX();
	by = b.getY();
	//set biome temperature to the gaussian curve range
	//convert to world coordninates
	wx = _worldX + bx;
	wy = _worldY + by;

	//convert world coord to gaussian range
	if (wy > WORLD_SIZEY / 2)
	{
		tempCoord = (WORLD_SIZEY / 2) - (wy - (WORLD_SIZEY / 2));
	}
	else
	{
		tempCoord = wy;
	}

	//use gaussian dist. use the y for distance from AVERAGE TEMP. (so in this case x on the curve is y on the map)
	float chance = 0.0f;

	chance = mGaussCurve(tempCoord, 1, 1200, 400);   // based on the 8k 4k world
	//chance = mGaussCurve(tempCoord, 16, 1200, 400)

	//change that chance into a difference
	float tempDiff = (float)AVERAGETEMP * (1.0f - chance) - (3.0f + rand() % 6);  // Gives the temp a range between -3 and +3 of result
	//determine its position in map


	if (wy > _worSizeY*0.75f || wy < _worSizeY*0.25f)		//--------- the minus represents the colder parts and the plus represents the warmer
		b.setTemperature((float)AVERAGETEMP - tempDiff);				//+++++++++
	else if (wy < _worSizeY*0.75f || wy > _worSizeY*0.25f)	//+++++++++
		b.setTemperature((float)AVERAGETEMP + tempDiff);				//---------
	else
		b.setTemperature((float)AVERAGETEMP);

	//set biome moisture
	float moisturePercentage = 0.0f;

	//for (int i = 0; i < _waterBiomes; i++) // check for moisture collisions at a later time 
	//{
	//
	//
	//		In here I will use the formula for a circle in order to determine how many circles the biome is in, then do 
	//		what I did below but just add a % of the other moisture to the final one
	//
	//
	//
	//
	//	
	//}

	//set biomes moisture %
	int closest = 0;
	float current = 0.0f, clo = 0.0f;

	for (int i = 0; i < _refWorld->_waterBiomes; i++)
	{
		current = mcalcPointDist( (float)wx, (float)wy, (float)_refWorld->wb[i]->getX(), (float)_refWorld->wb[i]->getY());

		if (current < clo || clo == 0)
		{
			clo = current;
			closest = i;
		}
	}
	moisturePercentage = (float)(1.0f - (mcalcPointDist((float)wx, (float)wy, (float)_refWorld->wb[closest]->getX(), (float)_refWorld->wb[closest]->getY() / (float)_refWorld->wb[closest]->getR())))*100;
	if (moisturePercentage < 0)
		moisturePercentage = 0;

	b.setMoisture(moisturePercentage);  // setting the %

	//set biome elevation

	//what this will do is create circles and have elevation be a random height at center and have it slowly decrease back to sea level 
	// these are mountian biomes that will have a biome number as well ( maybe prefixed differently?) so whenthe player is nearby it will load that mountian biome and refer to 
	//its height equasion 

	//THIS CAN BE IMPLEMENTED AT ANY TIME 

	//elevation will only change the temerature and maybe moisture (this will effect the actual biome accordingly)


	//consult chart   (the biome triangle)



	if (b.getTemp() <= 20 && b.getTemp() >= 14) // most common zone
	{
		if (moisturePercentage < 20)
		{
			b.setResources(referenceVector->at(COLDDESERT));
			b.setBiomeType(COLDDESERT);
			
		}
		if (moisturePercentage > 55)
		{
			b.setResources(referenceVector->at(TEMPDECFOREST));
			b.setBiomeType(TEMPDECFOREST);
		}
		else // in between
		{
			b.setResources(referenceVector->at(PRARIE));
			b.setBiomeType(PRARIE);
		}	
	}

	else if (b.getTemp() > 20 && b.getTemp() <= 40) //hotter
	{
		if (moisturePercentage > 80)
		{
			b.setResources(referenceVector->at(TROPRAINFOREST));
			b.setBiomeType(TROPRAINFOREST);
		}
		else if (moisturePercentage > 65)
		{
			b.setResources(referenceVector->at(TROPDECFOREST));
			b.setBiomeType(TROPDECFOREST);
		}
		else if (moisturePercentage > 45)
		{
			b.setResources(referenceVector->at(SAVANNA));
			b.setBiomeType(SAVANNA);
		}
		else if (moisturePercentage > 25)
		{
			b.setResources(referenceVector->at(TROPGRASSLAND));
			b.setBiomeType(TROPGRASSLAND);
		}
		else
		{
			b.setResources(referenceVector->at(WARMDESERT));
			b.setBiomeType(WARMDESERT);
		}
	}

	else if (b.getTemp() < 14 && b.getTemp() >= 5)//colder
	{
		if (moisturePercentage > 70)
		{
			b.setResources(referenceVector->at(HEAVYSNOWFOREST));
			b.setBiomeType(HEAVYSNOWFOREST);
		}
		else if (moisturePercentage < 30)
		{
			b.setResources(referenceVector->at(BOREALFOREST));
			b.setBiomeType(BOREALFOREST);
		}
		else
		{
			b.setResources(referenceVector->at(BARREN));
			b.setBiomeType(BARREN);
		}
	}

	else if (b.getTemp() < 5 && b.getTemp() >= 0.15)// very cold
	{
		b.setResources(referenceVector->at(TUNDRA));
		b.setBiomeType(TUNDRA);
		//OTHER BIOMES TO BE ADDED LATER:: POSSIBLY
	}

	else if (b.getTemp() < 0.15) //coldest
	{
		b.setResources(referenceVector->at(POLAR));
		b.setBiomeType(POLAR);
	}

	else if (b.getTemp() > 40) // very hot
	{
		b.setResources(referenceVector->at(BARREN));  // TO BE CHANGED TO VOLCANIC
		b.setBiomeType(BARREN);
	}
	//Biome is fully determined
}


bool GameMap::CreateBiomeFile()   // ADD ERROR HANDLING        ////ADD NEW RANDOM GENERATOR METHOD! PROBABLY THE 2D ARRAY METHOD
{
	//std::string convert = "\\maps\\" + _mapName;
	
	//std::wstring stemp = std::wstring(convert.begin(), convert.end());
	//LPCWSTR sw = stemp.c_str();

	//CreateDirectory(sw, NULL);
	std::string path = "maps\\biomeList.biome";

	std::fstream biomeFile(path, std::ios::out | std::ios::binary);

	if (!biomeFile.is_open())
	{
		return false;
	}

	//std::random_device rd;
	//std::mt19937 gen(rd());
	//std::uniform_int_distribution<> disx(15, _sizeX);       //uniformly creates biomes
	//std::uniform_int_distribution<> disy(15, _sizeY);
	
	int arrX(0), arrY(0);

	arrX = (_sizeX / 100);
	arrY = (_sizeY / 100);

	cartPair** coordZones = new cartPair*[arrX];
	for (int i = 0; i < arrX; i++)
	{
		coordZones[i] = new cartPair[arrY];
	}

	cartPair bLoc;
	
	for (int x = 0; x < arrX; x++)
	{
		for (int y = 0; y < arrY; y++)
		{
			coordZones[x][y].x = 50;
			coordZones[x][y].y = 50;
		}
	}
	for (int x = 0; x < arrX; x++)
	{
		for (int y = 0; y < arrY; y++)
		{
			coordZones[x][y].x += (100 * x);
			coordZones[x][y].y += (100 * y);
		}
	}

	Biome* b;

	int x(0), y(0) , tx(0), ty(0), ID(1);

	std::vector<BiomeInfo>* refBiomes = loadResourceInfo(); /// Make sure the biomeStats.bin file is in the right place! 

	for (int i = 0; i < arrX; i++)
	{

		for (int j = 0; j < arrY; j++)
		{
			x = coordZones[i][j].x;
			y = coordZones[i][j].y;

			tx = mrandom((x - 25), (x + 25));
			ty = mrandom((y - 25), (y + 25));

			//check occupancy in world
			if (_refWorld->checkOccupancy(x, y))
			{
				tx = (x - 1);
				ty = (y - 1);
			}


			b = new Biome(tx, ty, ID);    //(Creates a psudo-random point biome with respect to the map)

			_visualMap->setBiomeID(tx, ty, ID);  // i+1 because 0 is reserved for empty! 




			DetermineBiome(*b, refBiomes); // gains access to world info using private game map members

			_visualMap->setTileID(tx, ty, b->FindTileID());    //BIOMETYPE is tile ID! keep this, as it is used in rendering the map!  


															 //biome write

															 //write the number of biomes

			//int strSize = 0;
			//short idTemp = 0;
			//strSize = b->getName().size();
			//idTemp = b->getID();
			//biomeFile.write((char*)&idTemp, sizeof(short));
			//
			//biomeFile.write((char*)&strSize, sizeof(int));
			//biomeFile.write(b->getName().c_str(), strSize);
			//
			//strSize = b->getBiomeType();
			//
			//biomeFile.write((char*)&strSize, sizeof(int));
			//
			//strSize = b->getX();
			//biomeFile.write((char*)&strSize, sizeof(int));
			//strSize = b->getY();
			//biomeFile.write((char*)&strSize, sizeof(int));
			//
			//float tempFloat = b->getTemp();
			//biomeFile.write((char*)&tempFloat, sizeof(float));
			//tempFloat = b->getMoist();
			//biomeFile.write((char*)&tempFloat, sizeof(float));
			////biome resource write
			//
			//strSize = b->getResources().type.size();
			//biomeFile.write((char*)&strSize, sizeof(int));
			//biomeFile.write(b->getResources().type.c_str(), strSize);   // string and size of string
			//
			//for (int re = 0; re < 5; re++) // each of the 5 contained resources
			//{
			//	strSize = b->getResources().sources[re].name.size();
			//	biomeFile.write((char*)&strSize, sizeof(int));
			//
			//	biomeFile.write(b->getResources().sources[re].name.c_str(), b->getResources().sources[re].name.size());
			//
			//	biomeFile.write((char *)&b->getResources().sources[re].currentCount, sizeof(int));
			//	biomeFile.write((char *)&b->getResources().sources[re].replenishRate, sizeof(float));
			//	biomeFile.write((char *)&b->getResources().sources[re].max, sizeof(int));
			//}

			if (!b->SaveBiome(biomeFile))  // BIOME FILE NOTSAVED CORRECTLY (IF FALSE RETURN)
			{
				return false;
			}

			ID++;
			delete b;


		}

		
	}

	delete refBiomes;
	biomeFile.close();
	
	return true;
}

bool GameMap::CreateNewMap(std::string mapName) // maybe start using shared pointers for engine? 
{
	_mapName = mapName;

	///VISUAL MAP might need to be created outside the GameMapClass OR have the engine gain access to it 
	_visualMap = new visualMap(_sizeX,_sizeY); // creates the chunk to be entire map size for rendering purpose
	if (!CreateBiomeFile())//creates biomes and saves them to file
	{
		std::cout << "FILE NOT SUCCESSFULLY CREATED!" << std::endl;
		return false; 
	}

	if (!_visualMap->RenderInitalMap())
	{
		std::cout << "MAP NOT SUCCESSFULLY RENDERED!" << std::endl;
		return false;
	}

	//_visualMap->BlendMap();

	_visualMap->saveMap(_mapName);

	return true;
}

void GameMap::displayMap(int x, int y, MYRECT um)
{
	_visualMap->displaySqare(x, y, um);
}

void GameMap::displayWorldCoords()
{
	std::cout << _worldX << " , " << _worldY << std::endl;
}