#ifndef _HISTOGRAM_GDY_WDP_
#define	_HISTOGRAM_GDY_WDP_

#include "gdy_sol.h"
#include "vopt.h"

#include <math.h>

void GDY_Windowed_DP(Solution &sol, int X, int Y, vector<int> &A, vector<int> &S){
	int sqrtn = (int) sqrt(sol.n) + 1;
	for (int Ls=0,La=0; La+1<(int)A.size(); ){
		int Rs=Ls, Ra=La;
		
		vector<int> pos;
		while (Ra+1 < (int)A.size() && (Ra<La+X || Rs<Ls+sqrtn)){
			Ra++;
			while (S[Rs] < A[Ra]) pos.push_back(S[Rs++]);
		}	
		pos.push_back(S[Rs]);
		//printf("%d .. %d = %d / %d\n",La,Ra,Ra-La,Rs-Ls);

		int bsize = pos.size();
		if (bsize > 2*sqrtn){
			for (int i=0; i<bsize; i++)
				swap(pos[i], pos[rand()%bsize]);
			pos.erase(pos.begin()+2*sqrtn, pos.end());
			for (int i=La; i<=Ra; i++)
				pos.push_back(A[i]);
			UNIQUE(pos);
		}
		
		int subB = Ra - La;
		vector<int> optSubPar;
		VOpt(sol,subB,pos,optSubPar,true);

		// replace the current sub A with the optimal sub par
		for (int i=0; i<=subB; i++)
			A[La+i] = optSubPar[i];

		La += max(1, ((Ra-La)*Y)/100);
		while (S[Ls] < A[La]) Ls++;
	}
}

void GDY_WDP(Solution &sol, int B, int I, int X, int Y){ // X and Y is not used yet!
	GDY_SOL gsol(&sol);

	gsol.collectSamples(B,I+1);

	// the last run of the sampling as the aux
	vector<int> &aux = sol.pars;	
	
	GDY_Windowed_DP(sol, X,Y, aux, sol.samples);
}

#endif
