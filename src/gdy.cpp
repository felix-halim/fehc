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
	sprintf(sol.name,"GDY");			// set the name of the running algorithm
	GDY_SOL gsol(&sol);
	gsol.createPartitions(sol.B,'U');	// start with a uniform partition
	gsol.whileGetPut();					// improve greedily until stuck to a local optima
	gsol.populatePartitions(sol.pars);	// get the partitions positions
}
