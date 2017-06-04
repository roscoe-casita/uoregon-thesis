#include "HyperGraph.h"
#include <assert.h>

#ifndef HYPERGRAPH_TRANSVERSAL_H
#define HYPERGRAPH_TRANSVERSAL_H
#pragma once

extern int TransversalCalls =0;
extern int BadTransversalCalls =0;
extern int TransversalCount = 0;

void PrintAndResetCalls()
{
	std::cout << "Total Calls to IsTransversal: " << TransversalCalls << std::endl;
	std::cout << "Total Transversals: " << TransversalCount << std::endl;
	std::cout << "Transversals to Call Ratio: "  <<  ((float)TransversalCount)/((float)TransversalCalls) << std::endl << std::endl;
	std::cout << "Transversals to Bad Transversal Calls Ratio: "  <<  ((float)TransversalCalls )/((float)BadTransversalCalls ) << std::endl << std::endl;
	TransversalCalls = 0;
	TransversalCount = 0;
	BadTransversalCalls =0;
}
class TransversalReturnCall
{
public:
	bool HittingSet;
	bool SubIsHittingSet;
};

template<class T>
TransversalReturnCall IsTransversal(ExplicitHyperGraph<T> &ehg, const Odometer &o)
{
	TransversalReturnCall returnValue;
	returnValue.HittingSet = false;
	returnValue.SubIsHittingSet = false;
	if(o.size()==0)
	{
		return returnValue;
	}

	TransversalCalls++;
	size_t count = 0;
	for(std::vector<Odometer>::iterator itr = ehg.begin(); itr != ehg.end(); itr++)
	{
		Odometer test = (*itr);
		size_t o_index = 0;
		for(size_t t_index = 0; t_index < test.size(); t_index++)
		{
			for(size_t o_index = 0; o_index < o.size(); o_index++)
			{
				if(test[t_index] == o[o_index])
				{
					o_index = o.size();
					t_index = test.size();
					count++;
				}
			}
		}
	}
	returnValue.HittingSet = (ehg.GetHyperEdgeCount() == count);

	Odometer test;
	for(size_t test_index =0; test_index < o.size(); test_index++)
	{
		test = o;
		test.erase(test.begin()+test_index,test.begin()+test_index+1);

		TransversalReturnCall rec_call = IsTransversal(ehg,test);
		if((rec_call.HittingSet == true) || (rec_call.SubIsHittingSet == true))
		{
			BadTransversalCalls++;
			returnValue.SubIsHittingSet = true;
			break;
		}
	}
	if(returnValue.SubIsHittingSet == true)
	{
		returnValue.HittingSet = false;
	}
	return returnValue;
}

template<class T>
void NaiveAllHittingSets(ExplicitHyperGraph<T> &ehg,void (*process_hitting_set)(Odometer o, HyperEdge<T> hit_set))
{
	Odometer current_set;
	const int node_count = ehg.GetNodeCount();
	const int hyperedge_count = ehg.GetHyperEdgeCount();
	current_set.push_back(0);
	bool done = false;

	while(current_set.size() > 0)
	{
		if(IsTransversal(ehg,current_set).HittingSet==true)
		{
			TransversalCount++;
			(*process_hitting_set)(current_set,ehg.GetHyperEdge(current_set));
		}
		int current = current_set[current_set.size()-1];
		int next = current +1;
		if(next < node_count)
		{
			current_set.push_back(next);
		}
		else
		{
			current_set.pop_back();
			if(current_set.size() > 0)
			{
				current_set[current_set.size()-1]++;
			}
		}
	}
}


