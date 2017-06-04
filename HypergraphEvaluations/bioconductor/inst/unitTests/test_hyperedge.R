testHyperedge <- function() {
    nodes <- LETTERS[1:4]
    label <- "Simple hyperedge"
    he <- new("Hyperedge", nodes=nodes, label=label)
    checkEquals(nodes, nodes(he))
    checkEquals(label, label(he))

    ## change label
    label(he) <- "newone"
    checkEquals("newone", label(he))
    
    ## check that we can omit the label
    he2 <- new("Hyperedge", nodes=nodes) 
    checkEquals(nodes, nodes(he2))
    checkEquals(TRUE, is.na(label(he2)))
}


testDirectedHyperedge <- function() {
    head <- LETTERS[1:4]
    tail <- LETTERS[19:21]
    label <- "Directed hyperedge"
    dhe <- new("DirectedHyperedge", head=head, tail=tail, label=label)
    checkEquals(head, head(dhe))
    checkEquals(tail, tail(dhe))
    checkEquals(TRUE, setequal(c(tail, head), nodes(dhe)))
    checkEquals(TRUE, is(dhe, "Hyperedge"))
}


testToUndirected <- function() {
    head <- LETTERS[1:4]
    tail <- LETTERS[19:21]
    label <- "Directed hyperedge"
    ## omit the label 
    dhe <- new("DirectedHyperedge", head=head, tail=tail)
    he <- toUndirected(dhe)
    checkEquals(TRUE, setequal(nodes(dhe), nodes(he)))
    checkEquals(TRUE, is(he, "Hyperedge"))
    checkEquals(FALSE, is(he, "DirectedHyperedge"))
}
    
