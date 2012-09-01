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

void VOpt_Partitions(VI &pars, VI &pos, VVI &parent, int B){
	int n = pos.size() - 1;
	pars.clear();
	pars.push_back(pos[n]);
	for (int i=n,j=B; parent[i][j]>0; i=parent[i][j--])
		pars.push_back(pos[parent[i][j]]);
	while ((int)pars.size()<B+1) pars.push_back(pos[0]);
	sort(pars.begin(),pars.end());
}

// O(n*n) if prune set to true, otherwise O(n*n*B)
void VOpt_Construct(Solution &sol, int B, VI &pos, bool prune, VVD &error, VVI &parent){
	int n = pos.size() - 1;
//	VVD cnt(n+1, VD(B+1, 0));
//	cnt[0][0] = 1.0;
	for (int i=1; i<=n; i++){
//		cnt[i][1] = 1.0;
		error[i][1] = sol.sqerror(pos[0]+1,pos[i]);
		parent[i][1] = 0;
	}
	for (int i=2; i<=B; i++)
		for (int j=1; j<=n; j++){
			error[j][i] = error[j][i-1];
			parent[j][i] = parent[j][i-1];
			for (int k=j-1; k>0; k--){
				LD last = sol.sqerror(pos[k]+1,pos[j]);
				if (prune && last > error[j][i]) break;
				LD t = error[k][i-1] + last;
				if (t < error[j][i]){
					error[j][i] = t;
					parent[j][i] = k;
				}
			}
			/*
			for (int k=j-1; k>0; k--){
				LD last = sol.sqerror(pos[k]+1,pos[j]);
				LD t = error[k][i-1] + last;
				if (abs(t - error[j][i]) < 1e-9)
					cnt[j][i] += cnt[k][i-1];
			}*/
		}
//	fprintf(stderr,"C=%Lf\n",cnt[n][B]);
}

void VOpt(Solution &sol, int B, VI &pos, VI &pars, bool prune){
	int n = pos.size() - 1;
	VVD error(n+1, VD(B+1, 0));
	VVI parent(n+1, VI(B+1, 0));
	VOpt_Construct(sol,B,pos,prune,error,parent);
	VOpt_Partitions(pars,pos,parent,B);
}

void VOpt(Solution &sol, int B, bool prune){
	VI pos;
	for (int i=0; i<=sol.n; i++) pos.push_back(i);
	VOpt(sol,B,pos,sol.pars,prune);
}

// space efficient O(n)
int VOptMiddleBucket(Solution &sol, int B, VI const &pos, VI &pars, int L, int R){
	int n = R - L + 1;
	VVD terr(2, VD(n));
	VI mid(n, -1);
	for (int i=1; i<=n; i++)
		terr[0][i] = sol.sqerror(pos[L]+1,pos[L+i]);

	int cur = 1;
	for (int i=2; i<=B; i++){
		int prev = 1 - cur;
		for (int j=L+1; j<=R; j++){
			for (int k=j-1; k>L; k--){
				LD last = sol.sqerror(pos[k+1],pos[j]);
				if (last >= terr[cur][j-L]) break; // prune
				LD t = terr[prev][k-L] + last;
				if (t < terr[cur][j-L]){
					terr[cur][j-L] = t;
//					if (k-L < 1/2 n
				}
			}
		}
		cur = prev;
	}

	return 0;
}
