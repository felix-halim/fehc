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
#include "vopt.h"

void GDY_Batched_DP(Solution &sol, int batch_size, VI &pars, VI &samples){
	for (int Ls=0,Lg=0; Ls+1<(int)samples.size(); ){
		int Rs = min((int)samples.size()-1, Ls + batch_size);
		int Rg = Lg;
		while (pars[Rg] < samples[Rs]) Rg++; // off by one, it's ok
		while (samples[Rs] < pars[Rg]) Rs++;

		VI subPos, optSubPar;
		for (int i=Ls; i<=Rs; i++)
			subPos.push_back(samples[i]);

		int subB = Rg - Lg;
		VOpt(sol,subB,subPos,optSubPar,true);

		// replace the current sub pars with the optimal sub par
		for (int i=0; i<=subB; i++)
			pars[Lg+i] = optSubPar[i];

		Ls = Rs;
		Lg = Rg;
	}
}

void run(Solution &sol){
	sprintf(sol.name,"GDY_%d_DP",sol.I);	// set the name of the running algorithm
	GDY_SOL gsol(&sol);
	gsol.collectSamples(sol.B, sol.I);

	VI &aux = sol.pars;	// the auxiliary solution
	gsol.createPartitions(sol.B,'U');		// the auxiliary solution starts with uniform partitions
	gsol.whileGetPut();					// improved using basic greedy
	gsol.populatePartitions(aux);		// aux contains a GDY solution

	// add aux to samples
	sol.samples.insert(sol.samples.end(),sol.pars.begin(),sol.pars.end());
	UNIQUE(sol.samples);

	int sqrtn = (int) sqrt(sol.n) + 1;
	
	/*
	GDY_Batched_DP(sol, (sqrtn*4)/5, aux, sol.samples);
	GDY_Batched_DP(sol, (sqrtn*3)/4, aux, sol.samples);
	GDY_Batched_DP(sol, (sqrtn*2)/3, aux, sol.samples);
	GDY_Batched_DP(sol, sqrtn/2, aux, sol.samples);
	*/
	GDY_Batched_DP(sol, sqrtn, aux, sol.samples);
}
