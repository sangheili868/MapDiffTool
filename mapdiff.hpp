#ifndef MAPDIFF_HPP
#define MAPDIFF_HPP

#include<string>
#include "wesmap.hpp"

bool mapdiffer(std::string argument);
void parseCommas(std::string source, std::string & originalMap,
	std::string & newMap, std::string & diffMap);
int newDiff(const wesmap & mapA, int rowOffA, int colOffA, const wesmap mapB, int rowOffB, int colOffB);
void getDiff_helper_cols(const wesmap &mapA, const wesmap & mapB, wesmap mapOutput, int rowPassed, int * bestPosition);
void getDiff(const wesmap & mapA, const wesmap & mapB, wesmap & mapOutput);
int mapDiff_func(const wesmap & mapA, const wesmap & mapB, wesmap & mapOutput);
void getDiff_optimized(wesmap & mapA, wesmap & mapB, wesmap & mapOutput);

#endif