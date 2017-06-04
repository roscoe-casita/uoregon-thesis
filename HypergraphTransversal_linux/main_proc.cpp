#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "HyperGraph.h"
#include "HyperGraph_Transversal.h"
#include "GraphVizNodeDecorator.h"
#include "HyperGraph_Generators.h"


std::string PrintEdgeList(std::vector<HyperEdge<std::string> > variables,HyperEdge<std::string> real_edge,std::vector<HyperEdge<std::string> > negation_set)
{
	std::stringstream printer;

	HyperEdge<std::string> temp;
	temp.push_back("+");
	variables.push_back(temp);
	variables.push_back(real_edge);
	temp.clear();
	temp.push_back("-");
	variables.push_back(temp);
	for(size_t i=0;i<negation_set.size();i++)
	{
		variables.push_back(negation_set[i]);
	}
	printer << variables;
	return printer.str();
}


class MinTravTreeNode:public GraphVizNodeDecorator
{
public:

	MinTravTreeNode(HypergraphStackFrame &first,ExplicitHyperGraph<std::string> &ehg)
	{
		Identifier = first.Identifier;
		std::stringstream printer;

		generalized_variables = ehg.GetHyperEdges(first.Transversals);
		negation_set = ehg.GetHyperEdges(first.NegationSets);
		this->NodeShape = "record";
	}

	void UpdateLabel()
	{
		this->NodeLabel = PrintEdgeList(generalized_variables,real_edge,negation_set);
	}
	virtual std::vector<GraphVizNodeDecorator *> GenerateGraphvizChildren()
	{
		return Children;
	}

public:
	std::vector<GraphVizNodeDecorator *> Children;
	int Identifier;
	std::vector<HyperEdge<std::string> > generalized_variables;
	HyperEdge<std::string> real_edge;
	std::vector<HyperEdge<std::string> > negation_set;
};

std::map<int,MinTravTreeNode *> Nodes;


 void process_Hypergraph_transversal_tree_node(HypergraphStackFrame &first,std::vector<HypergraphStackFrame> &next,Odometer edge,ExplicitHyperGraph<std::string> &ehg)
 {
 	std::cout<< "Processing Traversal Tree Node:" << std::endl;

	std::vector<HyperEdge<std::string> > generalized_variables = ehg.GetHyperEdges(first.Transversals);

 	std::cout << "\t"<<generalized_variables << " -> "  << ehg.GetHyperEdge(edge) << std::endl;


 	MinTravTreeNode *parent = Nodes[first.Identifier];

 	if(NULL==parent)
 	{
 		parent = new MinTravTreeNode(first,ehg);
 		Nodes[first.Identifier] = parent;
 	}
 	parent->real_edge = ehg.GetHyperEdge(edge);
 	parent->UpdateLabel();

 	size_t count = next.size();
 	for(size_t c = 0; c < count; c++)
 	{
		std::cout << "\t" << "\t"<< ehg.GetHyperEdges(next[c].Transversals) << std::endl;

		MinTravTreeNode *ptr = new MinTravTreeNode(next[c],ehg);

		Nodes[ptr->Identifier] = ptr;
		parent->Children.push_back(ptr);
 	}
    std::cout << std::endl;
 }


void ProcessMinimumHittingSet(Odometer o, HyperEdge<std::string> edge)
{
	//PrintOdometer(o);
	std::cout<< edge << std::endl;

}

Odometer Unionize(const std::vector<Odometer> &singles);

Odometer Unionize(const std::vector<Odometer> &singles)
{
	Odometer returnValue;

	size_t size = singles.size();

	for(size_t s=0; s < size; s++)
	{
		returnValue = Union(returnValue,singles[s]);
	}
	return returnValue;
}


Odometer Negate(Odometer o,std::vector<Odometer> &negation_sets)
{
	Odometer returnValue =o;
	size_t set_size = negation_sets.size();
	for(size_t i =0; i < set_size; i++)
	{
		Odometer neg_set = negation_sets[i];
		if(DoesACoverB(neg_set,o))
		{
			returnValue = Minus(returnValue,neg_set);
		}
	}
	return returnValue;
}

