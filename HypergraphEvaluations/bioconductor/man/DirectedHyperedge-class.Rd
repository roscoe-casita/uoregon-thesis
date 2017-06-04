\name{DirectedHyperedge-class}
\docType{class}
\alias{DirectedHyperedge-class}
\alias{initialize,DirectedHyperedge-method}
\alias{nodes,DirectedHyperedge-method}
\alias{show,DirectedHyperedge-method}
\alias{head}
\alias{head,DirectedHyperedge-method}
\alias{tail}
\alias{tail,DirectedHyperedge-method}
\alias{toUndirected}
\alias{toUndirected,DirectedHyperedge-method}

\title{Class DirectedHyperedge}
\description{This class represents directed hyperedges in a
  \code{\link{Hypergraph-class}}.  A directed hyperedge consists of two
  disjount sets of nodes, those in the tail and those in the head of the
  hyperedge.  Directed hyperedges are sometimes called hyperarcs.
}
\section{Objects from the Class}{
Objects can be created by calls of the form \code{new("DirectedHyperedge", head, tail, label)}.
You can also use the convenience function \code{\link{DirectedHyperedge}}.
}
\section{Slots}{
  \describe{
    \item{\code{tail}:}{Character vector of nodes in the tail of the hyperedge}
    \item{\code{head}:}{Character vector of nodes in the head of the hyperege}
    \item{\code{label}:}{Character string describing the directed hyperedge}
  }
}
\section{Extends}{
Class \code{"Hyperedge"}, directly.
}
\section{Methods}{
  \describe{
    \item{head}{\code{signature(x = "DirectedHyperedge")}: Return a
      vector containing the nodes in the head of the hyperedge}

    \item{tail}{\code{signature(x = "DirectedHyperedge")}: Return a
      vector containing the nodes in the tail of the hyperedge}

    \item{initialize}{\code{signature(.Object = "DirectedHyperedge")}:
      Create a new instance.}

    \item{nodes}{\code{signature(object = "DirectedHyperedge")}: Return
      a vector containing all nodes present in the hyperedge.}

    \item{show}{\code{signature(object = "DirectedHyperedge")}: Print me }

    \item{toUndirected}{\code{signature(.Object = "DirectedHyperedge")}:
      Return a \code{\link{Hyperedge-class}} object that results from
      coercing to an undirected hyperedge.}
  }
}
\author{Seth Falcon}
\seealso{
  \code{\link{DirectedHyperedge}}
  \code{\link{Hyperedge}}
  \code{\link{Hyperedge-class}}
  \code{\link{Hypergraph-class}}
}
\examples{
head <- LETTERS[1:4]
tail <- LETTERS[19:21]
label <- "Directed hyperedge"
dhe <- new("DirectedHyperedge", head=head, tail=tail, label=label)
}
\keyword{classes}