template<class T>
void NaiveBranchAndBoundByNodeMinHypergraphTransversals(ExplicitHyperGraph<T> &ehg,size_t count, void (*process_min_hitting_set)(Odometer o, HyperEdge<T> min_set))
{
	Odometer current_set;
	const int node_count = ehg.GetNodeCount();
	const size_t hyperedge_count = ehg.GetHyperEdgeCount();
	current_set.push_back(0);
	bool done = false;

	while(!done)
	{
		if((count == 0) || current_set.size() ==0)
		{
			done=true;
			break;
		}
		if(IsTransversal(ehg,current_set).HittingSet == true)
		{
			TransversalCount++;
			count--;
			(*process_min_hitting_set)(current_set,ehg.GetHyperEdge(current_set));

			if(current_set[current_set.size()-1] < node_count -1)
			{
				current_set[current_set.size()-1]++;
			}
			else
			{
				current_set.pop_back();
				if(current_set.size() > 0)
				{
					current_set[current_set.size()-1]++;
				}
			}
		}
		else
		{
			int current = current_set[current_set.size()-1];
			int next = current +1;
			if(next < node_count)
			{
				current_set.push_back(next);
			}
			else
			{
				current_set.pop_back();
				if(current_set.size() > 0)
				{
					current_set[current_set.size()-1]++;
				}
			}
		}
	}
}

int global_id = 1;

class MinHypergraphStackFrame
{
public:
	MinHypergraphStackFrame()
	{
		identifier = global_id++;
	}
public:
	std::vector<Odometer> Transversals;
	Odometer visited;
	int identifier;

	void remove_self_items()
	{

		for(size_t i=0; i < Transversals.size(); i++)
		{
			visited = Minus(visited,Transversals[i]);
		}
	}
};


class Gamma
{
public:
	Odometer XMinusIntersection;
	Odometer Intersection;
	Odometer YMinusIntersection;
	Odometer Original;
};

class THGResult
{
public:
	std::vector<Odometer> Alphas;
	std::vector<Odometer> Betas;
	std::vector<Gamma> Gammas;

};

THGResult IntersectHypergraphTransversalWithEdge(std::vector<Odometer> &Transversals,Odometer o)
{
	THGResult returnValue;
	for(std::vector<Odometer>::iterator itr = Transversals.begin(); itr != Transversals.end(); itr++)
	{
		Odometer intersection = Intersection((*itr),o);
		if(0 == intersection.size())
		{
			// Alpha node type identified.
			returnValue.Alphas.push_back((*itr));
		}
		else if (intersection.size() >= (*itr).size())
		{
			// Beta type node identified,
			returnValue.Betas.push_back((*itr));
		}
		else 
		{
			Gamma gamma;
			gamma.Intersection = intersection;
			gamma.XMinusIntersection = Minus((*itr),intersection);
			gamma.YMinusIntersection = Minus(o,intersection);
			gamma.Original = (*itr);
			// Gamma general node is non-trivial.  Intersections get cut first, then two nodes appear.
			returnValue.Gammas.push_back(gamma);
		}
	}
	return returnValue;
}

