#import custom_sampler
import uuid

def makeHyperGraph(hyperedge):
    vector_things = [0 for x in range(len(hyperedge))]
    for i in range(len(hyperedge)):
        node = hyperedge[i]
        vector_things[i] = node
    return vector_things


def getHyperEdge(hypergraph,odometer):
    hyperedge = [0 for x in range(len(odometer))]
    space_size = len(hypergraph)
    for index in range(len(odometer)):
        node_index = odometer[index] % space_size
        hyperedge[index] = hypergraph[node_index]
    return hyperedge


def getOdometer(hypergraph,hyperedge):
    odometer = [0 for x in range(len(hyperedge))]
    for n in range(len(hyperedge)):
        lookup = hyperedge[n]
        for hyperindex in range(len(hypergraph)):
            node = hypergraph[hyperindex]
            if node == lookup:
                odometer[n] = hyperindex
                break
    return odometer

def make_graphviz_hyperedge_undirectedgraph(hyperedge):
    string_builder = "  [shape=point]\n"
    UUID = str(uuid.uuid4()).replace("-","")

    while UUID[0] in set("0123456789"):
        UUID = UUID[1:]

    string_builder = UUID + string_builder
    for n in hyperedge:
        string_builder += UUID + " -- " + n + "\n"

    return string_builder

def make_graphviz_hyperedge_directedgraph(hyperedge):
    string_builder = "  [shape=point]\n"
    UUID = str(uuid.uuid4()).replace("-","")

    while UUID[0] in set("0123456789"):
        UUID = UUID[1:]

    string_builder = UUID + string_builder
    string_builder += UUID + " -> " + hyperedge[0] + "\n"
    string_builder += hyperedge[1] + " -> " + UUID + "\n"

    return string_builder

def make_graphviz_undirected_hypergraph(hypergraph,hyperedges):

    string_builder = " graph hypergraph{ \n"
    for hyperedge in hyperedges:
        string_builder += make_graphviz_hyperedge_undirectedgraph(hyperedge)
    string_builder += "\n}\n"

    return string_builder

def make_graphviz_directed_hypergraph(hypergraph,hyperedges):

    string_builder = "digraph hypergraph{ \n"
    for hyperedge in hyperedges:
        string_builder += make_graphviz_hyperedge_directedgraph(hyperedge)
    string_builder += "\n}\n"

    return string_builder

def OdometerAsList(hypergraph,odometer):
    if len(odometer) == 1:
        if odometer[0] + 1 < len(hypergraph):
            odometer[0] += 1
            return True
    return False

def OdometerAsFullUndirectedGraph(hypergraph,odometer):
    if len(odometer) == 2:
        if odometer[1] + 1 < len(hypergraph):
            odometer[1] += 1
            return True
        else:
            if odometer[0] + 2 < len(hypergraph):
                odometer[0] += 1
                odometer[1] = odometer[0] +1
                return True
    return False

def OdometerAsFullDirectedGraph(hypergraph,odometer):
    if len(odometer) == 2:
        if odometer[1] + 1 < len(hypergraph):
            odometer[1] += 1
            return True
        else:
            if odometer[0] + 1 < len(hypergraph):
                odometer[0] += 1
                odometer[1] = 0
                return True
    return False

def OdometerAsAllTreePath(hypergraph,odometer):

    if len(odometer) > 0:
        if odometer[-1] +1 < len(hypergraph):
            odometer.append(odometer[-1]+1)
            return True
        else:
            odometer = odometer[:-1]
            if len(odometer) > 1:
                odometer[-1] += 1
    return False

def EnumerateOdometer(hypergraph,odometer,func):
    returnValue = [ getHyperEdge(hypergraph,odometer) ]
    while func(hypergraph,odometer):
        returnValue.append( getHyperEdge(hypergraph,odometer))
    return returnValue



def hello_world():

    hypergraph = makeHyperGraph(sorted("ABCD"))

    odometer = [0]
    hyperedges = EnumerateOdometer(hypergraph,odometer,OdometerAsList)

    print_str = make_graphviz_undirected_hypergraph(hypergraph,hyperedges)
    print(print_str)

    odometer = [0,1]
    hyperedges = EnumerateOdometer(hypergraph,odometer,OdometerAsFullUndirectedGraph)
    print_str = make_graphviz_undirected_hypergraph(hypergraph,hyperedges)
    print(print_str)

    odometer = [0,0]
    hyperedges = EnumerateOdometer(hypergraph,odometer,OdometerAsFullDirectedGraph)
    print_str = make_graphviz_directed_hypergraph(hypergraph,hyperedges)
    print(print_str)

    odometer = [0]
    hyperedges = EnumerateOdometer(hypergraph,odometer,OdometerAsAllTreePath)
    print_str = make_graphviz_undirected_hypergraph(hypergraph,hyperedges)
    print(print_str)


def main():

    hello_world()


if __name__ == "__main__":
    main()



