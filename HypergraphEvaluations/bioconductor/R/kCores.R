#   
# Reference:
#   A hypergraph model for the yeast protein complex network
# By E. Ramadan, A. Tarafdar, A. Pothen
#   Procs. Workshop High Performance Computational Biology, IEEE/ACM 2004
#   
# algorithm for computing the k-core of a hypergraph:
# ===================================================
#   
#   while there are vertices with degree < k do
#   {
#      for each such vertex v do
#      {
#         for each hyperedge f associated with v do
#         {
#            delet v from adj(f)
#            decrement d(f) by 1
#   	     if f is non-maximal then
#            {
#   	        for each vertex w associated with f do
#   	        {
#                  delete f from adj(w)
#                  decrement d(w) by 1
#                  if ( d(w) < k then
#                  {
#                     include w in list of vertices with degree < k
#   	           }
#               }
#            }
#         }
#      }
#   }
#   
kCoresHypergraph <- function(hg)
{
	nv <- numNodes(hg)
	core <- array(0, nv, dimnames = list(nodes(hg)))

	im <- inciMat(hg)
	ne <- ncol(im)

	v_deg <- sort(rowSums(im))
	k_num <- 0
	for ( i in 1:nv )
	{
		v <- names(v_deg)[i]
		k_num <- max(v_deg[v], k_num)
		core[v] <- k_num

		# v's hyperedges
		he_set <- which(im[v,] == 1)
		im[v, he_set] <- 0

		# remove non-maximal hyperedges
		# (1) selective approach
		for ( f in names(he_set) )
		{
			# hyperedges adjacent to f
			r_chosen <- which(im[, f] == 1)
			c_chosen <- which(im[r_chosen, ] > 0)
			im_sub <- matrix(im[r_chosen, c_chosen], nrow=length(r_chosen), ncol=length(c_chosen))
			rownames(im_sub) <- names(r_chosen)
			colnames(im_sub) <- names(c_chosen)

			for ( g in names(c_chosen) )
				if ( f != g && im_sub[, f] == im_sub[, g] )
				{
					im[, f] <- 0
				}
		}
		v_deg <- sort(rowSums(im))

##	# (2) brute-force approach 
##      for ( f in he_set )
##	{
##	   for ( g in 1:ne )
##	   if ( f != g && sum(im[, f] & im[, g]) == sum(im[, f]) )
##	   {
##	      im[, f] <- 0
##	   }
##	}
##	v_deg <- sort(rowSums(im))

	}
	core
}
#   
# greedy algorithm for computing an approximate minimum weight vertex 
# cover of a hypergraph
# ===================================================================
#   
# F[i] is the set of hy[eredges not yet covered by a partial vertex cover
# at the begining of the i-th iteration
#
# cost function alpha(v) = w(v) / | adj(v) intersect F[i] |
# which distributes the weight of the vertex equally among the hyperedges 
# it belongs to that are currently uncovered.
#
# at each step, it chooses a vertex with minimum cost alpha(v) to include 
# in the partial cover, deletes all hyperedges it covers
#
#   initialize:
#      i = 1; // iteration number
#      C = 0; // cover
#      F[1] = F;
#   // hyperedges yet to be covered
#   while F[i] != 0 do
#   {
#      for ( v in V - C ) do
#      {
#         choose a vectex v[i] with min cost alpha(v);
#         add v[i] to the cover C;
#         F[i+1] = F[i] - adj(v[i]);
#         i = i+1;
#      }
#   }
#   

vCoverHypergraph <- function(hg, vW=rep(1, numNodes(hg)))
{
	V <- nodes(hg)
	im <- inciMat(hg)

	names(vW) <- V
	deg <- rowSums(im)
	C <- names(which(deg == 0))
	F <- setdiff(V, C)

	while ( length(F) > 1 ) 
	{
		# choose a vectex v[i] with min cost alpha(v)
		deg <- rowSums(im)
		vW_cur <- vW / deg
		v <- names(which.min(vW_cur))

		C <- c(C, v)

		adj_he <- names(which(im[v,] == 1))

		im[v, ] <- 0
		im[, adj_he] <- 0

		r_chosen <- names(which(rowSums(im) > 0))
		c_chosen <- names(which(colSums(im) > 0))
		im <- im[r_chosen, c_chosen, drop = FALSE]

		vW <- vW[r_chosen]
		F <- r_chosen
	}
	C
}

