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
#include "gdy_sol.h"

void run(Solution &sol){
	sprintf(sol.name,"MHIST");	// set the name of the running algorithm
	GDY_SOL gsol(&sol);
	gsol.createPartitions(1,'U'); // we start from 1 whole partition
	gsol.initializeGetAndPutQueue();
	
	// then we split them B-1 times greedily reducing SSE each split
	for (int i=0; i<sol.B-1; i++){
		GDY_Node p;
		bool ok = gsol.getBestToPutPartition(p);
		assert(ok);
		gsol.addPartition(p.M, p.L, p.R);
	}
	gsol.populatePartitions(sol.pars);
	assert((int)sol.pars.size() == sol.B+1);
}
