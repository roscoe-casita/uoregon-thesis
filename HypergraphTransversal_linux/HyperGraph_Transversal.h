
#ifndef HYPERGRAPH_TRANSVERSAL_H
#define HYPERGRAPH_TRANSVERSAL_H
#pragma once

#include "HyperGraph.h"
#include <assert.h>
#include <algorithm>
#include <math.h>


int TransversalCalls =0;
int BadTransversalCalls =0;
int TransversalCount = 0;

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

bool IsMinimalTransversal(const Odometer &o, const std::vector<Odometer> &edges)
{
	TransversalCalls++;

	if(false==DoesAHitAll(o,edges))
	{
		return false;
	}

	std::vector<Odometer> tests = GenerateNMinusOne(o);
	size_t test_count = tests.size();

	for(size_t tc=0; tc < test_count; tc++)
	{
		if(true==DoesAHitAll(tests[tc],edges))
		{
			return false;
		}
	}
	return true;
}

template<class T>
void NaiveAllHittingSets(ExplicitHyperGraph<T> &ehg,void (*process_hitting_set)(Odometer o, HyperEdge<T> hit_set))
{
	Odometer current_set;
	const int node_count = ehg.GetNodeCount();
	//const int hyperedge_count = ehg.GetHyperEdgeCount();
	current_set.push_back(0);
	//bool done = false;

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
	//const size_t hyperedge_count = ehg.GetHyperEdgeCount();
	current_set.push_back(0);
	bool done = false;

	while(!done)
	{
		if((count == 0) || current_set.size() ==0)
		{
			done=true;
			break;
		}

		bool hittingset =IsMinimalTransversal(current_set,ehg.GetOdometers());
		if(hittingset)
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


extern int global_id = 1;
class HypergraphStackFrame
{
public:
	HypergraphStackFrame()
	{
		Identifier = global_id++;
	}

public:
	std::vector<Odometer> Transversals;
	std::vector<Odometer> NegationSets;
	Odometer NewEdge;
	int Identifier;
};


class Gamma
{
public:
	Odometer XMinusIntersection;
	Odometer Intersection;
	Odometer YMinusIntersection;
	Odometer Original;
};

class IHGResult
{
public:
	std::vector<Odometer> Alphas;
	std::vector<Odometer> Betas;
	std::vector<Gamma> Gammas;
	std::vector<Odometer> Intersections;
	Odometer new_alpha_set;
};



IHGResult IntersectTransversalWithEdge(std::vector<Odometer> &Transversals,Odometer o)
{
	IHGResult returnValue;
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
			returnValue.Intersections.push_back(intersection);
			gamma.XMinusIntersection = Minus((*itr),intersection);
			gamma.YMinusIntersection = Minus(o,intersection);
			gamma.Original = (*itr);
			// Gamma general node is non-trivial.  Intersections get cut first, then two nodes appear.
			returnValue.Gammas.push_back(gamma);
		}
	}
	for(size_t i =0; i < returnValue.Intersections.size(); i++)
	{
		o = Minus(o,returnValue.Intersections[i]);
	}
	returnValue.new_alpha_set = o;
	return returnValue;
}


std::vector<std::vector<bool> > Gen2expNtruefalse(size_t number)
{
	std::vector<std::vector<bool> > returnValue;
	assert(number <=32);

	size_t max = 1 << number;

	for(size_t i=0; i < max; i++)
	{
		std::vector<bool> current;
		size_t counter =1;

		while(counter < max)
		{
			if((counter & i) == counter)
			{
				current.push_back(true);
			}
			else
			{
				current.push_back(false);
			}
			counter = counter<<1;
		}
		returnValue.push_back(current);
	}
	return returnValue;
}




bool IsAppropriate(HypergraphStackFrame &new_frame, Odometer old_edge)
{
	std::vector<Odometer> new_frame_traversals;
	std::vector<Odometer> negative_set = new_frame.NegationSets;

	for(size_t t=0; t < new_frame.Transversals.size(); t++)
	{
		Odometer genvar =new_frame.Transversals[t];
		for(size_t n=0; n < negative_set.size(); n++)
		{
			Odometer negset = negative_set[n];
			if( DoesACoverB(negset,genvar))
			{
				genvar = Minus(genvar,negset);

				//new_frame.NegationSets = RemoveFromSet(new_frame.NegationSets,negset);
			}
		}
		if(genvar.size()>0)
		{
			new_frame_traversals.push_back(genvar);
		}
	}
	new_frame.Transversals = new_frame_traversals;
	// set a new trave
	// for each negative set,
	//  Subtract from the variable the current negative set. if the

	if(false ==DoesAnyHitA(new_frame.Transversals,old_edge))
	{
		return false;
	}
	return true;

}

