#ifndef MAPDIFF_HPP
#define MAPDIFF_HPP

#include<string>

bool mapdiffer(std::string argument);
void parseCommas(std::string source, std::string & originalMap,
	std::string & newMap, std::string & diffMap);

#endif