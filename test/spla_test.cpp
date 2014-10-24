/*
 * spla_test.cpp: (c) David R. Morrison, 2014
 *
 * Test functions for the sparse linear algebra library
 */

#include "types.h"
#include "spmat.h"
#include "spops.h"

#include <unittest++/UnitTest++.h>

using namespace std;
using namespace spla;

const int rows = 1000;
const int cols = 1000;
const double valmax = 99;
const double sparsity = 0.1;

SpMatData my_data;
SpMat* test_matrix;
SpVec test_vec(cols, 0);
vector<vector<double>> dense_matrix(rows, vector<double>(cols, 0));;
clock_t start;

TEST(MatrixConstruction)
{
	start = clock();
	test_matrix = new SpMat(rows, cols, my_data);
	printf("Matrix construction time: %0.2fs\n", (clock() - start) / (double)(CLOCKS_PER_SEC));

	CHECK(my_data == test_matrix->data());

	SpMatData test1;
	test1[{4,1}] = 5; test1[{2,2}] = 5; test1[{8,2}] = 5; test1[{2,3}] = 5; test1[{5,3}] = 5;
	test1[{6,4}] = 5; test1[{2,5}] = 5; test1[{4,5}] = 5; test1[{8,5}] = 5; test1[{0,6}] = 5;
	test1[{8,7}] = 5; test1[{7,9}] = 5;
	CHECK(test1 == SpMat(10, 10, test1).data());
}

TEST(MatrixCopy)
{
	start = clock();
	SpMat mat_copy(*test_matrix);
	printf("Matrix copy time: %0.2fs\n", (clock() - start) / (double)(CLOCKS_PER_SEC));

	CHECK(my_data == mat_copy.data());
}

TEST(MatrixAssign)
{
	start = clock();
	SpMat mat_assign = *test_matrix;
	printf("Matrix assignment time: %0.2fs\n", (clock() - start) / (double)(CLOCKS_PER_SEC));

	CHECK(my_data == mat_assign.data());
}

TEST(MatVecMultiply)
{

	start = clock();
	SpVec dense_out(rows, 0);
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j)
			dense_out[i] += dense_matrix[i][j] * test_vec[j];
	printf("Multiply time with dense matrix: %0.2fs\n", (clock() - start) / (double)(CLOCKS_PER_SEC));

	start = clock();
	SpVec out1 = *test_matrix * test_vec;
	printf("Multiply time using operator*: %0.2fs\n", (clock() - start) / (double)(CLOCKS_PER_SEC));

	CHECK(out1 == dense_out);
}

TEST(MatrixDestruction)
{
	delete test_matrix;
}


int main(int argc, char* argv[])
{
	int seed = time(nullptr);
	printf("RANDOM SEED: %d\n", seed);
	srand48(seed);

	// Generate a random sparse matrix 
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			if (drand48() < sparsity)
			{
				my_data[{i, j}] = (int)(drand48() * valmax + 1);
				dense_matrix[i][j] = my_data[{i, j}];
			}
		}
	}

	// Generate a random vector
	for (int i = 0; i < cols; ++i)
		test_vec[i] = (int)(drand48() * valmax + 1);

	return UnitTest::RunAllTests();
}
