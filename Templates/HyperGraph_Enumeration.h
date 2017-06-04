#include "HyperGraph.h"
		
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

template<class T>
static void EnumerationControl( bool (*func)(HyperEdge<T> item, Odometer &index, HyperGraph<T> &graph),Odometer start, HyperGraph<T> &graph)
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

		/// The control function needs to return true/false to control termination.
		done = !(*func)(graph.GetHyperEdge(start), start,graph);
	}
}
			
/*
Enumer over all combinations, depth combination first:
{A,B,C}  =>  {{A},{A,B},{A,B,C},{A,C},{B},{B,C},{C}}

This is a greedy combinational depth first search that should be controlled by branch and bound.

Use this function and customize to your needs.

Complexity:  O(2^N).  This will enumerate strictly the distinct combination set.

The order of the nodes in the hyperedge is not meaninful in this representation.
*/
template<class T>
static bool Combinations(Odometer &index,HyperGraph<T> &graph)
{
	size_t count = graph.GetNodeCount();
	// retrieve the current odometer stack

	/// get the current number.
	size_t current = index[index.size()-1];

	/// calculate the next number.
	size_t next = current +1;
		
	/// if the next number is still in range,
	if(next < count)
	{
		/// dive in depth first search, with the next element.
		index.push_back(next);
		return true;
	}
	else
	{
		/// the next number is out of range, so we need to back up and increment.
		index.pop_back();

		/// if the stack is empty, then we just finished the entire search, 
		if(index.size() <= 0)
		{
			return false;
		}
		else
		{
			/// the branch needs to be changed, we are searching down, and we can't go any further, so we need to try the next
			/// branch in the n way tree.
			/// next iteration we will dive again. or backup and exit.
			index[index.size()-1]++;
			return true;
		}
	}
}

/*
	Enumer over all the permutations of size M, for a given set of nodes of a hypergraph
	M is the count of indexes in the odometer.
	M = 3, 
	{A,B,C} => {{A,A,A}, {A,A,B}, {A,A,C}, {A,B,A},..., {C,C,A},{C,C,B}, {C,C,C}}
	
	Complexity:   O(N^N).  This should be controlled with a branch and bound that skips forward.  {A,A,A} = success, =>  {B,A,A}

	Use this function and 
*/

template<class T>
static bool Permutations(Odometer &index,HyperGraph<T> &graph)
{
	size_t count = graph.GetNodeCount();
	int control = ((int)index.size()) -1;

	while(control >= 0)
	{
		size_t current = index[control];
		size_t next = current +1;
	
		if(next < count)
		{
			index[control] = next;
			return true;
		}
		else
		{
			index[control] = 0;
			control--;
		}
	}
	return false;
}

/*
	Enumer over all the permutations of size M, M-1, M-2..., 2, 1, for a given set of nodes of a hypergraph
	M is the count of indexes in the odometer.
	{A,B,C} => {{A,A,A}, {A,A,B}, {A,A,C}, {A,B,A},..., {C,C,A},{C,C,B}, {C,C,C},{A,A},{A,B},{A,C},...{C,B},{C,C},{A},{B},{C}}
	
	Complexity:   O(N^N).  This should be controlled with a branch and bound that skips forward.  {A,A,A} = success, =>  {A,A}
*/
template<class T>
static bool MultiPermutations(Odometer &index,HyperGraph<T> &graph)
{
	size_t count = graph.GetNodeCount();

	int control = (int)(index.size() -1);

	while(control >= 0 )
	{
		size_t current = index[control];
		size_t next = current +1;
		
		if(next < count )
		{
			index[control] = next;
			return true;
		}
		else
		{
			index[control] = 0;
			control--;
		}
	}
	// Reset the odometer to the next iteration of permutations.
	if(index.size() > 1)
	{
		index.erase(index.end()-1,index.end());

		for(size_t i = 0; i< index.size(); i++)
		{
			index[i] = 0;
		}
		return true;
	}
	return false;
}
	

