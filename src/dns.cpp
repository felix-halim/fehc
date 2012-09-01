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
#include "vopt.h"

// My implementation of DnS is based on this paper:
//
// Efﬁcient algorithms for sequence segmentation
// by Evimaria Terzi and Panayiotis Tsaparas
//
// http://cs-people.bu.edu/evimaria/papers/TT06.pdf

void run(Solution &sol){
	sprintf(sol.name,"DnS%s",sol.prune?"2":"");	// set the name of the running algorithm
	int X = max(1, (int) (pow((double) sol.n / sol.B, 2.0/3.0) + 1e-9)); // Optimal value for X
	int pSize = sol.n / X;
	sol.samples.clear(); // Combined partitions of the optimal the sub-partitions
	for (int i=0; i<X; i++){
		int L = i*pSize, R = L+pSize; // L = left, R = right of the i'th sub-partition
		if (i+1==X) R = sol.n;  //
		vector<int> pos;
		for (int j=L; j<=R; j++) pos.push_back(j); // the content of the i'th sub partitions
		VOpt(sol,sol.B,pos,sol.pars,sol.prune); // DP on the sub-partition pos
		sol.samples.insert(sol.samples.end(),sol.pars.begin(),sol.pars.end());
	}
	sort(sol.samples.begin(),sol.samples.end());
	sol.samples.erase(unique(sol.samples.begin(),sol.samples.end()),sol.samples.end());
	VOpt(sol,sol.B,sol.samples,sol.pars,sol.prune); // DP on all of the combined sub-partitions
}
