#include <iostream>
#include <string>
#include "HyperGraph.h"

	
HyperGraph<std::string> *hyperGraph = 0;


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
		std::cout << "Testing Odometer usages, positive indexes greater then size: " << std::endl;		

		for(int i = argc; i < 2*argc; i++)
		{
			o.push_back(i);
		}		
		HyperEdge<std::string> item0 = hyperGraph->GetHyperEdge(o);

		PrintOdometer(o);
		PrintHyperedge(item0);
		o.clear();
		

		std::cout << "Testing Odometer usages, negative numbers: " << std::endl;		

		for(int i = -1; i > (-1)*argc; i--)
		{
			o.push_back(i);
		}		
		HyperEdge<std::string> item1 = hyperGraph->GetHyperEdge(o);

		PrintOdometer(o);
		PrintHyperedge(item1);
		o.clear();


		o = hyperGraph->GetHyperIndex(items);

		std::cout << "Testing Reverse Index Lookup: " << std::endl;

		PrintOdometer(o);
		PrintHyperedge(items);
		o.clear();
		

		delete hyperGraph;
	}
	return 0;
}
