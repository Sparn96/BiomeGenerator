#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include "biome.h"

using namespace std;

//biomeList.biome

const int BIOME_COUNT = 700;

void loadDataset(vector<Biome*>&);
int searchBiomeType(int biomeType, vector<Biome*>);
float averageTemperature(int biomeType, bool all, vector<Biome*>);
float aveMoist(vector<Biome*>);
void displayCoords(vector<Biome*>);

int main()
{
	vector<Biome*> myDataset;

	loadDataset(myDataset);

	cout << "Total Number of Biomes is "<< BIOME_COUNT << endl;
	cout << endl;
	cout << "0  Barren:      " << setw(3) << searchBiomeType(BARREN, myDataset) << setw(6) << setprecision(4) << averageTemperature(BARREN, false, myDataset) << endl;
	cout << "1  Temp Dec F:  " << setw(3) << searchBiomeType(TEMPDECFOREST, myDataset) << setw(6) << setprecision(4) << averageTemperature(TEMPDECFOREST, false, myDataset) << endl;
	cout << "2  Prarie:      " << setw(3) << searchBiomeType(PRARIE, myDataset) << setw(6) << setprecision(4) << averageTemperature(PRARIE, false, myDataset) << endl;
	cout << "3  Boreal F:    " << setw(3) << searchBiomeType(BOREALFOREST, myDataset) << setw(6) << setprecision(4) << averageTemperature(BOREALFOREST, false, myDataset) << endl;
	cout << "4  Tundra:      " << setw(3) << searchBiomeType(TUNDRA, myDataset) << setw(6) << setprecision(4) << averageTemperature(TUNDRA, false, myDataset) << endl;
	cout << "5  Polar:       " << setw(3) << searchBiomeType(POLAR, myDataset) << setw(6) << setprecision(4) << averageTemperature(POLAR, false, myDataset) << endl;
	cout << "6  Warm Desert: " << setw(3) << searchBiomeType(WARMDESERT, myDataset) << setw(6) << setprecision(4) << averageTemperature(WARMDESERT, false, myDataset) << endl;
	cout << "7  Trop Grass:  " << setw(3) << searchBiomeType(TROPGRASSLAND, myDataset) << setw(6) << setprecision(4) << averageTemperature(TROPGRASSLAND, false, myDataset) << endl;
	cout << "8  Cold Desert: " << setw(3) << searchBiomeType(COLDDESERT, myDataset) << setw(6) << setprecision(4) << averageTemperature(COLDDESERT, false, myDataset) << endl;
	cout << "9  Savanna:     " << setw(3) << searchBiomeType(SAVANNA, myDataset) << setw(6) << setprecision(4) << averageTemperature(SAVANNA, false, myDataset) << endl;
	cout << "10 Trop Dec F:  " << setw(3) << searchBiomeType(TROPDECFOREST, myDataset) << setw(6) << setprecision(4) << averageTemperature(TROPDECFOREST, false, myDataset) << endl;
	cout << "11 Trop Rain F: " << setw(3) << searchBiomeType(TROPRAINFOREST, myDataset) << setw(6) << setprecision(4) << averageTemperature(TROPRAINFOREST, false, myDataset) << endl;
	cout << "12 Heavy SnowF: " << setw(3) << searchBiomeType(HEAVYSNOWFOREST, myDataset) << setw(6) << setprecision(4) << averageTemperature(HEAVYSNOWFOREST, false, myDataset) << endl;
	cout << endl << endl;
	cout << "Average Map Temp:     " << setprecision(6) << averageTemperature(0, true, myDataset) << endl;
	cout << "Average Map Moisture: " << setprecision(6) << aveMoist(myDataset) << endl;

	cout << endl << endl;
	cout << "The corresponding coordinates of each biome (in order of creation)" << endl << endl;
	displayCoords(myDataset);

	system("PAUSE");





	return 0;
}

void loadDataset(vector<Biome*>& myData)
{
	vector<BiomeInfo>* myArray = new vector<BiomeInfo>;
	
	int strSize;
	char* charList;

	Biome finBiome;
	short a_id;
	int a_biomeType;
	int a_x;
	int a_y;
	float a_temperature;
	float a_moisture;
	BiomeInfo Infotemp;

	fstream biomeFile("biomeList.biome", ios::in | ios::binary);

	for (int i = 0; i < BIOME_COUNT; i++) // 200 is the small map biome count 
	{
		

		biomeFile.read((char*)&a_id, sizeof(short));
		biomeFile.read((char*)&strSize, sizeof(int));
		charList = new char[strSize];
		for (int len = 0; len < strSize; len++) //name;
		{
			biomeFile.read(&charList[len], sizeof(char));
		}
		string a_name(charList,strSize);
		biomeFile.read((char*)&a_biomeType, sizeof(int));
		biomeFile.read((char*)&a_x, sizeof(int));
		biomeFile.read((char*)&a_y, sizeof(int));
		biomeFile.read((char*)&a_temperature, sizeof(float));
		biomeFile.read((char*)&a_moisture, sizeof(float));
		delete[] charList;
		//load the Biome info

		biomeFile.read((char*)&strSize, sizeof(int));
		charList = new char[strSize];
		for (int len = 0; len < strSize; len++) //name;
		{
			biomeFile.read(&charList[len], sizeof(char));
		}
		string GGG(charList, strSize);
		Infotemp.type = GGG;

		for (int re = 0; re < 5; re++)
		{
			delete[] charList;
			biomeFile.read((char*)&strSize, sizeof(int));
			charList = new char[strSize];

			for (int len = 0; len < strSize; len++)
			{
				biomeFile.read(&charList[len], sizeof(char));
			}
			string tempString(charList, strSize);
			Infotemp.sources[re].name = tempString;


			biomeFile.read((char*)&Infotemp.sources[re].currentCount, sizeof(int));
			biomeFile.read((char*)&Infotemp.sources[re].replenishRate, sizeof(int));
			biomeFile.read((char*)&Infotemp.sources[re].max, sizeof(int));
		}
		delete[] charList;

		finBiome._name = a_name;
		finBiome._biomeType = a_biomeType;
		finBiome._id = a_id;
		finBiome._moisture = a_moisture;
		finBiome._temperature = a_temperature;
		finBiome._resources = Infotemp;
		finBiome._x = a_x;
		finBiome._y = a_y;

		myData.push_back(new Biome(finBiome));  // copy constructor
		
	}

}

int searchBiomeType(int biomeType, vector<Biome*> vec)
{
	int result = 0;
	for (auto i : vec)
	{
		if (i->_biomeType == biomeType)
		{
			result++;
		}
	}

	return result;
}

float averageTemperature(int biomeType, bool all, vector<Biome*> vec)
{
	float total = 0.0f;
	int count = 0;

	for (auto i : vec)
	{
		if (all == true)
		{
			total = total + (i->_temperature);
			count++;
		}
		else if(i->_biomeType == biomeType)
		{
			total = total + (i->_temperature);
			count++;
		}
	}

	if (count == 0)
	{
		return 0.0f;
	}

	return (total / count);

}

float aveMoist(vector<Biome*> vec)
{
	float total = 0.0f;
	int count = 0;

	for (auto i : vec)
	{
		total = total + (i->_moisture);
		count++;
	}

	return (total / count);
}

void displayCoords(vector<Biome*> vec)
{
	for (auto i : vec)
	{
		cout << "(" << setw(4) << i->_x << "," << setw(4) << i->_y << ") " << i->_biomeType << endl;
	}
}