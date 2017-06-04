setClass("Hyperedge", representation(head="character", label="character"))


setClass("DirectedHyperedge", representation(tail="character"),
         contains="Hyperedge")


setClass("Hypergraph", representation(nodes="character", hyperedges="list"))

