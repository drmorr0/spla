// spmat.cpp: (c) David R. Morrison, 2014

#include "spmat.h"

#include <algorithm>
#include <cstring>

using namespace std;

namespace spla
{

// Constructor: Take in a pair-indexed map of the non-zero elements, and convert them
// into the appropriate format
SpMat::SpMat(int rows, int cols, const SpMatData& data) :
	nRows(rows),		// The last element tells us how many rows and cols
	nCols(cols),
	nNonZero(data.size())
{
	mData = new double[nNonZero];
	mRInd = new size_t[nNonZero];
	mCInd = new size_t[nCols];
	for (size_t i = 0; i < nCols; ++i) mCInd[i] = nNonZero + 1;

	size_t pos = 0;

	// Data should be inserted in the correct order, since we're using a custom
	// sort function for the map, so now we just have to traverse the elements
	for (auto i = data.begin(); i != data.end(); ++i)
	{
		// Ignore zeros
		if (fabs(i->second) < Tolerance) continue;

		mRInd[pos] = i->first.first;
		mData[pos] = i->second;

		// Empty columns all "start" at the same index as the next non-empty column
		if (mCInd[i->first.second] == nNonZero + 1)
			mCInd[i->first.second] = pos;
		++pos;
	}
}

// Copy constructor: pretty straightforward, just deep-copy the data
SpMat::SpMat(const SpMat& mat) :
	nRows(mat.nRows),
	nCols(mat.nCols),
	nNonZero(mat.nNonZero)
{
	mData = new double[nNonZero];
	mRInd = new size_t[nNonZero];
	mCInd = new size_t[nCols];

	memcpy(mData, mat.mData, sizeof(size_t) * nNonZero);
	memcpy(mRInd, mat.mRInd, sizeof(size_t) * nNonZero);
	memcpy(mCInd, mat.mCInd, sizeof(size_t) * nCols);
}

// operator=: This is a partial copy-and-swap; since we're just throwing away the temporary guy,
// there's no reason to swap my data with his.  The annoying thing is that if new members get added
// to SpMat, we have to remember to add them here.  Maybe worth pulling out into a proper swap
// function sometime?
SpMat& SpMat::operator=(SpMat mat)
{
	nRows = mat.nRows;
	nCols = mat.nCols;
	nNonZero = mat.nNonZero;
	mData = mat.mData;
	mRInd = mat.mRInd;
	mCInd = mat.mCInd;
	return *this;
}

// Destructor: clean up our mess
SpMat::~SpMat()
{
	delete[] mData;
	delete[] mRInd;
	delete[] mCInd;
}

// Convert the internal data into a pair-indexed map of elements
SpMatData SpMat::data() const
{
	SpMatData data;
	size_t currCol = 0; findNextColStart(currCol);
	size_t nextCol = currCol + 1; findNextColStart(nextCol);

	for (size_t i = 0; i < nNonZero; ++i)
	{
		if (nextCol < nCols && i == mCInd[nextCol])
		{
			currCol = nextCol;
			findNextColStart(++nextCol);
		}
		data[{mRInd[i], currCol}] = mData[i];
	}

	return data;
}

};
