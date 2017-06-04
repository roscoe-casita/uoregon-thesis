#include <iostream>
#include <string>
#include "HyperGraph.h"

	
	OrderedHyperGraph<std::string> *hyperGraph = 0;

	/// function pointer for incrementing function.
	bool (*increment_function)(Odometer<std::string> &index,OrderedHyperGraph<std::string> &graph);

	bool Enumerate(HyperEdge<std::string> item, Odometer<std::string> &index,OrderedHyperGraph<std::string> &graph);

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
			std::vector<std::string> items;
			for( int i = 1; i < argc; i++)
			{
				items.push_back( std::string(argv[i]));
			}

			Odometer<std::string> o;

			hyperGraph = new OrderedHyperGraph<std::string>(items);

			std::vector<size_t> counts;

			std::cout << std::endl << "Node Count N: " << argc-1 << std::endl;
			std::cout << std::endl << "Enumerate Combinations: (2^N)-1 " << std::endl;
			enumerate_count = 0;
			counts.push_back(0);
			o.SetOdometer(counts);
			counts.clear();
			increment_function = OrderedHyperGraph<std::string>::Combinations;

			OrderedHyperGraph<std::string>::Enumerate(Enumerate,o,*hyperGraph);

			std::cout << std::endl << "Total Combinations: " << enumerate_count << std::endl;


			std::cout << std::endl << "Enumerate Permutations N^N: " << std::endl;
			enumerate_count = 0;
			for(int i = 1; i < argc; i++)
			{
				counts.push_back(0);
			}
			o.SetOdometer(counts);
			increment_function = OrderedHyperGraph<std::string>::Permutations;
			OrderedHyperGraph<std::string>::Enumerate(Enumerate,o,*hyperGraph);

			std::cout << std::endl << "Total Permutations N^N: " << enumerate_count << std::endl;



			std::cout << std::endl << "Enumerate Multi-Pick Permutations N^N + N^(N-1) + N^(N-2) + ... + N^0: " << std::endl;
			enumerate_count = 0;

			o.SetOdometer(counts);
			increment_function = OrderedHyperGraph<std::string>::MultiPermutations;
			OrderedHyperGraph<std::string>::Enumerate(Enumerate,o,*hyperGraph);

			std::cout << std::endl << "Total Multi-Pick Permutations N^N + N^(N-1) + N^(N-2) + ... + N^0: " << enumerate_count << std::endl;
			

		}
		return 0;
	}




	bool Enumerate(HyperEdge<std::string> item, Odometer<std::string> &index,OrderedHyperGraph<std::string> &graph)
	{	
		// advance a global counter for external reference.
		enumerate_count++;
		print_hyper_edge(item);


		// here increment the odometer via the specified function. 
		
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
		for(std::vector<std::string>::iterator itr = edge.GetConnectedNodes().begin(); 
			itr != edge.GetConnectedNodes().end();
			itr++)
		{
			std::cout << (*itr).c_str() << "\t";
		}
		std::cout << std::endl;
	}

