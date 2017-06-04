#ifndef TEMPLATE_HELPERS_H
#define TEMPLATE_HELPERS_H

#include <vector>


template<class T>
inline std::vector<T> ConcatVector(std::vector<T> part1,std::vector<T> part2)
{
	std::vector<T> returnValue;
	returnValue = part1;
	for(size_t i=0; i < part2.size(); i++)
	{
		returnValue.push_back(part2[i]);
	}
	return returnValue;
}

#endif