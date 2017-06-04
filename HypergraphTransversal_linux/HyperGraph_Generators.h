#include "HyperGraph.h"
#include "Odometer.h"
		
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
	int current = index[index.size()-1];

	/// calculate the next number.
	int next = current +1;
		
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



template<class T>
std::vector<HyperEdge<T> > GenFullyConnectedGraph(HyperEdge<T> nodes)
{
	int node_count = nodes.size();
	std::vector<HyperEdge<T> > returnValue;
	HyperGraph<T> hyper_graph(nodes);

	Odometer o;

	o.push_back(0);
	o.push_back(1);

	while(o[0] < node_count-1)
	{
		returnValue.push_back(hyper_graph.GetHyperEdge(o));

		if(o[1] < node_count-1)
		{
			o[1]++;
		}
		else
		{
			o[0]++;
			o[1] = o[0] +1; // bit of hackery.
		}

	}
	return returnValue;
}


template<class T>
bool IsEdgeSubsetORSupersetofAnyEdge(ExplicitHyperGraph<T> *hypergraph,Odometer o)
{
	for(std::vector<Odometer>::iterator itr = hypergraph->begin(); itr != hypergraph->end(); itr++)
	{
		if(true==DoesACoverBorBCoverA((*itr),o))
		{
			return true;
		}

	}
	return false;
}

template<class T>
bool AreAllNodesCovered(ExplicitHyperGraph<T> *hypergraph)
{
	Odometer nodes = hypergraph->GetHyperIndex(hypergraph->GetNodes());
	for(std::vector<Odometer>::iterator itr = hypergraph->begin(); itr != hypergraph->end(); itr++)
	{
		nodes = Minus(nodes,(*itr));
		if(nodes.size()==0)
		{
			break;
		}

	}
	if(nodes.size()>0)
	{
		return false;
	}
	return true;
}

template<class T>
std::vector<ExplicitHyperGraph<T> *> *GenerateNextSimpleHypergraphChildren(ExplicitHyperGraph<T> *gen_children)
{
	std::vector<ExplicitHyperGraph<T> *> *returnValue = new std::vector<ExplicitHyperGraph<T> *>();

	size_t edges = gen_children->GetHyperEdgeCount();
	for(size_t edge = 0; edge < edges; edge++)
	{
		Odometer o = gen_children->RemoveByIndex(0);
		ExplicitHyperGraph<T> * workitem = new ExplicitHyperGraph<T>(*gen_children);

		if(o.size() >2)
		{
			std::vector<Odometer> new_edges = GenerateNMinusOne(o);
			std::vector<Odometer> add_edges;
			for(std::vector<Odometer>::iterator itr = new_edges.begin(); itr != new_edges.end(); itr++)
			{
				if(false==IsEdgeSubsetORSupersetofAnyEdge(workitem,(*itr)))
				{
					add_edges.push_back((*itr));
				}
			}
			for(std::vector<Odometer>::iterator itr = add_edges.begin(); itr != add_edges.end(); itr++)
			{
				workitem->AddHyperIndex((*itr));
			}
			if(AreAllNodesCovered(workitem))
			{
				returnValue->push_back(workitem);
			}
			else
			{
				delete workitem;
			}
		}
		else
		{
			if(AreAllNodesCovered(workitem))
			{
				returnValue->push_back(workitem);
			}
			else
			{
				delete workitem;
			}
		}
	}
	return returnValue;
}

template<class T>
void EnumerateAllHypergraphsOverNodes(HyperEdge<T> nodes, void (*visit_function)(ExplicitHyperGraph<T> *visited_set) )
{
	std::vector<std::vector<ExplicitHyperGraph<T> *> *> WorkQueue;

	std::vector<ExplicitHyperGraph<T> *> *CurrentItems;

	ExplicitHyperGraph<T> *CurrentItem;

	CurrentItem = new ExplicitHyperGraph<T>();

	CurrentItem->AddHyperEdge(nodes);

	CurrentItems = new std::vector<ExplicitHyperGraph<T> *> ();
	CurrentItems->push_back(CurrentItem);
	WorkQueue.push_back(CurrentItems);

	while(false==WorkQueue.empty())
	{
		CurrentItems = WorkQueue.back();
		WorkQueue.pop_back();
		CurrentItem = CurrentItems->back();
		CurrentItems->pop_back();
		if(CurrentItems->size()>0)
		{
			WorkQueue.push_back(CurrentItems);
		}
		else
		{
			delete CurrentItems;
		}
		(*visit_function)(CurrentItem);
		CurrentItems = GenerateNextSimpleHypergraphChildren(CurrentItem);
		if(CurrentItems->size()>0)
		{
			WorkQueue.push_back(CurrentItems);
		}
		else
		{
			delete CurrentItems;
		}
		delete CurrentItem;
	}
}

























