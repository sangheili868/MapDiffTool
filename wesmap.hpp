#ifndef WESMAP_HPP
#define WESMAP_HPP

#include <string>
#include <vector>
#include <deque>
#include <unordered_map>

using namespace std;

struct westile {
	string bgCode;
	string fgCode;
	string name;
	westile(string input) {
		for (int i = 0; i < input.size(); i++) {
			if (input.at(i) == '^'){
				bgCode = input.substr(0, i);
				fgCode = input.substr(i + 1);
			}
		}
		if (fgCode == "") {
			bgCode = input.substr(0);
			fgCode = "XXXX";
		}
		if (bgCode.size() > 4 || fgCode.size() > 4) {
			cout << "Error: Corrupt map file: " << bgCode << "^" << fgCode << endl;
		}
		name = input;
	}
	bool operator==(const westile &myWestile) const {
		if (bgCode == myWestile.bgCode && fgCode == myWestile.fgCode) {
			return true;
		}
		else{
			return false;
		}
	}
};

class wesmap {
public:
	wesmap(const string & filename);  //map file constuctor
	wesmap(int, int, westile);   // empty map constructor
	void resize(int, int, westile);	//if size increases, fill with empty
	void resizeNew(int, int, int, int, westile);
	void writeMap(const string filename);
	void setLabel(int rowIndex, int colIndex, string myTile);
	void writeScenarioMap(string filename, string mapFile);

	westile getTile(int rowIndex, int colIndex) const {
		//cout << rowIndex << " " << colIndex << endl;
		return tiles[rowIndex][colIndex];
	}
	void setTile(int rowIndex, int colIndex, westile myTile)
	{
		tiles[rowIndex][colIndex] = myTile;
	}
	int getNumRows() const { return numRows; }
	int getNumCols() const { return numCols; }
	void setBorderSize(int n) { borderSize = n; }
	void setUsage(const string & n) { usage = n; }

private:
	deque< deque<westile> > tiles;
	unordered_map<int, unordered_map<int, string> > changedtiles;
	int borderSize;
	string usage;
	int numRows;
	int numCols;
	string filename;
};


#endif