#ifndef GraphVizNodeDecorator_H_
#define GraphVizNodeDecorator_H_

#include <string>

#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
//#include <typeinfo>

/*
 * This class is used to uniquely identify every graphviz node on the system, if you
 * inherit and use the graphvizenodedecorator, this provides an inherent unique lengthing identifier:  GUID
 */
class IdentifierDatabase
{
public:
	IdentifierDatabase();
	~IdentifierDatabase();
	std::string GenerateID();
	void RetireID(std::string id);
private:
	std::map<std::string,size_t> Database;
	std::vector<char> ValidCharacters;
};


class GraphVizNodeDecorator
{
public:
	GraphVizNodeDecorator();
	virtual ~GraphVizNodeDecorator();

	virtual std::vector<GraphVizNodeDecorator *> GenerateGraphvizChildren() =0;
	virtual std::string GenerateGravizEdgeDecoration(GraphVizNodeDecorator *edge);
	virtual std::string GenerateGravizNode();
	virtual std::string GenerateGravizEdge(GraphVizNodeDecorator *edge,bool digraph);

public:
	const std::string UniqueIdentifier;
	std::string NodeLabel;
	std::string NodeShape;
};

std::string GenerateGraphviz(GraphVizNodeDecorator *root,bool digraph,std::string name);

void WriteDotFile(std::string file_name,std::string content);
void CompileDotFile(std::string base_filename,std::string extension);

#endif
