// spmatvec.cpp: (c) David R. Morrison, 2014

#include "spops.h"

using namespace std;

namespace spla
{

// Standard mat-vec product
SpVec operator*(const SpMat& mat, const SpVec& vec)
{
	SpVec out(mat.nRows, 0);
	size_t currCol = 0; mat.findNextColStart(currCol);
	size_t nextCol = currCol + 1; mat.findNextColStart(nextCol);

	for (size_t i = 0; i < mat.nNonZero; ++i)
	{
		if (nextCol < mat.nCols && i == mat.mCInd[nextCol])
		{
			currCol = nextCol;
			mat.findNextColStart(++nextCol);
		}
		out[mat.mRInd[i]] += mat.mData[i] * vec[currCol];
	}

	return out;
}

};
