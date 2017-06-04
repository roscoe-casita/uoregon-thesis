\name{Hyperedge-class}
\docType{class}
\alias{Hyperedge-class}
\alias{initialize,Hyperedge-method}

\alias{label}
\alias{label,Hyperedge-method}
\alias{label<-}
\alias{label<-,Hyperedge,character-method}
\alias{nodes,Hyperedge-method}
\alias{show,Hyperedge-method}

\title{Class Hyperedge}
\description{A Hyperedge object represents a hyperedge in a hypergraph,
that is, a subset of the nodes of a hypergraph.
}
\section{Objects from the Class}{
Objects can be created by calls of the form \code{new("Hyperedge",
  nodes, label)}.
You can also use the convenience function \code{Hyperedge} to create
instances.  This is especially useful for creating a list of
\code{Hyperedge} instances using \code{\link{lapply}}.
}
\section{Slots}{
  \describe{
    \item{\code{head}:}{A vector of mode \code{"character"} containing
    the node labels that are a part of the hyperedge}
    \item{\code{label}:}{An arbitrary \code{"character"} string
      describing this hyperedge }
  }
}
\section{Methods}{
  \describe{
    \item{initialize}{\code{signature(.Object = "Hyperedge")}: Create an
    instance}
    \item{label}{\code{signature(object = "Hyperedge")}: Return the
      value of the \code{label} slot }
    \item{label<-}{\code{signature(object = "Hyperedge", value =
    "character")}: Set the label slot.}
    \item{nodes}{\code{signature(object = "Hyperedge")}: Return a
    vector containing the nodes in the hyperedge }
    \item{show}{\code{signature(object = "Hyperedge")}: Print a textual
    summary of the hyperedge }
  }
}
\author{Seth Falcon}

\seealso{
  \code{\link{Hyperedge}}
  \code{\link{Hypergraph-class}}
  \code{\link{DirectedHyperedge-class}}
}
\examples{
nodes <- LETTERS[1:4]
label <- "Simple hyperedge"
## Use the convenience constructor
he <- Hyperedge(nodes, label)
}
\keyword{classes}
