

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include "wesmap.hpp"
#include "mapdiff.hpp"

using namespace std;

int main(int argc, char* argv[])//mapdiffer(string argument)
{
	/*
	string firstmap, secondmap, filename;
	parseCommas(argument, firstmap, secondmap, filename);
	*/

	if (argc != 4) {
		cout << "Usage: mapdiff 'first map path' 'second map path' 'output file name'" << endl;
		return 0;
	}

	string firstmap(argv[1]);
	string secondmap(argv[2]);
	string filename(argv[3]);

	wesmap mapA(firstmap);
	wesmap mapB(secondmap);

	int numRows = max(mapA.getNumRows(), mapB.getNumRows());
	int numCols = max(mapA.getNumCols(), mapB.getNumCols());
	westile voidTile("Xv");

	//mapA.resizeNew(1, -1, numRows, numCols, voidTile);

	/*
	if (mapA.getNumRows() != numRows) {
		mapA.resize(numRows, mapA.getNumCols(), voidTile);
	}
	if (mapB.getNumRows() != numRows) {
		mapB.resize(numRows, mapB.getNumCols(), voidTile);
	}

	if (mapA.getNumCols() != numCols) {
		mapA.resize(mapA.getNumRows(), numCols, voidTile);
	}
	if (mapB.getNumCols() != numCols) {
		mapB.resize(mapB.getNumRows(), numCols, voidTile);
	}*/

	/* Old output map
	wesmap O(numRows, numCols, voidTile);
	O.setBorderSize(1);
	O.setUsage("map");  //OutputMap is filler
	*/

	/*
	cout << mapA.getNumRows() << " " << mapA.getNumCols() << endl;
	cout << mapB.getNumRows() << " " << mapB.getNumCols() << endl;
	cout << O.getNumRows() << " " << O.getNumCols() << endl;
	cout << "max" << numRows << " " << numCols << endl;
	*/
	mapB.resizeNew(0, 3, numRows, numCols, voidTile);
	mapA.resizeNew(0, 0, numRows, numCols, voidTile);

	for (int rowIndex = 0; rowIndex < numRows; rowIndex++) {
		for (int colIndex = 0; colIndex < numCols; colIndex++) {
			//cout << mapA.getTile(rowIndex, colIndex).bgCode << " " << mapB.getTile(rowIndex, colIndex).bgCode << endl;
			cout << "I am before getTile" << endl;
			if (mapA.getTile(rowIndex, colIndex) == mapB.getTile(rowIndex, colIndex)){
				cout << "I am in getTile" << endl;
				//O.setTile(rowIndex, colIndex, voidTile);
				
			}
			else {
				//cout << "Difference Found" << endl;
				//O.setTile(rowIndex, colIndex, mapB.getTile(rowIndex, colIndex));
				mapB.setLabel(rowIndex, colIndex, mapA.getTile(rowIndex, colIndex).name);
			}
		}
	}

	mapB.writeScenarioMap(filename,secondmap); //mapName is filler
	//mapB.writeMap(filename);
	return 0;
}

void parseCommas(std::string input, std::string & originalMap,
	std::string & newMap, std::string & diffMap)
{
	stringstream source(input);
	std::string junkTest;
	getline(source, originalMap, ',');
	getline(source, newMap, ',');
	getline(source, diffMap, ',');
	if (getline(source, junkTest, ','))
	{
		//error checking goes here, too many arguments
	}
}