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


// This is where the configurations and the solutions will be stored.
class Solution { public:
	map<long long, int> memo;
	LD *sum, *sqsum, *v;

	// input parameters
	char name[100];	// the name of the algorithm being run on this solution
	int n;			// the number of elements in the input data sequence
	int B;			// the number of buckets generated
	int I;			// the number iterations for GDY*
	LD epsilon;		// the AHistL's epsilon
	bool prune;		// the Dynamic Programming's simple pruning enabled?
	int memo_hit, split_count;	// counter for how many cache hits and splits
	vector<int> pars, samples;


	// Constructs a solution with input data sequence with n elements
	// This also precalculates the sum and square sum in O(n)
	void init(LD *V, int N){
		v = V;
		n = N;
		memo_hit = 0;
		sum = new LD[n+1];
		sqsum = new LD[n+1];
		for (int i=1; i<=n; i++){
			sum[i] += sum[i-1] + v[i-1];
			sqsum[i] = sqsum[i-1] + v[i-1]*v[i-1];
		}
		pars.clear();
		samples.clear();
	}

	// returns the square error of interval [a..b] inclusive
	LD sqerror(int a, int b){
		if (a>=b) return 0;
		LD x = sqsum[b] - sqsum[a-1];
		LD y = sum[b] - sum[a-1];
		return x - y * y / (b - a + 1.0);
	}

	// M is the position of the partition that we want to take out
	// L is the position of the partition immediately to the left of M
	// R is the position of the partition immediately to the right of M
	// Returns the cost of taking out a partition at position M
	// with L and R as the left and right position boundary
	LD get_cost(int L, int M, int R){
		return sqerror(L+1,R) - sqerror(L+1,M) - sqerror(M+1,R);
	}

	// Optimally split an interval [x..y] into two and returns the split position k
	// The split position k divides the [x..y] into [x..k] and [(k+1)..y]
	// memoization can help a little bit for multiple iterations of GDY
	int split_pos(int x, int y){
		long long key = (((long long) x) << 30LL) + y;
		if (memo.count(key)){ memo_hit++; return memo[key]; } else split_count++;
		int b = x;
		for (int i=x+1; i<y; i++)
			if (sqerror(x,i) + sqerror(i+1,y) < sqerror(x,b) + sqerror(b+1,y))
				b = i;
		return memo[key] = b;
	}

	// Returns the total error of this solution's partition
	LD total_error(){ return total_error(pars); }
	LD total_error(vector<int> const &ps){
		LD ret = 0;
		for (int i=1,L=ps[0]; i<(int)ps.size(); i++){
			int R = ps[i];
			assert(L<=R);
			ret += sqerror(L+1,R);
			L = R;
		}
		return ret;
	}
};