std::vector<Odometer> RemoveFromSet(std::vector<Odometer> vect,Odometer remove)
{
	std::vector<Odometer> returnValue;
	for(size_t i =0; i < vect.size();i++)
	{
		if(SetEqual(vect[i],remove))
		{

		}
		else
		{
			returnValue.push_back(vect[i]);
		}
	}
	return returnValue;
}
 
void ProcessTransversal(std::vector<Odometer> transversal,ExplicitHyperGraph<std::string> &ehg)
{
	size_t transversal_size = transversal.size();
	std::cout << "ProcessTransversal size:  " << transversal_size  << std::endl;

	Odometer o = Unionize(transversal);

	std::cout << ehg.GetHyperEdge(o) << std::endl << std::endl;

}


ExplicitHyperGraph<std::string> GetPaperHypergraphSample()
{
	std::vector<HyperEdge<std::string> > sample;

	HyperEdge<std::string> temp;
	temp.push_back("1");
	temp.push_back("2");
	temp.push_back("3");
	sample.push_back(temp);
	temp.clear();
	temp.push_back("3");
	temp.push_back("4");
	temp.push_back("5");
	sample.push_back(temp);
	temp.clear();
	temp.push_back("1");
	temp.push_back("5");
	sample.push_back(temp);
	temp.clear();
	temp.push_back("2");
	temp.push_back("5");
	sample.push_back(temp);
	temp.clear();
	temp.push_back("4");
	temp.push_back("2");
	temp.push_back("1");
	sample.push_back(temp);	

	std::cout<< sample;
	return ExplicitHyperGraph<std::string>(sample);
}

ExplicitHyperGraph<std::string> GetOddHypergraphSample()
{
	std::vector<HyperEdge<std::string> > sample;

	HyperEdge<std::string> temp;
	temp.push_back("A");
	temp.push_back("B");
	sample.push_back(temp);
	temp.clear();
	temp.push_back("D");
	temp.push_back("E");
	sample.push_back(temp);
	temp.clear();
	temp.push_back("G");
	temp.push_back("H");
	sample.push_back(temp);
	temp.clear();
	temp.push_back("J");
	temp.push_back("K");
	sample.push_back(temp);
	temp.clear();
	temp.push_back("M");
	temp.push_back("N");
	sample.push_back(temp);
	temp.clear();
	temp.push_back("B");
	temp.push_back("E");
	temp.push_back("H");
	temp.push_back("K");
	temp.push_back("N");
	temp.push_back("R");
	sample.push_back(temp);

	std::cout<< sample;
	return ExplicitHyperGraph<std::string>(sample);
}


void AddEdge(std::vector<HyperEdge<std::string> > &addto,std::string A,std::string B)
{
	HyperEdge<std::string> temp;
	temp.push_back(A);
	temp.push_back(B);
	addto.push_back(temp);
}

ExplicitHyperGraph<std::string> AnotherHypergraphSample()
{
	std::vector<HyperEdge<std::string> > sample;

	AddEdge(sample,"1","A");
	AddEdge(sample,"2","A");
	AddEdge(sample,"2","B");
	AddEdge(sample,"3","B");
	AddEdge(sample,"3","C");
	AddEdge(sample,"1","B");
	AddEdge(sample,"2","C");
	AddEdge(sample,"3","A");
	AddEdge(sample,"2","B");
	AddEdge(sample,"1","C");

	std::cout<< sample;
	return ExplicitHyperGraph<std::string>(sample);
}

ExplicitHyperGraph<std::string> Gen2DHypergraphFullyConnected()
{
	std::vector<HyperEdge<std::string> > sample;

	AddEdge(sample,"A","B");
	//AddEdge(sample,"C","D");
	AddEdge(sample,"A","C");
	AddEdge(sample,"A","D");
	AddEdge(sample,"B","C");
	AddEdge(sample,"B","D");
	std::cout<< sample;
	return ExplicitHyperGraph<std::string>(sample);
}



