/*
 * spla_test.cpp: (c) David R. Morrison, 2014
 *
 * Test functions for the sparse linear algebra library
 */

#include "spmat.h"

#include <unittest++/UnitTest++.h>

using namespace std;
using namespace spla;

const int rows = 1000;
const int cols = 1000;
const double valmax = 99;
const double sparsity = 0.1;

TEST(MatrixConstruction)
{
	SpMatData my_data;
	srand48(time(nullptr));

	// Generate a random sparse matrix 
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j)
			if (drand48() < sparsity)
				my_data[{i, j}] = (int)(drand48() * valmax + 1);

	SpMat test_matrix(my_data);
	SpMat test_matrix_2 = test_matrix;
	CHECK(my_data == test_matrix.data());
	CHECK(my_data == test_matrix_2.data());
}

int main(int argc, char* argv[])
{
	return UnitTest::RunAllTests();
}
