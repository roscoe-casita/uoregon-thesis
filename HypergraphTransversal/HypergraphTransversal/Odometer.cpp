#include "Odometer.h"
#include <iostream>
#include <assert.h>

/*
bool IsAInOdometer(int A,const Odometer &o)
{
	size_t count = o.size();
	for(size_t i = 0; i < count;i++)
	{
		if(o[i] == A)
		{
			return true;
		}
	}
	return false;
}

bool IsAnyIndexInOdometer(const Odometer &outer, const Odometer &inner)
{
	size_t count = outer.size();
	for(size_t i =0; i <count; i++)
	{
		if(IsAInOdometer(outer[i],inner))
		{
			return true;
		}
	}
	return false;
}
*/

Odometer Union(const Odometer &A, const Odometer &B)
{
	Odometer returnValue;
	returnValue=B;
	for(size_t i=0; i < A.size(); i++)
	{
		bool add =true;
		for(size_t j=0; j < B.size(); j++)
		{
			if(A[i] == B[j])
			{
				add = false;
				break;
			}
		}
		if(true==add)
		{
			returnValue.push_back(A[i]);
		}
	}
	return returnValue;
}

Odometer Intersection(const Odometer &A, const Odometer &B)
{
	size_t outer_size = A.size();
	size_t inner_size = B.size();
	Odometer returnValue;
	for(size_t o=0; o < outer_size; o++)
	{
		int compare = A[o];
		for(size_t i=0; i < inner_size; i++)
		{
			if(compare == B[i])
			{
				returnValue.push_back(compare);
			}
		}
	}
	return returnValue;
}

Odometer Minus(const Odometer &A, const Odometer &B)
{
	size_t outer_size = A.size();
	size_t inner_size = B.size();

	Odometer returnValue;
	for(size_t o=0; o < outer_size; o++)
	{
		bool add =true;
		int compare = A[o];
		for(size_t i=0; i < inner_size; i++)
		{
			if(compare == B[i])
			{
				add = false;
				break;
			}
		}
		if(true == add)
		{
			returnValue.push_back(compare);
		}
	}	
	return returnValue;
}

std::vector<Odometer> AddOdometer(const std::vector<Odometer> copy,const Odometer add)
{
	std::vector<Odometer> returnValue;

	for(size_t i=0; i < copy.size(); i++)
	{
		returnValue.push_back(copy[i]);
	}
	returnValue.push_back(add);
	return returnValue;
}


std::vector<Odometer> AddOdometers(const std::vector<Odometer> copy,const std::vector<Odometer> add)
{
	std::vector<Odometer> returnValue;

	for(size_t i=0; i < copy.size(); i++)
	{
		returnValue.push_back(copy[i]);
	}
	for(size_t i=0; i < add.size(); i++)
	{
		returnValue.push_back(add[i]);
	}
	return returnValue;
}


void PrintOdometer(const Odometer &o)
{
	std::cout << "Odometer:";
	for(size_t i = 0; i < o.size(); i++)
	{
		std::cout << "\t" << o[i];
	}
	std::cout<< std::endl;
}