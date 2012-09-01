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

#define EPS 1e-9

// This is the core of our GDY local search

class GDY_Node { public :
	int L, M, R;
	long double cost;
	GDY_Node(){}
	GDY_Node(int l, int m, int r, long double c):L(l),M(m),R(r),cost(c){}
	bool operator<(GDY_Node const &that) const { return cost > that.cost; }
};

class GDY_SOL { public:
	priority_queue<GDY_Node> getQueue, putQueue;
	map<int,int> left, right;
	Solution *sol;
	long double error;
	Random rnd;

	GDY_SOL(Solution *s):sol(s){}

	void createPartitions(vector<int> &pars){
		error = 0;
		left.clear();
		right.clear();
		for (int i=1; i<(int)pars.size(); i++){
			int L = pars[i-1], R = pars[i];
			assert(L<=R);
			left[R] = L; right[L] = R;
			error += sol->sqerror(L+1,R);
		}
	}

	void createPartitions(int B, int type){
		vector<int> pars;
		if (type=='U'){ // uniform
			pars.push_back(0);
			for (int i=1; i<=B; i++)
				pars.push_back((long long) i*sol->n / B);
		} else if (type=='R'){ // random
			for (int i=1; i<sol->n; i++)
				pars.push_back(i);
			for (int i=0; i<B; i++)
				swap(pars[i],pars[rnd.nextInt(pars.size())]);
			pars.erase(pars.begin()+B-1,pars.end());
			pars.push_back(0);
			pars.push_back(sol->n);
			sort(pars.begin(), pars.end());
		} else assert(false);
		createPartitions(pars);
		assert((int)left.size()==B);
	}

	void initializeGetAndPutQueue(){
		while (getQueue.size()>0) getQueue.pop();
		while (putQueue.size()>0) putQueue.pop();
		FORE(it,left){
			int R = it->first;
			int L = it->second;
			if (R<sol->n) addGetQueue(R);
			addPutQueue(L, R);
		}
	}

	/**
	 * Consider a segment [L .. R] as the segment to be split.
	 * The cost and the best splitting position of the segment [L .. R]
	 * is computed if it's not in the cache.
	 */
	void addPutQueue(int L, int R){
		if (L+1>=R) return; // no place to split
		int M = sol->split_pos(L+1,R);
		putQueue.push(GDY_Node(L,M,R,-sol->get_cost(L,M,R)));
	}

	/**
	 * Consider a partition M to be taken out.
	 */
	void addGetQueue(int M){
		if (M==0 || M==sol->n) return;
		assert(left.count(M));
		int L = left[M], R = right[M];
		getQueue.push(GDY_Node(L,M,R,sol->get_cost(L,M,R)));
	}

	bool getBestToGetPartition(GDY_Node &g){
		while (getQueue.size()>0){ // O(C)
			g = getQueue.top(); getQueue.pop();
			if (left.count(g.M) && right.count(g.M))
				if (left[g.M]==g.L && right[g.M]==g.R)
					return true;
		}
		return false;
	}


	bool getBestToPutPartition(GDY_Node &p){
		while (putQueue.size()>0){ // O(C)
			p = putQueue.top(); putQueue.pop();
			if (left.count(p.R) && left[p.R]==p.L){
				assert(right[p.L] == p.R);
				return true;
			}
		}
		return false;
	}


	/**
	 * Removes partition g and updates the putQueue O(log B).
	 */
	void removePartition(int g){
		int L = left[g], R = right[g];
		left.erase(g);
		right.erase(g);
		error += sol->get_cost(L,g,R);
		left[R] = L;
		right[L] = R;
		addGetQueue(L);
		addGetQueue(R);
		addPutQueue(L,R);
	}

	/**
	 * Adds partition p in between [L .. R] and updates the putQueue O(log B).
	 */
	void addPartition(int p, int L, int R){
		assert(L<p && p<R && left[R]==L && right[L]==R);
		assert(!left.count(p) && !right.count(p));
		error -= sol->get_cost(L,p,R);
		left[p] = L;
		right[p] = R;
		left[R] = p;
		right[L] = p;
		addGetQueue(L);
		addGetQueue(p);
		addGetQueue(R);
		addPutQueue(L,p);
		addPutQueue(p,R);
	}

	void whileGetPut(){
		initializeGetAndPutQueue();
		while (getQueue.size()>0){
			GDY_Node g;
			if (!getBestToGetPartition(g)) break;
			removePartition(g.M);
			GDY_Node p;
			getBestToPutPartition(p);
			if (g.cost + p.cost >= -EPS){
				error -= g.cost;
				left[g.M] =  g.L;
				right[g.L] =  g.M;
				left[g.R] =  g.M;
				right[g.M] =  g.R;
				addPutQueue(g.L, g.M);
				addPutQueue(g.M, g.R);
			} else {
				addPartition(p.M, p.L, p.R);
			}
		}
	}

	void whilePutGet(){
		initializeGetAndPutQueue();
		while (putQueue.size()>0){
			GDY_Node p;
			if (!getBestToPutPartition(p)) break;
			addPartition(p.M, p.L, p.R);
			GDY_Node g;
			getBestToGetPartition(g);
			if (g.cost + p.cost >= -EPS){
				left.erase(p.M);
				right.erase(p.M);
				left[p.R] =  p.L;
				right[p.L] =  p.R;
				addGetQueue(p.L);
				addGetQueue(p.R);
			} else {
				removePartition(g.M);
			}
		}
	}

	void collectSamples(int B, int I){
		vector<int> &S = sol->samples, &P = sol->pars;

		// first sample is from uniform partitions
		createPartitions(B,'U');
		whileGetPut();
		populatePartitions(P);
		S = P;

		// the rest of the samples are from randomized partitions
		for (int i=1; i<I; i++){
			createPartitions(B,'R');
			whileGetPut();
			populatePartitions(P);
			S.insert(S.end(),P.begin(),P.end());
		}

		UNIQUE(S); // the samples is always unique and sorted
	}

	/**
	 * Returns an array containing the partitions.
	 * The position of the first and the last partition is 0 and n.
	 * In between there will be B-1 other partitions.
	 * The array returned will be sorted.
	 * @return The list of partitions with B+1 elements.
	 */
	void populatePartitions(vector<int> &par){
		par.clear();
		par.push_back(0);
		FORE(i,left) par.push_back(i->first);
	}
};
