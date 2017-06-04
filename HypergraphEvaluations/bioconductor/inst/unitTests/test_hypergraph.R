simpleHypergraph <- function() {
    nodes <- LETTERS[1:4]
    hEdges <- lapply(list("A", LETTERS[1:2], LETTERS[3:4]), "Hyperedge")
    hg <- new("Hypergraph", nodes=nodes, hyperedges=hEdges)
}    


testConstruction <- function() {
    hg <- simpleHypergraph()
    checkEquals(TRUE, is(hg, "Hypergraph"))
}


testDirectedHypergraph <- function() {
    nodes <- letters[1:4]
    dhe1 <- DirectedHyperedge(tail=c("a", "b"), head=c("c", "d"))
    dhe2 <- DirectedHyperedge(tail=c("a"), head=c("b", "c", "d"))
    dhe3 <- DirectedHyperedge(tail=c("b", "c"), head=c("d"))
    dhe4 <- DirectedHyperedge(tail=c("a"), head=c("b"))
    hg <- new("Hypergraph", nodes=nodes,
              hyperedges=list(dhe1, dhe2, dhe3, dhe4))
    checkEquals(TRUE, is(hg, "Hypergraph"))
}
 

testHyperedges <- function() {
    nodes <- LETTERS[1:4]
    eList <- list("A", LETTERS[1:2], LETTERS[3:4])
    hEdges <- l2hel(eList)
    hg <- new("Hypergraph", nodes=nodes, hyperedges=hEdges)
    ## Add "default" labels
    for (i in seq_along(hEdges)) {
        he <- hEdges[[i]]
        label(he) <- as.character(i)
        hEdges[[i]] <- he
    }
    checkEquals(hEdges, hyperedges(hg))
    checkEquals(as.character(seq_along(eList)), hyperedgeLabels(hg))
}


testHyperedgeLabels <- function() {
    hg <- simpleHypergraph()
    expect <- as.character(1:3)
    checkEquals(expect, hyperedgeLabels(hg))
}


testNodes <- function() {
    nodes <- LETTERS[1:4]
    hEdges <- lapply(c("A", LETTERS[1:2], LETTERS[3:4]), "Hyperedge")
    hg <- new("Hypergraph", nodes=nodes, hyperedges=hEdges)
    checkEquals(nodes, nodes(hg))
}


testBadHyperedges <- function() {
    nodes <- LETTERS[1:4]
    hyperedges <- list(matrix(0, nrow=2, ncol=2))
    checkException(new("Hypergraph", nodes=nodes, hyperedges=hyperedges))

    hyperedges <- lapply(list(1:2, 1:3), "Hyperedge")
    checkException(new("Hypergraph", nodes=nodes, hyperedges=hyperedges))
    
    hyperedges <- lapply(list("A", c("A", "B"),
                              c("C", "Z"), c("Q", "R", "S")),
                         "Hyperedge")
    checkException(new("Hypergraph", nodes=nodes, hyperedges=hyperedges))
    
}


testNumNodes <- function() {
    nodes <- letters[1:10]
    hyperedges <- list(Hyperedge("a"))
    hg <- new("Hypergraph", nodes=nodes, hyperedges=hyperedges)
    checkEquals(10, numNodes(hg))
}

testInciMat <- function() {
    nodes <- letters[1:4]
    hEdges <- lapply(list(c("a", "b"),
                        c("b", "c"),
                        c("c", "d", "a")), "Hyperedge")
    hg <- new("Hypergraph", nodes=nodes, hyperedges=hEdges)
    mat <- inciMat(hg)
    expected <- cbind(c(1, 1, 0, 0),
                      c(0, 1, 1, 0),
                      c(1, 0, 1, 1))
    rownames(expected) <- nodes
    colnames(expected) <- as.character(1:length(hEdges))
    checkEquals(expected, mat)
    checkEquals(dimnames(expected), dimnames(mat))
}

testToGraphNEL <- function() {
    hg <- simpleHypergraph()
    bpg <- toGraphNEL(hg)
    checkEquals(TRUE, is(bpg, "graphNEL"))
    checkEquals(7, length(nodes(bpg)))
    checkEquals(5, numEdges(bpg))

    expectEdges <- list(A=c("1", "2"),
                        B="2",
                        C="3",
                        D="3",
                        "1"="A",
                        "2"=c("A", "B"),
                        "3"=c("C", "D"))
    bpEdges <- edges(bpg)
    checkEquals(expectEdges, bpEdges)
    expectNodes <- c(LETTERS[1:4], 1:3)
    checkEquals(expectNodes, nodes(bpg))
}


## testToGraphAM <- function() {
##     hg <- simpleHypergraph()
##     bpg <- toGraphAM(hg)
##     checkEquals(TRUE, is(bpg, "graphAM"))
##     checkEquals(7, length(nodes(bpg)))
##     checkEquals(5, numEdges(bpg))

##     expectEdges <- list(A=c("1", "2"),
##                         B="2",
##                         C="3",
##                         D="3",
##                         "1"="A",
##                         "2"=c("A", "B"),
##                         "3"=c("C", "D"))
##     bpEdges <- edges(bpg)
##     checkEquals(expectEdges, bpEdges)
##     expectNodes <- c(LETTERS[1:4], 1:3)
##     checkEquals(expectNodes, nodes(bpg))
## }    
