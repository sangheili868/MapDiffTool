

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include "wesmap.hpp"
#include "mapdiff.hpp"

using namespace std;

bool mapdiffer(string argument)
{
	string firstmap, secondmap, filename;
	parseCommas(argument, firstmap, secondmap, filename);
	wesmap mapA(firstmap);
	wesmap mapB(secondmap);

	int numRows = max(mapA.getNumRows(), mapB.getNumRows());
	int numCols = max(mapA.getNumCols(), mapB.getNumCols());
	westile voidTile("Xv");

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
	}

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

	for (int rowIndex = 0; rowIndex < numRows; rowIndex++) {
		for (int colIndex = 0; colIndex < numCols; colIndex++) {
			//cout << mapA.getTile(rowIndex, colIndex).bgCode << " " << mapB.getTile(rowIndex, colIndex).bgCode << endl;
			if (mapA.getTile(rowIndex, colIndex) == mapB.getTile(rowIndex, colIndex)){
				//O.setTile(rowIndex, colIndex, voidTile);
				
			}
			else {
				//cout << "Difference Found" << endl;
				//O.setTile(rowIndex, colIndex, mapB.getTile(rowIndex, colIndex));
				mapB.setLabel(rowIndex, colIndex, mapB.getTile(rowIndex, colIndex));
			}
		}
	}

	mapB.writeScenarioMap(filename,secondmap); //mapName is filler
	return true;
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