/*
 * spla_test.cpp: (c) David R. Morrison, 2014
 *
 * Test functions for the sparse linear algebra library
 */

#include "spmat.h"

#include <cstdio>

using namespace std;
using namespace spla;

const int rows = 1000;
const int cols = 1000;
const double valmax = 100;
const double sparsity = 0.1;

int main(int argc, char* argv[])
{
	SpMatData my_data;
	srand48(time(nullptr));

	// Generate a random sparse matrix 
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j)
			if (drand48() < sparsity)
				my_data[{i, j}] = drand48() * valmax;

	SpMat test_matrix(my_data);
}
