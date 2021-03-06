

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

	// Old output map
	wesmap O(numRows, numCols, voidTile);
	O.setBorderSize(1);
	O.setUsage("map");  //OutputMap is filler
	

	/*
	cout << mapA.getNumRows() << " " << mapA.getNumCols() << endl;
	cout << mapB.getNumRows() << " " << mapB.getNumCols() << endl;
	cout << O.getNumRows() << " " << O.getNumCols() << endl;
	cout << "max" << numRows << " " << numCols << endl;
	*/
	//mapB.resizeNew(5, 2, numRows, numCols, voidTile);
	//mapA.resizeNew(8, 3, numRows, numCols, voidTile);
	
	getDiff_optimized(mapA, mapB, O);

	mapA.setBorderSize(1);
	mapA.setUsage("map");  //OutputMap is filler
	mapA.writeMap("mapA.map");
	
	
	mapB.setBorderSize(1);
	mapB.setUsage("map");  //OutputMap is filler
	mapB.writeMap("mapB.map");
	

//JACOB INSERTED
	{
		//westile voidTile("Xv");
		//int numDiffs = 0;
		for (int rowIndex = 0; rowIndex < mapA.getNumRows(); rowIndex++) {
			for (int colIndex = 0; colIndex < mapA.getNumCols(); colIndex++) {
				//cout << mapA.getTile(rowIndex, colIndex).bgCode << " " << mapB.getTile(rowIndex, colIndex).bgCode << endl;
				if (mapA.getTile(rowIndex, colIndex) == mapB.getTile(rowIndex, colIndex)){
					//mapOutput.setTile(rowIndex, colIndex, voidTile);
				}
				else {
					//cout << "Difference Found" << endl;
					//mapOutput.setTile(rowIndex, colIndex, mapB.getTile(rowIndex, colIndex));
					//numDiffs++;
					mapB.setLabel(rowIndex, colIndex, mapA.getTile(rowIndex, colIndex).name);
				}
			}
		}
		//return numDiffs;
	}
//JACOB INSERTED
	/*
	for (int rowIndex = 0; rowIndex < numRows; rowIndex++) {
		for (int colIndex = 0; colIndex < numCols; colIndex++) {
			//cout << mapA.getTile(rowIndex, colIndex).bgCode << " " << mapB.getTile(rowIndex, colIndex).bgCode << endl;
			//cout << "I am before getTile" << endl;
			if (mapA.getTile(rowIndex, colIndex) == mapB.getTile(rowIndex, colIndex)){
				//cout << "I am in getTile" << endl;
				//O.setTile(rowIndex, colIndex, voidTile);
				
			}
			else {
				//cout << "Difference Found" << endl;
				//O.setTile(rowIndex, colIndex, mapB.getTile(rowIndex, colIndex));
				mapB.setLabel(rowIndex, colIndex, mapA.getTile(rowIndex, colIndex).name);
			}
		}
	}
	*/
	
	
	//O.writeMap(filename);
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
//All three maps MUST be the same size.
//returns number of differences.
//writes to output map.
int mapDiff_func(const wesmap & mapA, const wesmap & mapB, wesmap & mapOutput)
{
	westile voidTile("Xv");
	int numDiffs = 0;
	for (int rowIndex = 0; rowIndex < mapA.getNumRows(); rowIndex++) {
		for (int colIndex = 0; colIndex < mapA.getNumCols(); colIndex++) {
			//cout << mapA.getTile(rowIndex, colIndex).bgCode << " " << mapB.getTile(rowIndex, colIndex).bgCode << endl;
			if (mapA.getTile(rowIndex, colIndex) == mapB.getTile(rowIndex, colIndex)){
				mapOutput.setTile(rowIndex, colIndex, voidTile);
			}
			else {
				//cout << "Difference Found" << endl;
				mapOutput.setTile(rowIndex, colIndex, mapB.getTile(rowIndex, colIndex));
				numDiffs++;
			}
		}
	}
	return numDiffs;
}