std::vector<HypergraphStackFrame> GenerateNextDepth(HypergraphStackFrame &frame, Odometer o)
{
	assert(frame.Transversals.size()!=0);
	assert(o.size()!=0);

	std::vector<HypergraphStackFrame> returnValue;

	IHGResult result = IntersectTransversalWithEdge(frame.Transversals,o);

	if(result.Gammas.size()==0)
	{
		HypergraphStackFrame process_later;
		process_later.Transversals = result.Alphas;
		process_later.NegationSets = frame.NegationSets;
		if(result.Betas.size()>0)
		{
			for(size_t i=0; i< result.Betas.size(); i++)
			{
				process_later.Transversals.push_back(result.Betas[i]);
			}
			// Here we might still need to add a node, if it has not yet been encounted, and its not in the visited / negation set. 
		}
		else
		{
			process_later.Transversals.push_back(o); // Only alphas encountered.
		}
		if(IsAppropriate(process_later,o))
		{
			returnValue.push_back(process_later);
		}
	}
	else 
	{
		std::vector<std::vector<bool> > XorIntersection = Gen2expNtruefalse(result.Gammas.size());	
		// This can become an exponential iterator.

		for(size_t xori =0; xori < XorIntersection.size();xori++)
		{
			std::vector<bool> orders = XorIntersection[xori];
			// process a next frame.
			HypergraphStackFrame process_next;
			process_next.Transversals = result.Alphas;
			process_next.NegationSets = frame.NegationSets;

			bool all_false = true;
			bool all_true = true;
			for(size_t order=0;order < orders.size(); order++)
			{
				Gamma gamma = result.Gammas[order];
				if(false==orders[order])
				{
					process_next.Transversals.push_back(gamma.XMinusIntersection);
					process_next.NegationSets.push_back(gamma.Intersection);
					//process_next.NegationSets.push_back(gamma.YMinusIntersection);
					all_true=false;
				}
				else
				{
					process_next.Transversals.push_back(gamma.Intersection);
					//process_next.NegationSets.push_back(gamma.XMinusIntersection);
					//process_next.NegationSets.push_back(gamma.YMinusIntersection);

					all_false=false;
				}
			}

			if(true==all_false)
			{
				if(result.new_alpha_set.size()>0)
				{
					process_next.Transversals.push_back(result.new_alpha_set);
				}
			}
			else if(true==all_true)
			{
				for(size_t i=0; i< result.Betas.size(); i++)
				{
					process_next.Transversals.push_back(result.Betas[i]);
				}
				for(std::vector<Gamma>::iterator itr = result.Gammas.begin(); itr != result.Gammas.end();itr++)
				{
					process_next.NegationSets.push_back((*itr).XMinusIntersection);
				}
			}
			else
			{
				for(size_t i=0; i< result.Betas.size(); i++)
				{
					process_next.Transversals.push_back(result.Betas[i]);
				}
			}
			if(IsAppropriate(process_next,o))
			{
				returnValue.push_back(process_next);
			}
		}
	}
	return returnValue;
}


/*
 * 			Gamma item = result.Gammas[i];

			HypergraphStackFrame process_later;
			process_later.Transversals = result.Alphas;
			process_later.Transversals.push_back(item.XMinusIntersection);
			process_later.Transversals.push_back(item.YMinusIntersection);
			returnValue.push_back(process_later);
 *
 *
 */

template<class T>
void HyperGraphTransversals(ExplicitHyperGraph<T> &ehg,size_t count, void (*process_transversal_set)(std::vector<Odometer>,ExplicitHyperGraph<T> &), void (*process_tree)(HypergraphStackFrame & , std::vector<HypergraphStackFrame> &, Odometer,ExplicitHyperGraph<T> &))
{
	//const int node_count = ehg.GetNodeCount();
	const int hyperedge_count = ehg.GetHyperEdgeCount();
	std::vector<std::vector<HypergraphStackFrame> > ControlStack;
	ControlStack.resize(hyperedge_count);

	HypergraphStackFrame frame;

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
				std::vector<HypergraphStackFrame> temp = std::vector<HypergraphStackFrame>();
				(*process_tree)(frame,temp,Odometer(),ehg);
			}
			else
			{
				// process next layer into a vector of frames.
				// push the intersection last, so it comes off first.
				// 
				control++;
				Odometer next_edge = ehg.GetHyperIndex(control);
				std::vector<HypergraphStackFrame> next_set = GenerateNextDepth(frame,next_edge);

				for(std::vector<HypergraphStackFrame>::iterator itr = next_set.begin(); itr != next_set.end(); itr++)
				{
					ControlStack[control].push_back((*itr));
				}
				(*process_tree)(frame,next_set,next_edge,ehg);
			}
		}
	}
}


