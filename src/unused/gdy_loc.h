#ifndef _HISTOGRAM_GDY_LOC_
#define	_HISTOGRAM_GDY_LOC_

#include "gdy_sol.h"

void GDY_Loc(Solution &sol, int B){
	GDY_SOL gsol(&sol);
	gsol.createPartitions(B,'U');
	gsol.initializeGetAndPutQueue();
	while (true){
		bool update = false;
		vector<int> arr;
		FORE(i,gsol.left) arr.push_back(i->first);
		for (int i=0; i<(int)arr.size(); i++){
			int M = arr[i];
			if (M==sol.n) break;
			int L = gsol.left[M];
			int R = gsol.right[M];
			int k = sol.split_pos(L+1,R);
			if (M==k) continue;
			gsol.removePartition(M);
			gsol.addPartition(k,L,R);
			update = true;
		}
		if (!update) break;
	}
	gsol.populatePartitions(sol.pars);
}

#endif
