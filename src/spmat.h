#ifndef SPMAT_H
#define SPMAT_H

/*
 * spmat.h: (c) David R. Morrison, 2014
 *
 * A (naive) sparse matrix implementation.  Most LP solvers that I've seen use a column-indexed
 * sparse matrix representation.  I'm not really sure why they choose this, but I'm going to go with
 * it since I figure they must know what they're doing.
 *
 * Since we want this to be lightning-fast, we're going to handle all of our own memory allocations.
 *
 */

#include "types.h"

namespace spla
{

class SpMat
{
public:
	SpMat(const SpMatData& data);
	SpMat(const SpMat&);
	~SpMat();

private:
	double* mData;
	size_t* mRInd;
	size_t* mCInd;

	size_t nRows;
	size_t nCols;
	size_t nNonZero;	
};

};

#endif // SPMAT_H
