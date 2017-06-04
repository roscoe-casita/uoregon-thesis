#ifndef ODOMETER_H
#define ODOMETER_H
#pragma once
#include <vector>
#include <iostream>

/*
	Odometers is a vector of indexs,  each index corresponds 1 to 1 to a specific node for all hypergraphs. 
	
	Each Odometers instance represents a single linear integer programming variable. 

	The program is now able to explitily control the Odometer, save instances of the control, and modify the indexes on each iteration.  
	
	http://www.quickperm.org/odometers.php

	Odometers have the following features:
		
		Represents Current Combination
		Counts All Generations ("Distance")
		Sequentially Linked Digits or Wheels
		Expected Duplicate Digits
		Standalone and Shareable Readings
		Defines Upper and Lower Indices 
		Able to Set(n) & Distribute Readings
		Able to Reset()
		Able to Rewind()
		Able to Fast-Forward(n)
		Calculate the Nth Combination / Permutation without enumerating first.
*/
class Odometer : public std::vector<int>
{
};

Odometer Union(const Odometer &A, const Odometer &B);
Odometer Intersection(const Odometer &A, const Odometer &B);
Odometer Minus(const Odometer &A, const Odometer &B);
std::vector<Odometer> AddOdometer(const std::vector<Odometer> copy,const Odometer add);
std::vector<Odometer> AddOdometers(const std::vector<Odometer> copy,const std::vector<Odometer> add);

void PrintOdometer(const Odometer &o);
std::ostream operator<<(std::ostream &printer,const Odometer &o);

#endif