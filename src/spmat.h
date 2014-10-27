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
 *  * mRow[i] is the row index of mData[i]
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
	friend SpMat operator*(const SpMat&, const SpMat&);

public:
	SpMat() : nRows(0), nCols(0), nNonZero(0) { }
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
	size_t* mRow;
	size_t* mCInd;

	size_t nRows;
	size_t nCols;
	size_t nNonZero;	

	size_t nextc(size_t& col) const;
	void init_cptrs(size_t& curr, size_t& next, size_t& nextInd) const;
	void incr_cptrs(size_t& curr, size_t& next, size_t& nextInd) const;
};

inline size_t SpMat::nextc(size_t& col) const
{ 
	while (col < nCols && mCInd[col] == nNonZero) ++col; 
	return (col < nCols ? mCInd[col] : nNonZero); 
}

inline void SpMat::init_cptrs(size_t& curr, size_t& next, size_t& nextInd) const
{
	curr = 0; nextc(curr);
	next = curr + 1; 
	nextInd = nextc(next);
}

inline void SpMat::incr_cptrs(size_t& curr, size_t& next, size_t& nextInd) const
{
	curr = next;
	nextInd = nextc(++next);
}

};

#endif // SPMAT_H