std::vector<MinHypergraphStackFrame> GenerateNextDepth(MinHypergraphStackFrame frame, Odometer o)
{
	assert(frame.Transversals.size()!=0);
	assert(o.size()!=0);

	std::vector<MinHypergraphStackFrame> returnValue;

	o= Minus(o,frame.visited);  // this eliminates duplicate children?. 
	THGResult result = IntersectHypergraphTransversalWithEdge(frame.Transversals,o);

	if(result.Betas.size() >0)
	{
		Odometer visited = frame.visited;

		for(size_t b = 0; b < result.Betas.size(); b++)
		{
			o = Minus(o,result.Betas[b]);
		}
		visited = Union(visited,o);
		frame.visited = visited;
		frame.remove_self_items();
		returnValue.push_back(frame);  

		return returnValue;
	}
	// before anything:
	//Odometer save = o;


	if(result.Gammas.size()==0)
	{
		// only alphas encountered. Copy and add.
		Odometer visited = frame.visited;
		for(size_t new_alpha =0; new_alpha < o.size(); new_alpha++)
		{
			Odometer add;
			add.push_back(o[new_alpha]);

			MinHypergraphStackFrame process_later;
			process_later.visited = Union(visited,o);

			process_later.Transversals = result.Alphas;
			process_later.Transversals.push_back(add);
			process_later.remove_self_items();
			returnValue.push_back(process_later);
		}
	}
	else 
	{
		/*
		Cross product 2^ |Gamma| iteration selection.
		Some day this will need to be another control selection function,  (Note this is old, there is a new algorithm in polace)
		similar to the N way tree, but that it controls generation of the level, instead
		of just generating all the nodes in the next level.

		The problem is when |Gamma| is large.
		*/
		/*
		Optimal selection of non-regenerating children MUST go here.
		*/

		// (Alpha Sets + Beta Sets)  *  [ (  Gamma_0 \ Intersection_0 +  Intersection_0) * (  Gamma_1 \ Intersection_1 +  Intersection_1)…. (  Gamma_N \ Intersection_n +  Intersection_n)] 
		// This is the piece wise cross product expansion.

		// Consider instead that the value produced from the function is an encoding for a program.

		//  {{1,2},{4,5}},{{3}}}  this will be interpreted into the hitting sets:   {{1,4},{1,5},{2,4},{2,5},{3}}

		// When an edge is hit, it adds a Pick 1-N operation, normally a 2^N expansion.
		// Here when we encounter an edge, we only need to pick 1 and only 1 general node to expand if we do need to expand a general node.
		//result = IntersectHypergraphTransversalWithEdge(frame.Transversals,o);

		//result = IntersectHypergraphTransversalWithEdge(frame.Transversals,o);

		std::vector<Odometer> Intersection_container;
		Odometer visited = frame.visited;
		for(size_t i = 0; i < result.Gammas.size(); i++)
		{
			Gamma item = result.Gammas[i];

			MinHypergraphStackFrame process_later;
			process_later.Transversals = result.Alphas;
			visited = Union(visited,item.Intersection);

			Intersection_container.push_back(item.Intersection);
			process_later.Transversals.push_back(item.XMinusIntersection); 
			process_later.Transversals.push_back(item.YMinusIntersection); 
			returnValue.push_back(process_later);
		}

		for(size_t i =0; i < returnValue.size(); i++)
		{
			returnValue[i].visited = visited;
			returnValue[i].remove_self_items();
		}
		// now take the non-intersecting parts of the first, and add them to the alphas.
		MinHypergraphStackFrame process_next;
		process_next.Transversals = result.Alphas;
		for(size_t i=0; i < Intersection_container.size(); i++)
		{
			process_next.Transversals.push_back(Intersection_container[i]);
		}
		process_next.visited =  frame.visited;
		process_next.remove_self_items();
		returnValue.push_back(process_next);
	}
	return returnValue;
}


template<class T>
void HyperGraphTransversals(ExplicitHyperGraph<T> &ehg,size_t count, void (*process_transversal_set)(std::vector<Odometer>,ExplicitHyperGraph<T> &), void (*process_tree)(MinHypergraphStackFrame, std::vector<MinHypergraphStackFrame>, ExplicitHyperGraph<char> &))
{
	const int node_count = ehg.GetNodeCount();
	const int hyperedge_count = ehg.GetHyperEdgeCount();
	std::vector<std::vector<MinHypergraphStackFrame>> ControlStack;
	ControlStack.resize(hyperedge_count);

	MinHypergraphStackFrame frame;

	int control = 0;
	
	frame.Transversals.push_back(ehg.GetHyperIndex(control));
	ControlStack[control].push_back(frame);


	// N way tree control abstraction, this can be done with a single struct that contains
	// the int control from above.
	
	bool done = false;
	while(!done)
	{
		if((count == 0) || (control < 0))
		{
			done=true;
			break;
		}
		if(ControlStack[control].size() == 0)
		{
			control--;
		}
		else
		{
			// take one off the stack,
			// if this
			frame = ControlStack[control].back();
			ControlStack[control].pop_back();

			if(control == hyperedge_count-1)
			{
				TransversalCount++; // external variable counter.

				(*process_transversal_set)(frame.Transversals,ehg);
				(*process_tree)(frame,std::vector<MinHypergraphStackFrame>(),ehg);
			}
			else
			{
				// process next layer into a vector of frames.
				// push the intersection last, so it comes off first.
				// 
				control++;
				Odometer next_edge = ehg.GetHyperIndex(control);
				std::vector<MinHypergraphStackFrame> next_set = GenerateNextDepth(frame,next_edge);

				for(std::vector<MinHypergraphStackFrame>::iterator itr = next_set.begin(); itr != next_set.end(); itr++)
				{
					ControlStack[control].push_back((*itr));
				}
				(*process_tree)(frame,next_set,ehg);
			}
		}
	}
}
#endif