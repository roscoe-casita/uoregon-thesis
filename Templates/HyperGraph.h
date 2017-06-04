#ifndef HYPERGRAPH_HPP
#define HYPERGRAPH_HPP
#pragma once
#include <vector>
#include <map>
#include <assert.h>
	
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

void PrintOdometer(const Odometer &o)
{
	std::cout << "Odometer:";
	for(size_t i = 0; i < o.size(); i++)
	{
		std::cout << "\t" << o[i];
	}
	std::cout<< std::endl;
}
/*
	A hyper edge is a vector of nodes. This is the original C-Berg Hypergraph definition.
	
	A Hyperedge has a specific odometer given a hypergraph that contains the hyperedge.

	There are an countable number of odometers that map to a specific hyperedge given a hypergraph.

	*/
template<class T>
class HyperEdge: public std::vector<T>
{
};

template<class T>
void PrintHyperedge(const HyperEdge<T> &o)
{
	std::cout << "Hyperedge:";
	for(size_t i = 0; i < o.size(); i++)
	{
		std::cout << "\t" << o[i];
	}
	std::cout<< std::endl;
}


/*
	A hypergraph is a set of nodes (hyperedge) that has an arbirary ordering (index in the array) applied to it.

	There are only a few routines which run in <= O(C) || O(N) time, Translating an odometer to hyperedge, hyperedge to odometer.

	All other 'algorithms' are heuristics which generally have runtimes that depend on the contents of the hypernode or hyperedge. 
*/
template <class T>
class HyperGraph
{
public:
	/// A Hypergraph instance cannot change at run time. Odometer instances correspond to a specific hypergraph instance.
	/// Theoretically it is possible to build an online version that translates current outstanding odometer instance values to the new values.
	/// This is a heavyweight approach that should be implemented with that approach as the core tenant. 
	HyperGraph(HyperEdge<T> nodes)
	{
		// this assert ensures that zero sized hypergraphs are not created.
		assert(nodes.size() >0);

		// Validate that all Nodes in the network are unique. They must be comparable and distinguishable. 
		// This loop and check is not needed in production code if the vector of nodes is unique.
		for(size_t outer_index = 0; outer_index < nodes.size(); outer_index++)
		{
			for(size_t inner_index = outer_index +1; inner_index  < nodes.size(); inner_index ++)
			{
				assert(nodes[outer_index]!=nodes[inner_index]);
			}
		}
		// Initilize the arbirary ordering / mapping.
		for(size_t index =0; index < nodes.size(); index++)
        	{
			ReverseIndex[nodes[index]] = index;
		}
		Nodes = nodes;
	}

	/// Free the list of nodes. IF nodes needs to be destroyed, call them in the derived hypergraph class destructor.
	virtual ~HyperGraph(void)
	{
		Nodes.clear();
		ReverseIndex.clear();
	}
        
	/// Copy another hypergraph.
	HyperGraph( HyperGraph<T> &hyper_graph) 
	{
		Nodes = hyper_graph.Nodes;
		ReverseIndex = hyper_graph.ReverseIndex;
	}
	
	/// Set this hypergraph equal to another.
	HyperGraph<T> &operator=(HyperGraph<T> &hyper_graph) 
	{
		if(this != &hyper_graph)
		{
			Nodes.clear();
			Nodes = hyper_graph.Nodes;
			ReverseIndex.clear();
			ReverseIndex = hyper_graph.ReverseIndex;
		}
		return *this;
	}
public:
	/// Translate an odometer into a hyperedge. ( N -> 1)
	/// Complexity:  O(C + n),  where n = number of nodes in the given HyperEdge.
	HyperEdge<T> GetHyperEdge(Odometer index)	
	{
		// container for the return value.
		HyperEdge<T> returnValue; 
		// the hyperedge size is known, it is the size of the odometer.
		returnValue.resize(index.size());

		// the hypergraph size, used to round the indexing space.
		int size = (int)Nodes.size();

		for(size_t idx = 0; idx < index.size(); idx++)
		{	
			// for each number in the odometer,
			// map all numbers down to the size of the space of the hypergraph.
			// this allows indexing such as  [-1] and [2*size() + 1].
			int result = index[idx];

			// Compiler invariant double modulo to index space:   (X%M + M )%M,
			// http://stackoverflow.com/questions/1082917/mod-of-negative-number-is-melting-my-brain
			// this is a non-branching transform that is compiler independent.

			// This block of code rounds memory space:  [-2,-1,0,1,2,3],  size = 3 =>   [1,2,0,1,2,0] 
			int a = result;
			int b = size;
			result=  a - (a/b)*b;
			result = result + b;
			result = result - (result/b) * b;
			// end of rounding memory space code.


			returnValue[idx] = Nodes[result];
		}
		// construct an edge container and return the result.
		return returnValue;
	}

	/// Translate a hyperedge into an odometer.  ( 1 -> 1)
	Odometer GetHyperIndex(HyperEdge<T> edge)
	{
		Odometer returnIndex; 
		returnIndex.resize(edge.size());
		for(size_t index = 0; index < edge.size(); index++)
		{
			// this assert can be removed in production code.
			assert(ReverseIndex.find(edge[index]) != ReverseIndex.end());

			// lookup the index of an arbirary node.
			returnIndex[index] = ReverseIndex[edge[index]];
		}
		return returnIndex;
	}

	/*
	Get the Hypergraph node count.
	*/
	size_t GetNodeCount()		
	{
		return Nodes.size();
	}

protected:
	HyperEdge<T> Nodes;
	std::map<T,size_t> ReverseIndex;
};

#endif
