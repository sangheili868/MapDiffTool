#include <iostream>
#include <sstream>
#include "filesystem.hpp"
#include "wesmap.hpp"

//http://wiki.wesnoth.org/TerrainCodesWML
//http://wiki.wesnoth.org/TerrainCodeTableWML

wesmap::wesmap(const string & filename)
{
	//File Constructor
	string config = filesystem::read_file(filename);
	stringstream infile(config);
	/*
	if (!infile.is_open()) {
		cout << "Invalid filename: " << filename << " does not exist." << endl;
		exit(0);
	}*/

	//usage=map
	//border_size=1
	infile >> config;
	if (config.substr(0, 6) == "usage=")
	{
		usage = config.substr(6);
		infile >> config;
	}
	if (config.substr(0, 12) == "border_size=")
	{
		stringstream toConvert(config.substr(12));
		toConvert >> borderSize;
		config.clear();
	}
	string input = config;
	string inputLine;
	vector<westile> tileRow;
	numRows = 0;
	getline(infile, inputLine);
	getline(infile, inputLine);
	getline(infile, inputLine);

	while (getline(infile, inputLine))
	{
		//infile >> skipws >> inputLine;
		numCols = 0;
		stringstream line(inputLine);
		while (getline(line, input, ','))
		{
			stringstream inputStream(input);
			inputStream >> ws;
			inputStream >> input;
			//cout << "|" << input <<"|" << endl; //check eliminated whitespace
			tileRow.push_back(westile(input));
			numCols++;
		}
		tiles.push_back(tileRow);
		numRows++;
		tileRow.clear();
	}
	//cout << "|" << tiles[0][0].bgCode << "|" << endl;
	//cout << "numRows: " << numRows << endl;
	//cout << "numCols: " << numCols << endl;
}

wesmap::wesmap(int rowNum, int colNum, westile myTile)
{
	vector<westile> temp;
	numRows = rowNum;
	numCols = colNum;
	for (int i = 0; i < colNum; i++) {
		temp.push_back(myTile);
	}
	for (int j = 0; j < rowNum; j++) {
		tiles.push_back(temp);
	}
}

void wesmap::resize(int newNumRows, int newNumCols, westile newtile) {
	//if size increases, fill with empty

	if (newNumRows > numRows)
	{
		vector<westile> tempvec;
		for (int i = 0; i<numCols; i++)
			tempvec.push_back(newtile);
		for (int j = numRows; j<newNumRows; j++)
			tiles.push_back(tempvec);
		numRows = newNumRows;
	}
	if (newNumCols > numCols)
	{
		for (int i = 0; i<numRows; i++)
			for (int j = numCols; j<newNumCols; j++)
				tiles[i].push_back(newtile);
		numCols = newNumCols;
	}
}

void wesmap::writeMap(string filename) {
	stringstream myfile;
	myfile << "border_size=" << borderSize << endl;
	myfile << "usage=" << usage << endl << endl;

	for (int row = 0; row < numRows; row++) {
		for (int col = 0; col < numCols; col++) {
			myfile << tiles[row][col].bgCode;
			if (tiles[row][col].fgCode != "XXXX") {
				myfile << "^" << tiles[row][col].fgCode;
			}
			if (col != numCols - 1)
			{
				myfile << '\t' << ", ";
			}
		}
		myfile << endl;
	}
	filesystem::write_file(filename, myfile.str());
}

void setLabel(int rowIndex, int colIndex, string myTile)
{
	if (changedtiles.count(rowindex))
	{
		// Row already has a map
		changedtiles[rowIndex].insert(make_pair(colIndex, myTile.name()));
	} else {
		// map for the row does not exists, so make it
		unordered_map<int, westile> newmap;
		newmap.insert(make_pair(colIndex, myTile.name()));
		changedtiles.insert(make_pair(rowIndex, newmap));
	}
}