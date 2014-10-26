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
	mData(nullptr),
	mRInd(nullptr),
	mCInd(nullptr),
	nRows(rows),		
	nCols(cols),
	nNonZero(data.size())
{
	if (nNonZero > 0)
	{
		mData = new double[nNonZero];
		mRInd = new size_t[nNonZero];
	}
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

// operator=: We use a semi-copy-and-swap technique here.  It's a bit of a hack, but I don't 
// really care right now
SpMat& SpMat::operator=(SpMat mat)
{
	nRows = mat.nRows;
	nCols = mat.nCols;
	nNonZero = mat.nNonZero;
	mData = mat.mData;
	mRInd = mat.mRInd;
	mCInd = mat.mCInd;

	// Make sure that our data doesn't get deleted when the other guy goes out of scope
	mat.mData = nullptr;
	mat.mRInd = nullptr;
	mat.mCInd = nullptr;
	return *this;
}

// Destructor: clean up our mess
SpMat::~SpMat()
{
	if (mData) delete[] mData;
	if (mRInd) delete[] mRInd;
	if (mCInd) delete[] mCInd;
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