//Maps need not be the same size.
//mapOutput must have as many rows as max(rows in A, rows in B)
//and as many columns as max(cols in A, cols in b)
void getDiff(const wesmap & mapA, const wesmap & mapB, wesmap & mapOutput)
{
	wesmap tempA = mapA;
	wesmap tempB = mapB;
	westile voidTile("Xv");
	int rowNow = 0, colNow = 0;
	int bestPosition[3] = { 2000000000, 0, 0 };
	int toCompare = 0;
	int rowsA, rowsB, colsA, colsB;
	rowsA = mapA.getNumRows(); rowsB = mapB.getNumRows();
	colsA = mapA.getNumCols(); colsB = mapB.getNumCols();

	if (rowsA != rowsB)
	{
		if (rowsA < rowsB)
		{
			for (int rowNow = 0; rowNow < rowsB - rowsA; rowNow++)
			{
				tempA = mapA;
				tempA.resizeNew(rowNow, colNow, mapOutput.getNumRows(), mapOutput.getNumCols(), voidTile);
				getDiff_helper_cols(tempA, mapB, mapOutput, rowNow, bestPosition);
			}
		}
		else
		{
			for (int rowNow = 0; rowNow < rowsA - rowsB; rowNow++)
			{
				tempB = mapB;
				tempB.resizeNew(rowNow, colNow, mapOutput.getNumRows(), mapOutput.getNumCols(), voidTile);
				getDiff_helper_cols(mapA, tempB, mapOutput, rowNow, bestPosition);
			}
		}
	}
	tempA = mapA;
	tempB = mapB;
	//found best resize, now do it.
	if (rowsA != rowsB)
	{
		if (rowsA < rowsB)
		{
			tempA.resizeNew(bestPosition[1], 0, mapOutput.getNumRows(), mapOutput.getNumCols(), voidTile);
			if (colsA < colsB)
			{
				tempA.resizeNew(0, bestPosition[2], mapOutput.getNumRows(), mapOutput.getNumCols(), voidTile);
			}
			else
			{
				tempB.resizeNew(0, bestPosition[2], mapOutput.getNumRows(), mapOutput.getNumCols(), voidTile);
			}
		}
		else
		{
			tempB.resizeNew(bestPosition[1], 0, mapOutput.getNumRows(), mapOutput.getNumCols(), voidTile);
			if (colsA < colsB)
			{
				tempA.resizeNew(0, bestPosition[2], mapOutput.getNumRows(), mapOutput.getNumCols(), voidTile);
			}
			else
			{
				tempB.resizeNew(0, bestPosition[2], mapOutput.getNumRows(), mapOutput.getNumCols(), voidTile);
			}
		}
	}
	mapDiff_func(mapA, mapB, mapOutput);
}

//Maps need not be the same size.
//mapOutput must have as many rows as max(rows in A, rows in B)
//and as many columns as max(cols in A, cols in b)
//Note: does modify mapA and mapB to have the same dimensions as mapOutput
void getDiff_optimized(wesmap & mapA, wesmap & mapB, wesmap & mapOutput)
{
	westile voidTile("Xv");
	int numRowsA, numRowsB, numColsA, numColsB;
	numRowsA = mapA.getNumRows(); numRowsB = mapB.getNumRows();
	numColsA = mapA.getNumCols(); numColsB = mapB.getNumCols();
	int bestPosition[5] = { 2000000, 0, 0};
	int rowA, rowB, colA, colB;
	//int rowB = 0;
	//for (int rowA = 0; ((rowA < numRowsA - numRowsB) || (rowA < numRowsB - numRowsA)) && ((rowB < numRowsA - numRowsB) || (rowB < numRowsB - numRowsA)); rowA++, rowB++)
	for (int row = 0; row < abs(numRowsA - numRowsB); row++)
	{
		for (int col = 0; col < abs(numColsA - numColsB); col++)
		{
			if (numRowsA < numRowsB) {
				rowA = row; rowB = 0;
			}
			else if (numRowsA > numRowsB) {
				rowA = 0; rowB = row;
			}
			else {
				rowA = 0; rowB = 0;
			}
			if (numColsA < numColsB) {
				colA = col;	colB = 0;
			}
			else if (numColsA > numColsB) {
				colA = 0; colB = col;
			}
			else {
				colA = 0; colB = 0;
			}
			int toCompare = newDiff(mapA, rowA, colA, mapB, rowB, colB);
			if (toCompare < bestPosition[0])
			{
				bestPosition[0] = toCompare;
				bestPosition[1] = row;
				bestPosition[2] = col;
				/*bestPosition[1] = rowA;
				bestPosition[2] = rowB;
				bestPosition[3] = colA;
				bestPosition[4] = colB;*/
			}
		}
	}
	
	//found best resize, now do it.
	if (numRowsA != numRowsB)
	{
		if (numRowsA < numRowsB)
		{
			cout << "mapA.resizeNew(" << bestPosition[1] <<", 0, mapOutput.getNumRows(), mapOutput.getNumCols(), voidTile);" << endl;
			mapA.resizeNew(bestPosition[1], 0, mapOutput.getNumRows(), mapOutput.getNumCols(), voidTile);
			if (numColsA < numColsB)
			{
				cout << "mapA.resizeNew(0, " << bestPosition[2] <<", mapOutput.getNumRows(), mapOutput.getNumCols(), voidTile);" << endl;
				mapA.resizeNew(0, bestPosition[2], mapOutput.getNumRows(), mapOutput.getNumCols(), voidTile);
			}
			else
			{
				cout << "mapB.resizeNew(0, " << bestPosition[2] <<", mapOutput.getNumRows(), mapOutput.getNumCols(), voidTile);" << endl;
				mapB.resizeNew(0, bestPosition[2], mapOutput.getNumRows(), mapOutput.getNumCols(), voidTile);
			}
		}
		else
		{
			cout << "mapB.resizeNew(" << bestPosition[1] <<", 0, mapOutput.getNumRows(), mapOutput.getNumCols(), voidTile);" << endl;
			mapB.resizeNew(bestPosition[1], 0, mapOutput.getNumRows(), mapOutput.getNumCols(), voidTile);
			if (numColsA < numColsB)
			{
				cout << bestPosition[0] << endl;
				cout << "mapA.resizeNew(0, " << bestPosition[2] <<", mapOutput.getNumRows(), mapOutput.getNumCols(), voidTile);" << endl;
				mapA.resizeNew(0, bestPosition[2], mapOutput.getNumRows(), mapOutput.getNumCols(), voidTile);
			}
			else
			{
				cout << "mapB.resizeNew(0," << bestPosition[2] <<", mapOutput.getNumRows(), mapOutput.getNumCols(), voidTile);" << endl;
				mapB.resizeNew(0, bestPosition[2], mapOutput.getNumRows(), mapOutput.getNumCols(), voidTile);
			}
		}
	}
	mapDiff_func(mapA, mapB, mapOutput);
}

