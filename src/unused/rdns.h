#ifndef _HISTOGRAM_RDNS_
#define	_HISTOGRAM_RDNS_

#include "vopt.h"

void RDnS(Solution &sol, int B, bool prune, int left, int right, vector<int> &pars){
	int n = right-left, X = (int) (sqrt(n) + 1.001);

	if (n/X <= (B*3)/2){ // from Terzi's code
		vector<int> pos;
		for (int j=left; j<=right; j++) pos.push_back(j);
		VOpt(sol,B,pos,pars,prune);
	} else {
		int pSize = n / X;
		vector<int> samples;
		for (int i=0; i<X; i++){
			int L = left + i*pSize, R = L+pSize; // L = left, R = right of the i'th sub-partition
			if (i+1==X) R = right;
			RDnS(sol,B,prune,L,R,pars);
			samples.insert(samples.end(),pars.begin(),pars.end());
		}
		UNIQUE(samples);
		VOpt(sol,B,samples,pars,prune); // DP on all of the combined sub-partitions
	}
	//sol.samples.insert(sol.samples.end(),pars.begin(),pars.end());
}

// entry point
void RDnS(Solution &sol, int B, bool prune){
	//sol.samples.clear();

	RDnS(sol,B,0,prune,sol.n,sol.pars);

	//UNIQUE(sol.samples);
}

#endif
