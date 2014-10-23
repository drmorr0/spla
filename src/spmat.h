#ifndef SPMAT_H
#define SPMAT_H

/*
 * spmat.h: (c) David R. Morrison, 2014
 *
 * A (naive) sparse matrix implementation.  Since we want this to be lightning-fast, we're going to
 * handle all of our own memory allocations.
 *
 * Data is stored in the so-called "Compressed sparse column" format, described below:
 *
 *  mData is 
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

	SpMatData data() const;

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
