#ifndef ODOMETER_H
#define ODOMETER_H
#pragma once
#include <assert.h>
#include <vector>
#include <iostream>
#include "TemplateHelpers.h"

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
bool StrictEqual(const Odometer &A, const Odometer &B);
bool SetEqual(const Odometer &A, const Odometer &B);

bool DoesACoverB(const Odometer &A,const Odometer &B);
bool DoesAHitB(const Odometer &A,const Odometer &B);
bool DoesACoverBorBCoverA(const Odometer &A,const Odometer &B);
bool DoesAHitAll(const Odometer &A,const std::vector<Odometer> &sets);
bool DoesAnyHitA(const std::vector<Odometer> &test, const Odometer &set);

std::vector<Odometer> GenerateNMinusOne(Odometer original_set);

template<class T>
std::vector<std::vector<T> > GenerateNMinusOne(std::vector<T> original_set)
{
	std::vector<std::vector<T> > returnValue;
	size_t os_size = original_set.size();
	for(size_t os=0; os < os_size; os++)
	{
		std::vector<T> add = original_set;
		add.erase(add.begin() + os);
		returnValue.push_back(add);
	}
	return returnValue;
}
//std::vector<Odometer> GenerateCountDownSets(Odometer o);

void PrintOdometer(const Odometer &o);
std::ostream &operator<<(std::ostream &printer,const Odometer &o);
std::ostream &operator<<(std::ostream &printer,const std::vector<Odometer> &odometers);


#endif
