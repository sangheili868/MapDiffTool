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

void wesmap::resizeNew(int moveRows, int moveCols, int newNumRows, int newNomCols, westile newtile) {
	if (newNumRows > numRows)
	{
		vector<westile> tempvec;

		if (moveRows >= 0){
			for (int i = 0; i < moveRows; i++){
				tempvec.push_back(newtile);
			}

			if (moveCols >= 0){
				for (int k = 0; k < moveCols; k++){
					tiles.push_back(tempvec);
				}
			}
			else{
				for (int k = 0; k > moveCols; k--){
					tiles.push_front(tempvec);
				}
			}
		}

		else{
			for (int i = 0; i > moveRows; i--){
				tempvec.push_front(newtile);
			}

			if (moveCols >= 0){
				for (int k = 0; k < moveCols; k++){
					tiles.push_back(tempvec);
				}
			}
			else{
				for (int k = 0; k > moveCols; k--){
					tiles.push_front(tempvec);
				}
			}
		}

		for (int j = 0; j < numCols; j++){
			tempvec.push_back(newtile);
		}
		
		for (int l = numRows; l < newNumRows; l++){
			tiles.push_back(tempvec);
		}
		numRows = newNumRows;
	}
	/*if (newNumCols > numCols)
	{
		for (int i = 0; i<numRows; i++)
			for (int j = numCols; j<newNumCols; j++)
				tiles[i].push_back(newtile);
		numCols = newNumCols;
	}*/
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

void writeScenarioMap(string filename, string mapFile) {
	string input;
	stringstream myfile;

	myfile << "current_time=0" << endl;
	myfile << "description=""" << endl;
	myfile << "experience_modifier=70" << endl;
	myfile << "id=" << filename << endl;
	myfile << "mapdata=\"";
	 
	ifstream inFile;
	inFile.open(mapFile);

	while (!inFile.eof()) {
		cout << input;
	}
	inFile.close();

	myfile << endl << "\"" << endl;
	myfile << "name=\"" << fileName << "\"";
	myfile << "random_starting_time = no" << endl;
	myfile << "turns=-1" << endl;
	myfile << "victory_when_enemies_defeated=yes" << endl;
	myfile << "\[time\]\
		blue = 0\
		green = -20\
		id = \"dawn\"\
		image = \"misc/time-schedules/default/schedule-dawn.png\"\
		lawful_bonus = 0\
		mask = \"\"\
		#textdomain wesnoth - help\
		name = _\"Dawn\"\
		red = -20\
		sound = \"ambient/morning.ogg\"\
		[\/ time]\
	[time]\
	blue = 0\
		green = 0\
		id = \"morning\"\
		image = \"misc/time-schedules/default/schedule-morning.png\"\
		lawful_bonus = 25\
		mask = \"\"\
		name = _\"Morning\"\
		red = 0\
		sound = \"\"\
		[/ time]\
	[time]\
	blue = 0\
		green = 0\
		id = \"afternoon\"\
		image = \"misc/time-schedules/default/schedule-afternoon.png\"\
		lawful_bonus = 25\
		mask = \"\"\
		name = _\"Afternoon\"\
		red = 0\
		sound = \"\"\
		[/ time]\
	[time]\
	blue = -20\
		green = -20\
		id = \"dusk\"\
		image = \"misc/time-schedules/default/schedule-dusk.png\"\
		lawful_bonus = 0\
		mask = \"\"\
		name = _\"Dusk\"\
		red = 0\
		sound = \"ambient/night.ogg\"\
		[/ time]\
	[time]\
	blue = -10\
		green = -35\
		id = \"first_watch\"\
		image = \"misc/time-schedules/default/schedule-firstwatch.png\"\
		lawful_bonus = -25\
		mask = \"\"\
		name = _\"First Watch\"\
		red = -45\
		sound = \"\"\
		[/ time]\
	[time]\
	blue = -10\
		green = -35\
		id = \"second_watch\"\
		image = \"misc/time-schedules/default/schedule-secondwatch.png\"\
		lawful_bonus = -25\
		mask = \"\"\
		name = _\"Second Watch\"\
		red = -45\
		sound = \"\"\
		[/ time]\
		" << endl;

	for (auto iter = changedtiles.begin(); iter != changedtiles.end(); ++iter) {
		for (auto iter2 = changedtiles.at(iter->first).begin(); iter2 != changedtiles.at(iter->first).end(); ++iter2) {
			myfile << "[label]" << endl;
			myfile << "/tcolor=\"221, 221, 221,0\"" << endl;
			myfile << "/timmutable=yes" << endl;
			myfile << "/tteam_name=\"\"" << endl;
			myfile << "/ttext=\"" << (iter2->second) << "\"" << endl;
			myfile << "/tvisible_in_fog=yes" << endl;
			myfile << "/tvisible_in_shroud=no" << endl;
			myfile << "/tx=" << (iter->first) << endl;
			myfile << "/ty=" << (iter2->first) << endl;
			myfile << "[/label]" << endl;
		}
	}

}
