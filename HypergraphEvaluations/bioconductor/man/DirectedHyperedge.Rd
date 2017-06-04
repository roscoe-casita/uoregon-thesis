\name{DirectedHyperedge}
\alias{DirectedHyperedge}
\title{Constructor for DirectedHyperedge objects}
\description{
  A convenience constructor for \code{\link{DirectedHyperedge-class}}
  objects
}
\usage{
DirectedHyperedge(head, tail, label = "")
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{head}{Character vector of nodes that are part of the head of the
  hyperedge}
  \item{tail}{Character vector of nodes that part of the tail of the
    hyperedge}
  \item{label}{A character string describing the directed hyperedge}
}
\value{
  An object of class \code{\link{DirectedHyperedge-class}}
}
\author{Seth Falcon}
\seealso{
  \code{\link{DirectedHyperedge-class}}
  \code{\link{Hyperedge-class}}
  \code{\link{Hypergraph-class}}
}
\keyword{classes}
