#include <iostream>
#include <string>
#include "HyperGraph.h"

	
HyperGraph<std::string> *hyperGraph = 0;

/// function pointer for incrementing function.
bool (*increment_function)(Odometer &index,HyperGraph<std::string> &graph);

bool Enumerate(HyperEdge<std::string> item, Odometer &index,HyperGraph<std::string> &graph);

void print_hyper_edge(HyperEdge<std::string> edge);
	

/// counter to advance the global external count, set and reset in main.
int enumerate_count = 0;

int main(int argc, char *argv[])
{
	for(int i =0; i < argc; i++)
	{
		std::cout<<argv[i]<<std::endl;
	}

	if(argc > 1 )
	{
		HyperEdge<std::string> items;
		for( int i = 1; i < argc; i++)
		{
			items.push_back( std::string(argv[i]));
		}


		hyperGraph = new HyperGraph<std::string>(items);

		std::cout << std::endl << "Node Count N: " << argc-1 << std::endl;
		Odometer o;
	
		std::cout << std::endl << "Enumerate Permutations: " << std::endl;
			
		enumerate_count = 0;
		o.clear();
		for(int i = 1; i < argc; i++)
		{
			o.push_back(0);
			
		}
		increment_function = Permutations<std::string>;
		EnumerationControl(Enumerate,o,*hyperGraph);

		std::cout << std::endl << "Total Permutations: " << enumerate_count << std::endl;
	}
	return 0;
}




bool Enumerate(HyperEdge<std::string> item, Odometer &index,HyperGraph<std::string> &graph)
{	
	// advance a global counter for external reference.
	enumerate_count++;
	print_hyper_edge(item);


	// here increment the odometer via the specified function. 

	// This is a generalize procedure, so this will advance by combination, permutation, multi-select, and random.
		
	return (*increment_function)(index,graph);

}

/// Used by the hyper index for reverse index lookup, tested with commented out function in code above.
int CompareStr(std::string &s1, std::string &s2)
{
	return s1.compare(s2);
}

	
/// print out a hyperedge of strings.
void print_hyper_edge(HyperEdge<std::string> edge)
{
	std::cout << "Nodes:\t";
	for(std::vector<std::string>::iterator itr = edge.begin(); 
		itr != edge.end();
		itr++)
	{
		std::cout << (*itr).c_str() << "\t";
	}
	std::cout << std::endl;
}

