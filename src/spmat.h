#ifndef SPMAT_H
#define SPMAT_H

/*
 * spmat.h: (c) David R. Morrison, 2014
 *
 * A (naive) sparse matrix implementation.  Since we want this to be lightning-fast, we're going to
 * handle all of our own memory allocations.
 *
 * Data is stored in the so-called "Compressed sparse column" format, described below:
 *  * mData is the \ith non-zero entry of the matrix, when ordered by columns first, and then rows
 *  * mRInd[i] is the row index of mData[i]
 *  * mCInd[i] is the position in mData of the beginning of the \ith column of the matrix; note
 *    that if the \ith column is empty, multiple columns will begin at position mCInd[i], and you
 *    need to scan to find the last column beginning in said position
 * See Wikipedia for more details
 *
 */

#include "types.h"

namespace spla
{

class SpMat
{
	friend SpVec operator*(const SpMat&, const SpVec&);

public:
	SpMat(int rows, int cols, const SpMatData& data);
	SpMat(const SpMat& mat);
	SpMat& operator=(SpMat mat);
	~SpMat();

	SpMatData data() const;
	size_t rows() const { return nRows; }
	size_t cols() const { return nCols; }
	size_t nz() const { return nNonZero; }

private:
	double* mData;
	size_t* mRInd;
	size_t* mCInd;

	size_t nRows;
	size_t nCols;
	size_t nNonZero;	

	void findNextColStart(size_t& curr) const
		{ while (curr < nCols && mCInd[curr] == nNonZero + 1) ++curr; }
};

};

#endif // SPMAT_H
