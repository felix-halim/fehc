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

#include <stdio.h>
#include <math.h>
#include <sys/times.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <queue>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include "random.h"

#define ALL(x) x.begin(), x.end()
#define UNIQUE(x) sort(ALL(x)); x.erase(unique(ALL(x)),x.end())
#define FORE(i,a) for (__typeof(a.begin()) i=a.begin(); i!=a.end(); i++)

using namespace std;

typedef long double LD;
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef vector<LD> VD;
typedef vector<VD> VVD;

#include "solution.h"

Solution sol;

void die(char const *msg){ puts(msg); exit(0); }

// to be implemented by the algorithm
void run(Solution &sol);

int main(int argc, char *argv[]){
	char *opts = (char*) "B:e:I:P"; 
	int n, c;

	sol.prune = true;	// default using Optimized VOpt (early break)
	while ((c = getopt(argc,argv,opts)) != -1) switch (c){
		case 'B': sol.B = atol(optarg); break;
		case 'e': sol.epsilon = atof(optarg); break;
		case 'I': sol.I = atol(optarg); break;
		case 'P': sol.prune = false; break;
		default : die("bad parameters");
	}

	scanf("%d",&n);
	LD *arr = new LD[n];
	for (int i=0; i<n; i++) scanf("%Lf",&arr[i]);
	sol.init(arr,n);

	tms start_time, end_time;
	times(&start_time);

		run(sol);

	times(&end_time);

	if (sol.B+1 != (int) sol.pars.size())
		die("The algorithm returns with incorrect number of partitions!");

	LD terr = sol.total_error();

	printf("{\"algorithm\":\"%s\"",sol.name);
	printf(",\"n\":%d",sol.n);
	printf(",\"utime\":%.6lf", 1.0 * (end_time.tms_utime - start_time.tms_utime) / sysconf(_SC_CLK_TCK));
	printf(",\"stime\":%.6lf", 1.0 * (end_time.tms_stime - start_time.tms_stime) / sysconf(_SC_CLK_TCK));
	printf(",\"total_error\":%.9Lf", terr);
	printf(",\"euclidean_error\":%.9lf", sqrt(terr/sol.n));
	printf(",\"segments\":[");
	FORE(i,sol.pars){
		if (i != sol.pars.begin()) printf(",");
		printf("%d",*i);
	}
	printf("],\"samples\":[");
	FORE(i,sol.samples){
		if (i != sol.samples.begin()) printf(",");
		printf("%d",*i);
	}
	printf("],\"memo_hit\":%d",sol.memo_hit);
	printf(",\"split_count\":%d}\n",sol.split_count);
}