ExplicitHyperGraph<std::string> Gen2DConnectedGraph(int count)
{
	HyperEdge<std::string> nodes;

	for(int i =0; i< count; i++)
	{
		char add = 'A' + i;
		std::string value;
		value += add;
		nodes.push_back(value);
	}	
	std::vector<HyperEdge<std::string> > fully_connected = GenFullyConnectedGraph<std::string>(nodes);

	std::cout<<fully_connected;
	return ExplicitHyperGraph<std::string>(fully_connected);
}

void Pause()
{
	std::cout<< "Please press any key to continue..." << std::endl;
	//int c = getchar();
}
void RunAndOpenOutput(std::string basefilename,ExplicitHyperGraph<std::string> &ehg)
{
	std::cout <<"Runing tests over:" << basefilename << std::endl;
	std::cout << ehg.GetHyperEdges() << std::endl;
	global_id = 1;

	for(std::map<int,MinTravTreeNode*>::iterator itr = Nodes.begin(); itr != Nodes.end(); itr++)
	{
		delete itr->second;
	}
	Nodes.clear();
	
	//std::cout<< "Processing Naive All Hitting Sets:" << std::endl;
	//NaiveAllHittingSets(ehg,ProcessMinimumHittingSet);

	::PrintAndResetCalls();

	std::cout<< "Processing By Branch and Bound :" << std::endl;
	NaiveBranchAndBoundByNodeMinHypergraphTransversals(ehg,-1,ProcessMinimumHittingSet);

	::PrintAndResetCalls();

	std::cout<< "Processing By Hypergraph of Transversals:" << std::endl;
	HyperGraphTransversals<std::string>(ehg,-1,ProcessTransversal,process_Hypergraph_transversal_tree_node);

	::PrintAndResetCalls();


	MinTravTreeNode *node = Nodes[1];

	std::string temp = GenerateGraphviz(node,true,"HyperGraph Transversal Tree");

	WriteDotFile(basefilename,temp);
	CompileDotFile(basefilename,"png");
	std::string command = "xdot " + basefilename + ".dot";
	system(command.c_str());
	Pause();
}


int simple_hypergraph_count = 0;
void visiter_function(ExplicitHyperGraph<std::string> *visited_set)
{
	simple_hypergraph_count++;

	std::cout<< "Encountered Hypergraph:" << std::endl;

	std::cout << visited_set->GetHyperEdges() << std::endl << std::endl;

	//RunAndOpenOutput("HGTT",*visited_set);

}

void TestProcedure()
{
	ExplicitHyperGraph<std::string> tester ;

	tester = GetPaperHypergraphSample();
	RunAndOpenOutput("Paper",tester);

	//tester = GetOddHypergraphSample();
	//RunAndOpenOutput("Odd",tester);
	tester = AnotherHypergraphSample();
	RunAndOpenOutput("Another",tester);
	tester=Gen2DConnectedGraph(4);
	RunAndOpenOutput("2D",tester);

	tester=Gen2DHypergraphFullyConnected();
	RunAndOpenOutput("2D2",tester);

}

void GenerateProcedure(int argc, char *argv[])
{
	HyperEdge<std::string> values;

	if(argc < 3)
	{
		std::cout << "Usage: <prgm> A B C ... " << std::endl;
	}
	for(int i=1; i< argc;i++)
	{
		std::string temp = argv[i];
		values.push_back(temp);
	}
	//tester.AddHyperEdge(values);

	EnumerateAllHypergraphsOverNodes<std::string>(values,visiter_function);

	std::cout <<" Total Simple Hypergraph Count: " << simple_hypergraph_count <<std::endl;

}


int main(int argc, char *argv[])
{
	TestProcedure();
	//GenerateProcedure(argc,argv);
	return 0;
}
