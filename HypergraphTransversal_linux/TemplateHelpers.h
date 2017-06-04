#ifndef TEMPLATE_HELPERS_H
#define TEMPLATE_HELPERS_H

#include <vector>


template<class T>
std::vector<T> ConcatVectors(const std::vector<T> &first,const std::vector<T> &second)
{
	std::vector<T> returnValue;

	size_t first_size = first.size();
	size_t second_size = second.size();
	for(size_t i=0; i < first_size; i++)
	{
		returnValue.push_back(first[i]);
	}
	for(size_t i=0; i < second_size; i++)
	{
		returnValue.push_back(second[i]);
	}
	return returnValue;
}


#endif
