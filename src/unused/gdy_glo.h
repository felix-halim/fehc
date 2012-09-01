#ifndef _HISTOGRAM_GDY_GLO_
#define	_HISTOGRAM_GDY_GLO_

#include "gdy_sol.h"

void GDY_Glo(Solution &sol, int B){
	GDY_SOL gsol(&sol);
	gsol.createPartitions(B,'U');
	gsol.initializeGetAndPutQueue();
	gsol.populatePartitions(sol.pars);
	for (LD terr = 1e100; B>1; ){
		vector<int> arr;
		FORE(i,gsol.left) arr.push_back(i->first);
		for (int i=0; i<(int)arr.size(); i++)
			swap(arr[i],arr[rand()%arr.size()]);
		for (int i=0; i<(int)arr.size(); i++){
			int g = arr[i];
			if (g==0 || g==sol.n) continue;

			gsol.removePartition(g);

			GDY_Node p;
			gsol.getBestToPutPartition(p);
			if (!(p.L < p.M && p.M < p.R)) goto finish;

			gsol.addPartition(p.M, p.L,p.R);

			vector<int> pars;
			gsol.populatePartitions(pars);
			LD nerr = sol.total_error(pars);
			LD eps = nerr * 1e-9;
			if (nerr + eps > terr) goto finish;
			sol.pars = pars;
			terr = nerr;
		}
	}

	finish:;
}

#endif
