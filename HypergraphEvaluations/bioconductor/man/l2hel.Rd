\name{l2hel}
\alias{l2hel}
\title{Create lists of Hyperedge objects}
\description{
  Conveniently create lists of \code{\link{Hyperedge-class}} instances.
}
\usage{
l2hel(e)
}
\arguments{
  \item{e}{A list of character vectors.  Each element of the list
  represents a hyperedge and the character vector value specifies the
  nodes of the hypergraph that are part of the hyperedge.  The names of
  the list elements, if found, will be used as the label for the
  corresponding Hyperedge object.}
}
\value{
  A list of \code{Hyperedge-class} objects.  If the list \code{e} did
  not have names, the labels of the Hyperedges will be set to its index
  in the list coerced to character.
}
\author{Seth Falcon}
\seealso{
  \code{\link{Hyperedge-class}}
  \code{\link{Hypergraph-class}}
}
\examples{
edges <- list("e1"="A", "e2"=c("A", "B"), "e3"=c("C", "D"))
hEdgeList <- l2hel(edges)
}
\keyword{classes}