void getDiff_helper_cols(const wesmap &mapA, const wesmap & mapB, wesmap mapOutput, int rowPassed, int * bestPosition)
{
	westile voidTile("Xv");
	wesmap tempA = mapA;
	wesmap tempB = mapB;
	int toCompare = 0;
	int rowsA, rowsB, colsA, colsB;
	rowsA = mapA.getNumRows(); rowsB = mapB.getNumRows();
	colsA = mapA.getNumCols(); colsB = mapB.getNumCols();

	if (rowsA != colsB)
	{
		if (colsA < colsB)
		{
			for (int colNow = 0; colNow < colsB - colsA; colNow++)
			{
				tempA = mapA;
				tempA.resizeNew(rowPassed, colNow, mapOutput.getNumRows(), mapOutput.getNumCols(), voidTile);
				toCompare = mapDiff_func(tempA, tempB, mapOutput);
				if (toCompare < bestPosition[0])
				{
					bestPosition[0] = toCompare;
					bestPosition[1] = rowPassed;
					bestPosition[2] = colNow;
				}
			}
		}
		else
		{
			for (int colNow = 0; colNow < colsA - colsB; colNow++)
			{
				tempB = mapB;
				tempB.resizeNew(rowPassed, colNow, mapOutput.getNumRows(), mapOutput.getNumCols(), voidTile);
				toCompare = mapDiff_func(tempA, tempB, mapOutput);
				if (toCompare < bestPosition[0])
				{
					bestPosition[0] = toCompare;
					bestPosition[1] = rowPassed;
					bestPosition[2] = colNow;
				}
			}
		}
	}
}

int newDiff(const wesmap & mapA, int rowOffA, int colOffA, const wesmap mapB, int rowOffB, int colOffB)
{
	int numDiffs = 0;
	int rowB = rowOffA;
	for (int rowA = rowOffB; (rowA < mapA.getNumRows()) && (rowB < mapB.getNumRows()); rowA++, rowB++)
	{
		int colB = colOffA;
		for (int colA = colOffB; (colA < mapA.getNumCols()) && (colB < mapB.getNumCols()); colA++, colB++)
		{
			if (mapA.getTile(rowA, colA) == mapB.getTile(rowB, colB))
			{
				//match!
			}
			else
			{
				numDiffs++;
			}
		}
	}
	if (numDiffs == 0) cout << rowOffA << " " << colOffA << " " << rowOffB << " " << colOffB << endl;
	return numDiffs;
}