#include <stdio.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>

#include "HyperGraph.h"
#include "HyperGraph_Enumeration.h"
#include "HyperGraph_Transversal.h"
#include "GraphVizNodeDecorator.hpp"

std::stringstream dot_printer;

void ClearTraversalProperties()
{
		dot_printer.str("");
}


class MinTravTreeNode:public GraphVizNodeDecorator
{
public:
	MinTravTreeNode(MinHypergraphStackFrame &frame)
	{
	}
	virtual std::vector<GraphVizNodeDecorator *> GenerateGraphvizChildren()
	{
		std::vector<GraphVizNodeDecorator *> returnValue;
		for(size_t i = 0; i < Children.size(); i ++)
		{
			returnValue.push_back(Children[i]);
		}
		return returnValue;
	}
public:
	std::vector<MinTravTreeNode *> Children;
};

std::map<int,MinTravTreeNode *> Nodes;

void process_Hypergraph_transversal_tree_node(MinHypergraphStackFrame first, std::vector<MinHypergraphStackFrame> next,ExplicitHyperGraph<char> &ehg)
{
	std::map<int,MinTravTreeNode *>::iterator finder = Nodes.find(first.identifier);

	MinTravTreeNode *Parent = NULL;
	if(finder==Nodes.end())
	{
		Parent = new MinTravTreeNode(first);
		Nodes[first.identifier] = Parent;
	}
	Parent = Nodes[first.identifier];

	for(size_t t = 0; t < next.size(); t ++)
	{
		MinTravTreeNode *child = new MinTravTreeNode(next[t]);
		Parent->Children.push_back(child);
		Nodes[next[t].identifier] = child;
	}
}


void ProcessMinimumHittingSet(Odometer o, HyperEdge<char> edge)
{
	std::cout<< "MinTransversal:" << std::endl;
	//PrintOdometer(o);
	PrintHyperedge(edge);
	std::cout<< std::endl;

}

void ProcessTransversal(std::vector<Odometer> transversal,ExplicitHyperGraph<char> &ehg)
{
	size_t transversal_size = transversal.size();
	std::cout << "ProcessTransversal size:  " << transversal_size  << std::endl;

	size_t traversal_count = 1;
	std::cout<< "{" ;
	for(size_t o = 0; o < transversal_size ;o++)
	{
		Odometer od = transversal[o];
		HyperEdge<char> he = ehg.GetHyperEdge(od);
		traversal_count = traversal_count * od.size();
		std::cout<< "{" ;

		
		for(size_t i =0; i < he.size(); i ++)
		{
			std::cout<< he[i];

			if(o+1 < he.size())
			{
				std::cout<<"|";
			}
		}
		std::cout<< "}" ;
		if(o+1 < transversal_size)
		{
			std::cout<<",";
		}
	}
	std::cout<< "};" << std::endl;
	std::cout << "Total minimum hitting sets in this traversal: " << traversal_count  << std::endl;
}


ExplicitHyperGraph<char> GetPaperHypergraphSample()
{
	std::vector<HyperEdge<char>> sample;

	HyperEdge<char> temp;
	temp.push_back('1');
	temp.push_back('2');
	temp.push_back('3');
	sample.push_back(temp);
	temp.clear();
	temp.push_back('3');
	temp.push_back('4');
	temp.push_back('5');
	sample.push_back(temp);
	temp.clear();
	temp.push_back('1');
	temp.push_back('5');
	sample.push_back(temp);
	temp.clear();
	temp.push_back('2');
	temp.push_back('5');
	sample.push_back(temp);
	temp.clear();
	temp.push_back('4');
	temp.push_back('2');
	temp.push_back('1');
	sample.push_back(temp);	PrintHyperedges(sample);
	return ExplicitHyperGraph<char>(sample);
}

ExplicitHyperGraph<char> Gen2DConnectedGraph(int count)
{
	HyperEdge<char> nodes;

	for(int i =0; i< count; i++)
	{
		nodes.push_back((char)('A' + i));
	}	
	std::vector<HyperEdge<char>> fully_connected = GenFullyConnectedGraph<char>(nodes);

	PrintHyperedges(fully_connected);
	return ExplicitHyperGraph<char>(fully_connected);
}


void TestHypergraph(ExplicitHyperGraph<char> &tester,std::string file_name)
{
	::ClearTraversalProperties();

	std::cout<< "Processing Naive All Hitting Sets:" << std::endl;
	NaiveAllHittingSets(tester,ProcessMinimumHittingSet);

	::PrintAndResetCalls();

	std::cout<< "Processing By Branch and Bound :" << std::endl;
	NaiveBranchAndBoundByNodeMinHypergraphTransversals(tester,-1,ProcessMinimumHittingSet);

	::PrintAndResetCalls();

	std::cout<< "Processing By Hypergraph of Transversals:" << std::endl;
	HyperGraphTransversals(tester,-1,ProcessTransversal,process_Hypergraph_transversal_tree_node);

	::PrintAndResetCalls();

	std::ofstream printer;
	printer.open(file_name);
	printer<< GenerateGraphviz(Nodes[1],true,file_name);
	printer.close();
}

int main(int argc, char *argv[])
{
	ExplicitHyperGraph<char> tester = GetPaperHypergraphSample();


	TestHypergraph(tester,"PaperTest.dot");
	
	tester = Gen2DConnectedGraph(4);
	TestHypergraph(tester,"FullyConnect.dot");

	system("pause");
	return 0;
}