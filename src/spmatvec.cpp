// spmatvec.cpp: (c) David R. Morrison, 2014

#include "spmat.h"

#include <cstring>

using namespace std;

namespace spla
{

// Standard mat-vec product
SpVec operator*(const SpMat& mat, const SpVec& vec)
{
	SpVec out(mat.nRows, 0);

	size_t currCol, nextCol, nextInd;
	mat.init_cptrs(currCol, nextCol, nextInd);

	for (size_t i = 0; i < mat.nNonZero; ++i)
	{
		if (i == nextInd)
			mat.incr_cptrs(currCol, nextCol, nextInd);
		out[mat.mRow[i]] += mat.mData[i] * vec[currCol];
	}

	return out;
}

};
