\name{kCoresHypergraph}
\alias{kCoresHypergraph}
\title{Find all the k-cores in a hypergraph}
\description{Find all the k-cores in a hypergraph }
\usage{
kCoresHypergraph(hg)
}
\arguments{
  \item{hg}{an instance of the \code{Hypergraph} class }
}
\details{
A k-core in a hypergraph is a maximal subhypergraph where (a) no hyperedge 
is contained in another, and (b) each node is adjacent to at least \code{k} 
hyperedges in the subgraph.

The implementation is based on the algorithm by E. Ramadan, A. Tarafdar, A. Pothen, 2004.

}
\value{
A vector of the core numbers for all the nodes in \code{g}.
}

\references{
A hypergraph model for the yeast protein complex network,  Ramadan, E.   Tarafdar, A.   Pothen, A., Parallel and Distributed Processing Symposium, 2004. Proceedings. 18th International.
}

\author{Li Long <li.long@isb-sib.ch>}

\examples{
# to turn the snacoreex.gxl (from RBGL package) graph to a hypergraph
# this is a rough example 
kc_hg_n <- c("A", "C", "B", "E", "F", "D", "G", "H", "J", "K", "I", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U")
kc_hg_e <- list(c("A", "C"), c("B", "C"), c("C", "E"), c("C", "F"), c("E", "D"), c("E", "F"), c("D", "G"), c("D", "H"), c("D", "J"), c("H", "G"), c("H", "J"), c("G", "J"), c("J", "M"), c("J", "K"), c("M", "K"), c("M", "O"), c("M", "N"), c("K", "N"), c("K", "F"), c("K", "I"), c("K", "L"), c("F", "I"), c("I", "L"), c("F", "L"), c("P", "Q"), c("Q", "R"), c("Q", "S"), c("R", "T"), c("S", "T"))
kc_hg_he <- lapply(kc_hg_e, "Hyperedge")
kc_hg <- new("Hypergraph", nodes=kc_hg_n, hyperedges=kc_hg_he)

kCoresHypergraph(kc_hg)

}

\keyword{ models }
