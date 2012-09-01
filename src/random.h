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

#include <assert.h>

// This implementation is taken from java.util.Random with slight modifications

class Random {
    static const long long multiplier = 0x5DEECE66DLL;
    static const long long addend = 0xBLL;
    static const long long mask = (1LL << 48) - 1;
	unsigned long long seed;

public:
	Random(){ setSeed(140384); }
	Random(long long s){ setSeed(s); }

	void setSeed(long long s) {
		seed = (s ^ multiplier) & mask;
	}

	int next(int bits) {
		seed = (seed * multiplier + addend) & mask;
		return (int)(seed >> (48 - bits));
	}

	int nextInt() {
		return next(32);
	}

	int nextInt(int n) {
		assert(n > 0);
		if ((n & -n) == n)  // i.e., n is a power of 2
			return (int)((n * (long long)next(31)) >> 31);
		int bits, val;
		do {
			bits = next(31);
			val = bits % n;
		} while (bits - val + (n-1) < 0);
		return val;
	}

	long long nextLong() {
		return ((long long)(next(32)) << 32) + next(32);
	}

	bool nextBoolean() {
		return next(1) != 0;
	}

	float nextFloat() {
		return next(24) / ((float)(1 << 24));
	}

	double nextDouble() {
		return (((long long)(next(26)) << 27) + next(27)) / (double)(1LL << 53);
	}
};
