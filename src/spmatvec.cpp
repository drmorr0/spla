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

// Mat-mat product; this is not a particularly efficient algorithm
SpMat operator*(const SpMat& m1, const SpMat& m2)
{
	SpMat out(m1.nRows, m2.nCols, {});
	size_t outnzpos = 0;
	size_t outnz = m1.nNonZero < m2.nNonZero ? m1.nNonZero : m2.nNonZero;
	out.mData = new double[outnz];
	out.mRInd = new size_t[outnz];

	size_t m2ccol = 0; m2.findNextColStart(m2ccol);
	size_t m2ncol = m2ccol + 1; m2.findNextColStart(m2ncol);
	vector<bool> emptyCols(out.nCols, true);
	
	while (m2ccol < out.nCols)
	{
		size_t m2nind = m2ncol < m2.nCols ? m2.mCInd[m2ncol] : m2.nNonZero;
		SpVec outCol(m1.nRows, 0);
		for (size_t j = m2.mCInd[m2ccol]; j < m2nind; ++j)
		{
			size_t m1ccol = m2.mRInd[j]; 
			size_t m1ncol = m1ccol + 1; m1.findNextColStart(m1ncol);
			size_t m1nind = m1ncol < m1.nCols ? m1.mCInd[m1ncol] : m1.nNonZero;

			for (size_t i = m1.mCInd[m1ccol]; i < m1nind; ++i)
				outCol[m1.mRInd[i]] += m1.mData[i] * m2.mData[j];
		}

		size_t colStartInd = 0;
		for (size_t i = 0; i < outCol.size(); ++i)
		{
			if (outCol[i] > Tolerance)
			{
				if (emptyCols[m2ccol]) colStartInd = outnzpos;
				emptyCols[m2ccol] = false;

				if (outnzpos >= outnz - 1)
				{
					outnz *= 2; 
					double* dataRealloc = new double[outnz];
					size_t* rindRealloc = new size_t[outnz];
					memcpy(dataRealloc, out.mData, sizeof(double) * outnzpos);
					memcpy(rindRealloc, out.mRInd, sizeof(size_t) * outnzpos);
					delete[] out.mData; delete[] out.mRInd;
					out.mData = dataRealloc; out.mRInd = rindRealloc;
				}

				out.mData[outnzpos] = outCol[i];
				out.mRInd[outnzpos] = i;
				++(out.nNonZero);
				++outnzpos;
			}
		}
		if (!emptyCols[m2ccol]) out.mCInd[m2ccol] = colStartInd;
		m2ccol = m2ncol; m2ncol = m2ccol + 1; m2.findNextColStart(m2ncol);
	}

	for (size_t i = 0; i < emptyCols.size(); ++i)
		if (emptyCols[i]) out.mCInd[i] = out.nNonZero + 1;

	return out;
}

};
