#pragma once
#include <vector>
#include <map>
#include <assert.h>
	
//#define USE_REVERSE_INDEX


	/*
		The odometer class is designed as a vector of counters, where each counter corresponds to an index.

		This represents the ability to explicitly control a 'many dimentional' index vector. The user is able to explitily control the Odometer, 
		save instances of the control, and modify the indexes on each iteration. This explicityly removes recursion from code, and explitly removes stacks of things, 
		replaceing them with stack of numbers to represent the complex index.

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
	template<class T>
    class Odometer : public std::vector<int>
	{
    };

	/*
		A hyper edge is a complex construct that represents a multi-node-edge. Multiple HyperEdges would be required
		to represent a directed hyper edge, or a multi-directional-multiple-node-edges. The underlying construction supports such 
		rediculous and essoteric concepts, but implementing each one such that they are restricted is left as an exercise for the user.
		
		Each HyperEdge representation will have some specific attributes that need to be calculated or pre-stored and retrieve.

		Really this implementation assumes the space will potentially be dense, and either narrow and deep, or broad and shallow, and in both cases
		can be constructed quickly given some set of nodes which composed a theoretical edge. 

		A simple sub graph could be represented by the following two multi-sets:
			1. Set of all nodes in the sub-graph.
			2. Set of all two node hyper-edges in the sub-graph.
		
		This would represent a 'normally connected undirected graph' of the hypergraph.  This flexibility is the desired attribute of hypergraphs.
		The representation of the problem as a simple construct forces the user to re-think the problem in terms of hypegraph encodings.
	
		A Cycle in a sub-graph could be represented as one hyperedge, as they have ordered it even includes the cycle pathway that was detected when it was found.

		A hyper edge is simple a subset of nodes of the Hypergraph, that have some additional, user defined data (Not defined here).

		A hyper edge can also be stored and cached for lookup later.

		A sparce hyper edge / hyper index/ hyper graph can be simulated with a collection of nodes in an index, and a collection of edges that is small.

		This construction is for simplisity sake to create a simple, undirected, multiple node connection edge.
	*/
    template<class T>
    class HyperEdge: public std::vector<T>
    {
    };


	/*
		The hyper index is not the hypergraph itself, this is the array that allows an odometer to index into the network.

		A hyper graph has complexity M^N where:
			N is the number of Nodes, 
			M is the dimentionality of edges (Can be smaller or larger then N).

		Example: 
			A simple fully connected undirected hypergraph has 2^N hyperedges. 
			A N dimentional edge network, fully connected is  N^N hyperedges.
			A sparse, fully connected graph network with dimensionality 2 is N^2 hyperedges.

			A network where there is some arbitrary restriction on the size of the network edges by some other relation such as nPc, 
			then the network size might be restricted at: N! / C!*(N-C)!

			Because of the nature of the non-determinable inter relations of the network, instead of trying to create every representation
			we leave the customization to the end user.  This is simple a mechnizm to advance towards a solution.

			As every Hyper Edge represents a NP-Hard or NP-Complete problem space, effecient systems to answer the questions will exploit the
			nature of the date, not the nature of the structure.  Thus this provides the simplest structure, which allows the user to exploit
			the relations inside the data to direct the odometer vector search enumerators.

		This serves to represent the mechanism to index into a space larger then the universe.
		This allows an external control function to control the direction the odometer will move to.

		As hypergraph enumerators are known to be unknownably haltable, they can run forever. 
		They can potentially contain complexity > N^N, we assume the user will return false when
		the function should stop enumeration.

		User beware. 
	*/



	template <class T>
	class HyperGraph
	{
		public:
			/// The nodes in the network cannot change at run time.
			/// accept a list of network nodes. they are the underlying representation of a fully connected network.
			/// There is a need for a compare function to establish equality for reverse index lookup from a set of nodes.
			HyperGraph(HyperEdge<T> nodes)	{
																/// At this point, validation would dictate that we scan all the nodes, ensuring that all nodes are unique.
																/// Duplicate nodes causes hyperedges to be non-deterministic, specifically with reverse index lookup.
																/// All hyperedges can be scanned with multiple nodes, but not all hyper edges with multiple nodes can be generated.
																/// If this is confussing, think of many to one and one to many, this is a many to many, so all must be distinct. 
																///  Also notice that the caller is not allowed to make any changes to the network after creation.
																///  If the nodes changed, then all the currently stored odometers associated with the previous network, must be 
																///  recalculated for the new nodes, as the indexes could have changed.
																///		Instead of allowing the user to perform this insane operation, we force them to implement it by only allowing
																///		instances to exist with fixed point references. 

																for(size_t outer_index = 0; outer_index < nodes.size(); outer_index++)
																{
																	for(size_t inner_index = outer_index +1; inner_index  < nodes.size(); inner_index ++)
																	{
																		assert(nodes[outer_index]!=nodes[inner_index]);
																	}
																}
                
                                                                for(size_t index =0; index < nodes.size(); index++)
                                                                {
                                                                    ReverseIndex[nodes[index]] = index;
                                                                }
																Nodes = nodes;
															}

			/// The nodes are no longer needed after this point.  The destructed may need to call delete on each node.
			~OrderedHyperGraph(void)						{
																Nodes.clear();
                												ReverseIndex.clear();
                                                            }
        
			OrderedHyperGraph(const OrderedHyperGraph<T> &hyper_graph) 
															{
																Nodes = hyper_graph.Nodes;
																#ifdef USE_REVERSE_INDEX
																	ReverseIndex = hyper_graph.ReverseIndex;
																#endif
															}

			OrderedHyperGraph<T> &operator=(const OrderedHyperGraph<T> &hyper_graph) 
															{
																if(this != &hyper_graph)
																{
																	Nodes.clear();
																	Nodes = hyper_graph.Nodes;
																	#ifdef USE_REVERSE_INDEX
																		ReverseIndex.clear();
																		ReverseIndex = hyper_graph.ReverseIndex;
																	#endif
																}
																return *this;
															}

		public:
			/// Retreive the hyper-edge-nodes with a given odometer.
			/// Complexity:  O(N) = N,  where N = number of nodes in the given HyperEdge.
			HyperEdge<T> GetHyperEdge(Odometer<T> index)		{
																// result container.
																std::vector<T> returnValue;
																std::vector<size_t> counter = index.GetOdometer();
																returnValue.resize(counter.size());

																// for each number in the odometer,
																for(size_t index = 0; index < counter.size(); index++)
																{
																	returnValue[index] = Nodes[counter[index]];
																}

																// construct an edge container and return the result.
																return HyperEdge<T>(returnValue);
															}

			/// retrieve the odometer for a given hyper-edge-index.
			/// Complexity: O(N,M) = N*M where N is the number of nodes in the HyperEdge, and M is the number of nodes in the HyperGraph.
			/// Reverse lookup is slightly more complicated because each node in the hypergraph (M) needs to be compared to each node in the Hyper Edge (N). 
			Odometer<T> GetHyperIndex(HyperEdge<T> edge)		{
																std::vector<size_t> returnIndex; 
																std::vector<T> index_to_edge = edge.GetConnectedNodes();
																returnIndex.resize(index_to_edge.size());
																#ifdef USE_REVERSE_INDEX
																
																	for(size_t index = 0; index < index_to_edge.size(); index++)
																	{
																		returnIndex[index] = ReverseIndex[index_to_edge[index]];
																	}
																#else
																	for(size_t outer_index = 0; outer_index < index_to_edge.size(); outer_index++)
																	{
																		for(size_t inner_index = 0; inner_index < nodes.size(); inner_index ++)
																		{
																			if(index_to_edge[outer_index] == nodes[inner_index])
																			{
																				returnValue[outer_index] = inner_index;
																				break;
																			}
																		}
																	}
																#endif
																return Odometer<T>(returnIndex);
															}

			/*
				Get the Hypergraph node count.
			*/
			size_t GetNodeCount()							{
																return Nodes.size();
															}
			/*
			Enumerate Hyperedges. 
			Input:	Odometer start position.
			Input:	Hypergraph.
			Input:	External control function that does the following:
					reads the current hyperedge.
					reads the current ordered hypergraph.
					Sets the next odometer index.
					returns true/false for continuing enumerations.
			*/	
			static void Enumerate( bool (*func)(HyperEdge<T> item, Odometer<T> &index, OrderedHyperGraph<T> &graph),Odometer<T> start, OrderedHyperGraph<T> &graph)
															{
																/// Loop control.
																bool done = false;
																/// Continue until done.
																while(!done)
																{
																	/// Call the external control function, passing
																	///		Current hyper edge.
																	///		current odometer.
																	///		Network Node Count.
																	done = !(*func)(graph.GetHyperEdge(start), start,graph);
																}
															}
		private:
			/// internal storage container of the nodes in the hypergraph.
			std::vector<T> Nodes;
			#ifdef USE_REVERSE_INDEX
				std::hash_map<T,size_t> ReverseIndex;
			#endif
		public:
			
		/*
			Enumer over all the distinct combinations, or distinct hyper edges in a fully connected undirected hypergraph:

				{A,B,C}  =>  {{A},{B},{C},{A,B},{A,C},{B,C},{A,B,C}}

			This is also a depth first search.

		*/
		static bool Combinations(Odometer<T> &index,OrderedHyperGraph<T> &graph)
		{
			size_t count = graph.GetNodeCount();
			// retrieve the current odometer stack.
			std::vector<size_t> &o = index.GetOdometer();


			/// get the current number.
			size_t current = o[o.size()-1];

			/// calculate the next number.
			size_t next = current +1;
		
			/// if the next number is still in range,
			if(next < count)
			{
				/// dive in depth first search, with the next element.
				o.push_back(next);
				return true;
			}
			else
			{
				/// the next number is out of range, so we need to back up and increment.
				o.pop_back();

				/// if the stack is empty, then we just finished the entire search, 
				if(o.size() <= 0)
				{
					return false;
				}
				else
				{
					/// the branch needs to be changed, we are searching down, and we can't go any further, so we need to try the next
					/// branch in the n way tree.
					/// next iteration we will dive again. or backup and exit.
					o[o.size()-1]++;
					return true;
				}
			}
		}

		/*
			Enumer over all the distinct multi-pick permutations of a hypergraph

				{A,B,C}  =>  {{A},{B},{C},{A,B},{A,C},{B,C},{A,B,C}}

			This is also a depth first search.

		*/
		static bool Permutations(Odometer<T> &index,OrderedHyperGraph<T> &graph)
		{
			size_t count = graph.GetNodeCount();
			std::vector<size_t> &o = index.GetOdometer();

			int control = ((int)o.size()) -1;

			while(control >= 0)
			{
				size_t current = o[control];
				size_t next = current +1;
		
				if(next < count)
				{
					o[control] = next;
					return true;
				}
				else
				{
					o[control] = 0;
					control--;
				}
			}
			return false;
		}

		/*
			Enumer over all the distinct multi-pick permutations of a hypergraph

				{A,B,C}  =>  {{A},{B},{C},{A,B},{A,C},{B,C},{A,B,C}}

			This is also a depth first search.

		*/
		static bool MultiPermutations(Odometer<T> &index,OrderedHyperGraph<T> &graph)
		{
			size_t count = graph.GetNodeCount();
			std::vector<size_t> &o = index.GetOdometer();

			int control = (int)(o.size() -1);

			while(control >= 0 )
			{
				size_t current = o[control];
				size_t next = current +1;
		
				if(next < count )
				{
					o[control] = next;
					return true;
				}
				else
				{
					o[control] = 0;
					control--;
				}
			}
			if(index.GetOdometer().size() > 1)
			{
				o.erase(o.end()-1,o.end());

				for(size_t i = 0; i< o.size(); i++)
				{
					o[i] = 0;
				}
				return true;
			}
			return false;
		}
	};
