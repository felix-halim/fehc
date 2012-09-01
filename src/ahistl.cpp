// FEHC (Fast and Effective Histogram Construction) is a program 
// to evaluate the efficiency and effectiveness of several
// histogram construction (or sequence segmentation) algorithms
// as mentioned in these two papers:
//
// (1). Felix Halim, Panagiotis Karras, Roland H. C. Yap: 
//      Fast and effective histogram construction. CIKM 2009.
// (2). Felix Halim, Panagiotis Karras, Roland H. C. Yap: 
//      Local Search in Histogram Construction. AAAI 2010.
//
// Copyright (C) 2009-2012 Felix Halim
//
// FEHC is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation version 3 of the License.
//
// FEHC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License <http://www.gnu.org/licenses/> for more details.


#include "tester.h"

// My implementation of AHistL is based on this Journal Paper:
//
// Approximation and Streaming Algorithms for Histogram Construction Problems 
// by Sudipto Guha, Nick Koudas, and Kyuseok Shim
//
// http://www.cis.upenn.edu/~sudipto/mypapers/histjour.pdf

class AHistL_Tuple { public :
	int b;
	LD apxerrb, apxerra;
	AHistL_Tuple(int bb, LD apxe){
		b = bb;
		apxerra = apxerrb = apxe;
	}
};

typedef vector<list<AHistL_Tuple> > AHistL_Queue;

LD AHistL_apxerr(AHistL_Queue &Q, Solution &sol, int i, int k){
	LD ret = sol.sqerror(1,i);
	bool first = true;

	FORE(t,Q[k-1]){
		if (t->b + 1 > i){
			if (first) first = false;
			else break;
		}
		ret = min(ret, t->apxerrb + sol.sqerror(t->b + 1, i));
	}
	return ret;
}

LD AHistL_createBestList(AHistL_Queue &Q, Solution &sol, int start, int end, int k, LD cutOff, LD p, LD z){
	LD err = AHistL_apxerr(Q,sol,start,k);
	if (err >= cutOff) return cutOff;
	while (start < end){
		int mid = (start + end + 1) / 2;
		cutOff = AHistL_createBestList(Q, sol, mid, end, k, cutOff, p, z);
		if (err >= cutOff) return cutOff;
		end = mid-1;
	}
	if (err < cutOff){
		Q[k].push_front(AHistL_Tuple(start,err));
		cutOff = (err-z)/p;
	}
	return cutOff;
}

LD AHistL_sub(AHistL_Queue &Q, Solution &sol, int n, int B, LD maxEst, LD p, LD z){
	for (int k=1; k<B; k++){
		Q[k].clear();
		AHistL_createBestList(Q,sol,1,n,k,maxEst,p,z);
	}
	return AHistL_apxerr(Q,sol,n,B);
}

int AHistL_parent(AHistL_Queue &Q, Solution &sol, int i, int k){
	LD ret = sol.sqerror(1,i);
	int parent = -1;
	bool first = true;
	FORE(t,Q[k-1]){
		if (t->b+1 > i){
			if (first) first = false;
			else break;
		}
		LD e = t->apxerrb + sol.sqerror(t->b+1,i);
		if (e + 1e-9 < ret){
			ret = e;
			parent = t->b;
		}
	}
	return parent;
}


void run(Solution &sol){
	sprintf(sol.name,"AHistL%.0Lf",sol.epsilon*100);	// set the name of the running algorithm

	int n = sol.n, B = sol.B;
	AHistL_Queue Q(B+1, list<AHistL_Tuple>());

	LD cutoff = 0, z = 0, p = 1 + sol.epsilon/(2*B);
	LD E = AHistL_sub(Q,sol,n,B,cutoff,p,z);
	if (E>0){
		LD D = 0.5;
		cutoff = 4*D;
		z = D/(2.0*B);
		E = AHistL_sub(Q,sol,n,B,cutoff,p,z);
		while (E >= 4*D){
			D *= 2;
			cutoff = 4*D;
			z = D/(2.0*B);
			E = AHistL_sub(Q,sol,n,B,cutoff,p,z);
		}
		cutoff = E;
		z = sol.epsilon * D / (2*B);
		E = AHistL_sub(Q,sol,n,B,cutoff,p,z);
	}

	sol.pars.clear();
	sol.pars.push_back(0);
	for (int i=n,j=B; j>0; j--){
		sol.pars.push_back(max(0,i));
		i = AHistL_parent(Q,sol,i,j);
	}
	sort(sol.pars.begin(),sol.pars.end());
	assert(fabs(E - sol.total_error()) < 1e-5);
}
