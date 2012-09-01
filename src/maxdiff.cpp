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

void run(Solution &sol){
	sprintf(sol.name,"MaxDiff");	// set the name of the running algorithm

	typedef pair<LD,int> pdi;
	priority_queue<pdi, vector<pdi>, greater<pdi> > pq;

	// insert the first B-1 partition positions
	for (int i=1; i<sol.B; i++){
		LD diff = fabs(sol.v[i-1] - sol.v[i]);
		pq.push(make_pair(diff,i-1));
	}

	// scan the rest of the lements and keep track 
	// the B-1 biggest difference of adjacent partitions
	for (int i=sol.B; i<sol.n; i++){
		LD diff = fabs(sol.v[i-1] - sol.v[i]);	// adjacent partition difference
		pq.push(make_pair(diff,i-1));
		LD rem = pq.top().first; pq.pop();
		assert(diff >= rem); // what we removed should be no greater than what we added
	}

	assert((int)pq.size() == sol.B-1); 

	// generate the actual partitions
	sol.pars.clear();
	sol.pars.push_back(0);			// the first 
	sol.pars.push_back(sol.n);		// and the last partitions are fixed to 0 and n
	for (int i=1; i<sol.B; i++)		// fetch the rest of B-1 partitions from the priority queue
		sol.pars.push_back(pq.top().second), pq.pop();
	sort(sol.pars.begin(),sol.pars.end());
}
