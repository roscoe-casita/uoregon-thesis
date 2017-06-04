#include "Odometer.h"
#include <iostream>
#include <assert.h>
#include <algorithm>

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
	//assert(outer_size!=0);
	//assert(inner_size!=0);
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


bool StrictEqual(const Odometer &A, const Odometer &B)
{
	if(A.size()!=B.size())
		return false;
	size_t size = A.size();
	for(size_t i=0;i<size;i++)
	{
		if(A[i]!=B[i])
			return false;
	}
	return true;
}

bool SetEqual(const Odometer &A, const Odometer &B)
{
	if(A.size()!=B.size())
		return false;
	Odometer a = A;
	Odometer b = B;
	std::sort(a.begin(),a.end());
	std::sort(b.begin(),b.end());
	return StrictEqual(a,b);
}



bool DoesACoverB(const Odometer &A,const Odometer &B)
{
	size_t a_size = A.size();
	size_t b_size = B.size();
	for(size_t a = 0; a < a_size; a++)
	{
		bool found = false;
		for(size_t b =0; b < b_size;b++)
		{
			if(A[a]==B[b])
			{
				found = true;
				break;
			}
		}
		if(false==found)
			return false;
	}
	return true;
}

bool DoesACoverBorBCoverA(const Odometer &A,const Odometer &B)
{
	if(DoesACoverB(A,B))
		return true;
	if(DoesACoverB(B,A))
		return true;
	return false;
}

bool DoesAHitB(const Odometer &A,const Odometer &B)
{
	size_t a_size = A.size();
	size_t b_size = B.size();
	for(size_t a =0; a < a_size; a++)
	{
		for(size_t b =0; b<b_size; b++)
		{
			if(A[a]==B[b])
				return true;
		}
	}
	return false;
}

bool DoesAHitAll(const Odometer &A,const std::vector<Odometer> &sets)
{
	bool returnValue = true;
	size_t sets_size = sets.size();
	for(size_t ss = 0; ss < sets_size; ss++)
	{
		if(false==DoesAHitB(A,sets[ss]))
		{
			returnValue = false;
			break;
		}
	}
	return returnValue;
}

bool DoesAnyHitA(const std::vector<Odometer> &test, const Odometer &set)
{
	bool returnValue = false;
	size_t test_size = test.size();
	for(size_t ts = 0; ts < test_size; ts++)
	{
		if(true==DoesAHitB(test[ts],set))
		{
			returnValue=true;
			break;
		}
	}
	return returnValue;
}

std::vector<Odometer> GenerateNMinusOne(Odometer o)
{
	std::vector<Odometer> returnValue;

	size_t od = o.size();
	for(size_t i=0; i < od; i++)
	{
		Odometer add = o;
		add.erase(add.begin() + i);
		returnValue.push_back(add);
	}
	return returnValue;
}
std::vector<Odometer> GenerateCountDownSets(Odometer o)
{
	std::vector<Odometer> returnValue;
	size_t od = o.size();
	for(size_t i=0; i < od; i++)
	{
		Odometer add;
		for(size_t in =i; in < od; in++)
		{
			add.push_back(o[in]);
		}
		returnValue.push_back(add);
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

std::ostream &operator<<(std::ostream &printer,const Odometer &o)
{
	printer << "{" ;
	size_t size = o.size();
	for(size_t i = 0; i < size; i++)
	{
		printer << o[i];
		if(i +1 < size)
		{
			printer << "|";
		}
	}
	printer << "}" ;
	return printer;
}


std::ostream &operator<<(std::ostream &printer,const std::vector<Odometer> &odometers)
{
	printer << "[" ;
	size_t size = odometers.size();
	for(size_t i = 0; i < size; i++)
	{
		printer << odometers[i];
		if(i +1 < size)
		{
			printer << ",";
		}
	}
	printer << "]" ;
	return printer;
}