/*
 *
 *
 *
 *
 * {{A,B,C}} + {D,E,F}		% Minimum traversal over generalized node 1, add edge that does not overlap. (all alphas)
{{A,B,C}|{D,E,F}}		//
	{A|D}
	{A|E}
	{A|F}
	{B|D}
	{B|E}
	{B|F}
	{C|D}
	{C|E}
	{C|F}

{{A,B,C}|{D,E,F}} + {G,H,I}	% Add the next edge, as above, all alphas.
{{A,B,C}|{D,E,F}}|{G,H,I}}	//  // compact representation as output. This is an encoded program that produces the minimum hitting sets as follows.
	{A|D|G}
	{A|D|H}
	{A|D|I}
	{A|E|G}
	{A|E|H}
	{A|E|I}
	{A|F|G}
	{A|F|H}
	{A|F|I}
	{B|D|G}
	{B|D|H}
	{B|D|I}
	{B|E|G}
	{B|E|H}
	{B|E|I}
	{B|F|G}
	{B|F|H}
	{B|F|I}
	{C|D|G}
	{C|D|H}
	{C|D|I}
	{C|E|G}
	{C|E|H}
	{C|E|I}
	{C|F|G}
	{C|F|H}
	{C|F|I}


{{A,B,C}|{D,E,F}}|{G,H,I}} + {B,E,H,J,K}	% Add a 3 way intersection edge, with one node overlap in each, + extra tails.

At this point it will be easier to assign an odometer / indexing scheme to show this.

This become a trivial traveral in a specific ordering:

0,0,0, +0  (This should also be interesting because its the alternative non-intersecting minium hitting set,
1,0,0, (This is the low hanging fruit
0,1,0,
0,0,1, //	Buried in the middle is all the combinations
1,1,0,	// If there was more then 2x0 1x1  to 1x0 2x1,  then the middle ground would be the random area.
1,0,1,
0,1,1,	(Near the edges is the most unique intersection selection so its not as interesting as either the edges or middle. this is the lost ground of combinitorics.
1,1,1	(Intersection of all intersections, this should be most interesting.

[[{A,C},{B}], [{D,F},{E}], [{G,I},{H}]]  // enumeration over these gamma segments results in the following traversal nodes, which contain minimum hitting sets:
					  [{J,K}]

{{A,C}|{D,F}|{G,I}|{J,K}}//	2^3-1  This is the piece wise outer union .  [0,0,0] + 0  // Non-intersection sets.
	{A|D|G|J}
	{A|D|G|K}
	{A|D|I|J}
	{A|D|I|K}
	{A|F|G|J}
	{A|F|G|K}
	{A|F|I|J}
	{A|F|I|K}
	{C|D|G|J}
	{C|D|G|K}
	{C|D|I|J}
	{C|D|I|K}
	{C|F|G|J}
	{C|F|G|K}
	{C|F|I|J}
	{C|F|I|K}

{{B}|{D,F}|{G,I}}	// [1,0,0]	// Low intersection sets.
	{B|D|G}
	{B|D|I}
	{B|F|G}
	{B|F|I}

{{A,C}|{E}|{I,G}} 	// [0,1,0]	// Low intersection sets.
	{A|E|I}
	{C|E|G}
	{C|E|I}
	{A|E|G}

{{A,C}|{D,F}|{H}}	// [0,0,1]	// Low intersection sets.
	{A|D|H}
	{C|D|H}
	{C|F|H}
	{A|F|H}

{{B}|{E}|{G,I}		// [1,1,0]	// High intersection sets.
	{B|E|G}
	{B|E|I}

{{B}|{D,F}|{H}}		// [1,0,1]	// High intersection sets.
	{B|D|H}
	{B|F|H}

{{A,C}|{E}|{H}}		// [0,1,1]	// High intersection sets.
	{A|E|H}
	{C|E|H}

{{B}|{E}|{H}}		// [1,1,1]	// 'most interesting minimum spanning sets.
	{B|E|H}				// As a set, this is the least set that crosses the most intersections.
 *
 */
#endif
