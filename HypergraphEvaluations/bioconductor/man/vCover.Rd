\name{vCoverHypergraph}
\alias{vCoverHypergraph}
\title{Approximate minimum weight vertex cover in a hypergraph}
\description{Approximate minimum weight vertex cover in a hypergraph }
\usage{
vCoverHypergraph(hg, vW=rep(1, numNodes(hg)))
}
\arguments{
  \item{hg}{an instance of the \code{Hypergraph} class }
  \item{vW}{vertex weights}
}
\details{
Hypergraph \code{g} has non-negative weights on its vertices.
The minimum weight vertex cover problem is to find a subset of vertices C 
such that C includes at least one vertex from each hyperedge and the sum of 
the weights of the vertices in C is minimum.  This problem is NP-hard.

We implement the greedy algorithm to approximate near-optimal solution, 
proposed by E. Ramadan, A. Tarafdar, A. Pothen, 2004.

}
\value{
A list of vertices from hypergraph \code{g}.
}

\references{
A hypergraph model for the yeast protein complex network,  Ramadan, E.   Tarafdar, A.   Pothen, A., Parallel and Distributed Processing Symposium, 2004. Proceedings. 18th International.
}

\author{Li Long <li.long@isb-sib.ch>}

\examples{
# to turn the snacoreex.gxl graph (from RBGL package) to a hypergraph
# this is a rough example 
kc_hg_n <- c("A", "C", "B", "E", "F", "D", "G", "H", "J", "K", "I", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U")
kc_hg_e <- list(c("A", "C"), c("B", "C"), c("C", "E"), c("C", "F"), c("E", "D"), c("E", "F"), c("D", "G"), c("D", "H"), c("D", "J"), c("H", "G"), c("H", "J"), c("G", "J"), c("J", "M"), c("J", "K"), c("M", "K"), c("M", "O"), c("M", "N"), c("K", "N"), c("K", "F"), c("K", "I"), c("K", "L"), c("F", "I"), c("I", "L"), c("F", "L"), c("P", "Q"), c("Q", "R"), c("Q", "S"), c("R", "T"), c("S", "T"))
kc_hg_he <- lapply(kc_hg_e, "Hyperedge")
kc_hg <- new("Hypergraph", nodes=kc_hg_n, hyperedges=kc_hg_he)

vCoverHypergraph(kc_hg)

}

\keyword{ models }
