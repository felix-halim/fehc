#ifndef _HISTOGRAM_DNSC_
#define	_HISTOGRAM_DNSC_

#include "vopt.h"

// entry point
void DnSC(Solution &sol, int B, bool prune){
	long long n = sol.n;
	int sqrtn = (int) sqrt(sol.n);

	VVD error[2];
	VVI parent[2];
	VD terr[3];
	VVI pars[3];
	VI pos[3];

	int pre = 0, cur = 1, fut = 2;
	for (int i=1; i<=sqrtn; i++){
		int L = ((i-1)*n) / sqrtn, R = (i*n) / sqrtn, N = R-L;
		pos[cur].clear();
		for (int j=L; j<=R; j++) pos[cur].push_back(j);
		error[cur] = VVD(N+1,VD(B+1,0));
		parent[cur] = VVI(N+1,VI(B+1,0));

		VOpt_Construct(sol,B,pos[cur],prune,error[cur],parent[cur]);
		terr[cur] = VD(B+1,0);
		pars[cur] = VVI(B+1, VI());
		for (int j=1; j<=B; j++){
			terr[cur][j] = error[cur][N][j];
			VOpt_Partitions(pars[cur][j],pos[cur],parent[cur],j);
		}
		if (i==1){ swap(pre,cur); continue; }
		
		// Combine
		pars[fut] = VVI(B+1,VI());
		pars[fut][1].push_back(0);
		pars[fut][1].push_back(R);
		terr[fut] = VD(B+1,0);
		terr[fut][1] = sol.sqerror(1, R);
		for (int b=2; b<=B; b++){
			terr[fut][b] = 1e100;
			for (int j=1; j<b; j++){
				double pe = terr[pre][j], ce = terr[cur][b-j];
				VI &pp = pars[pre][j], &cp = pars[cur][b-j];
				if (pe + ce + EPS < terr[fut][b]){
					terr[fut][b] = pe + ce;
					pars[fut][b] = pp;
					pars[fut][b].insert(pars[fut][b].end(), cp.begin(), cp.end());
				}
			}
		}
		swap(pre,fut);
	}

	sol.pars = pars[pre][B];
	UNIQUE(sol.pars);
}

#endif
