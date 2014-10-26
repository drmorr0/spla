/*
 * spla_test.cpp: (c) David R. Morrison, 2014
 *
 * Test functions for the sparse linear algebra library
 */

#include "types.h"
#include "spmat.h"

#include <unittest++/UnitTest++.h>

using namespace std;
using namespace spla;

const int rows = 100;
const int cols = 100;
const double valmax = 99;
const double sparsity = 0.1;

struct MatrixPair
{
	SpMatData data;
	SpMat smat;
	vector<vector<double>> dmat;
};

MatrixPair test_mat_1;
MatrixPair test_mat_2;
SpVec test_vec(cols, 0);
clock_t start;

MatrixPair genRandomMatrix(int nr, int nc, double sparsity)
{
	MatrixPair out;
	out.dmat.resize(nr, vector<double>(nc, 0));

	// Generate a random sparse matrix 
	for (int i = 0; i < nr; ++i)
	{
		for (int j = 0; j < nc; ++j)
		{
			if (drand48() < sparsity)
			{
				out.data[{i, j}] = (int)(drand48() * valmax + 1);
				out.dmat[i][j] = out.data[{i, j}];
			}
		}
	}
	out.smat = SpMat(nr, nc, out.data);
	return out;
}

TEST(MatrixConstruction)
{
	CHECK(test_mat_1.data == test_mat_1.smat.data());
	CHECK(test_mat_2.data == test_mat_2.smat.data());

	SpMatData test1;
	test1[{4,1}] = 5; test1[{2,2}] = 5; test1[{8,2}] = 5; test1[{2,3}] = 5; test1[{5,3}] = 5;
	test1[{6,4}] = 5; test1[{2,5}] = 5; test1[{4,5}] = 5; test1[{8,5}] = 5; test1[{0,6}] = 5;
	test1[{8,7}] = 5; test1[{7,9}] = 5;
	CHECK(test1 == SpMat(10, 10, test1).data());
}

TEST(MatrixCopy)
{
	SpMat mat_copy(test_mat_1.smat);
	CHECK(test_mat_1.data == mat_copy.data());
}

TEST(MatrixAssign)
{
	SpMat mat_assign = test_mat_1.smat;
	CHECK(test_mat_1.data == mat_assign.data());
}

TEST(MatVecMultiply)
{
	start = clock();
	SpVec dense_out(rows, 0);
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j)
			dense_out[i] += test_mat_1.dmat[i][j] * test_vec[j];
	printf("mat-vec time with dense matrix: %0.2fs\n", 
			(clock() - start) / (double)(CLOCKS_PER_SEC));

	start = clock();
	SpVec out1 = test_mat_1.smat * test_vec;
	printf("mat-vec time using operator*: %0.2fs\n", 
			(clock() - start) / (double)(CLOCKS_PER_SEC));

	CHECK(out1 == dense_out);
}

TEST(MatMatMultiply)
{
	start = clock();
	vector<vector<double>> dense_out(test_mat_1.dmat.size(), 
			vector<double>(test_mat_2.dmat[0].size(), 0));
	for (size_t i = 0; i < test_mat_1.dmat.size(); ++i)
		for (size_t j = 0; j < test_mat_2.dmat.size(); ++j)
			for (size_t k = 0; k < test_mat_2.dmat[0].size(); ++k)
				dense_out[i][k] += test_mat_1.dmat[i][j] * test_mat_2.dmat[j][k];
	printf("mat-mat time with dense matrices: %0.2fs\n", 
			(clock() - start) / (double)(CLOCKS_PER_SEC));


	start = clock();
	SpMat out1 = test_mat_1.smat * test_mat_2.smat;
	printf("mat-mat time with operator*: %0.2fs\n", 
			(clock() - start) / (double)(CLOCKS_PER_SEC));
	
	vector<vector<double>> sparse_out(out1.rows(), vector<double>(out1.cols(), 0));
	SpMatData o1data = out1.data();
	for (auto i = o1data.begin(); i != o1data.end(); ++i)
		sparse_out[i->first.first][i->first.second] = i->second;

	CHECK(sparse_out == dense_out);

}

int main(int argc, char* argv[])
{
	int seed = time(nullptr);
	printf("RANDOM SEED: %d\n", seed);
	srand48(seed);

	test_mat_1 = genRandomMatrix(rows, cols, sparsity);
	test_mat_2 = genRandomMatrix(cols, rows, sparsity);

	// Generate a random vector
	for (int i = 0; i < cols; ++i)
		test_vec[i] = (int)(drand48() * valmax + 1);

	return UnitTest::RunAllTests();
}
