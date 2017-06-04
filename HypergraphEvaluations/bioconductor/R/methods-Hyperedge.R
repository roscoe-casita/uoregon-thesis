## Hyperedge methods
setMethod("initialize", "Hyperedge",
          function(.Object, nodes, label) {
              .Object@head <- as.character(nodes)
              if (missing(label)) {
                  label <- ""
                  label[1] <- NA ## want an NA of type character
              }
              .Object@label <- label
              .Object
          })
Hyperedge <- function(nodes, label="")
  new("Hyperedge", nodes=nodes, label=label)


l2hel <- function(e) {
    ## Convenience function to create lists of Hyperedges
    numEdges <- length(e)
    hel <- vector(mode="list", length=numEdges)
    nms <- names(e)
    if (is.null(nms))
      nms <- as.character(1:numEdges)
    for (i in 1:numEdges) 
      hel[[i]] <- Hyperedge(nodes=e[[i]], label=nms[i])
    hel
}


setMethod("nodes", signature(object="Hyperedge"), function(object) object@head)


setMethod("label", signature(object="Hyperedge"),
          function(object) object@label)


setReplaceMethod("label", signature(object="Hyperedge", value="character"),
                 function(object, value) {
                     if (length(value) != 1)
                       stop("Labels for Hyperedges must be character vectors of length 1")
                     object@label <- value
                     object
                 })


setMethod("show", signature(object="Hyperedge"),
          function(object) {
              cat(paste("A", class(object)[1]), "containing",
                  length(nodes(object)), "nodes.\n")
          })


## DirectedHyperedge methods
setMethod("initialize", "DirectedHyperedge",
          function(.Object, head, tail, label="") {
              .Object@label <- label
              .Object@head <- as.character(head)
              .Object@tail <- as.character(tail)
              .Object
          })
DirectedHyperedge <- function(head, tail, label="")
  new("DirectedHyperedge", head=head, tail=tail, label=label)


setMethod("nodes", signature(object="DirectedHyperedge"), function(object) {
    c(object@tail, object@head)
})


setMethod("toUndirected", signature(.Object="DirectedHyperedge"),
          function(.Object) {
              new("Hyperedge", nodes=nodes(.Object), label=label(.Object))
          })
          

setMethod("head", signature(x="DirectedHyperedge"),
          function(x) x@head)


setMethod("tail", signature(x="DirectedHyperedge"),
          function(x) x@tail)


setMethod("show", "DirectedHyperedge", function(object) {
    callNextMethod()
    cat(length(tail(object)), "nodes in the tail and ")
    cat(length(head(object)), "nodes in head.\n")
})
