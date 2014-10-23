// spmat.cpp: (c) David R. Morrison, 2014

#include "spmat.h"

#include <algorithm>
#include <cstring>

using namespace std;

namespace spla
{

SpMat::SpMat(const SpMatData& data) :
	nRows((--data.end())->first.first + 1),
	nCols((--data.end())->first.second + 1),
	nNonZero(data.size())
{
	mData = new double[nNonZero];
	mRInd = new size_t[nNonZero];
	mCInd = new size_t[nCols];
	int pos = 0;
	int col = -1;

	// Data should be inserted in the correct order, since we're using a custom
	// sort function for the map, so now we just have to traverse the elements
	for (auto i = data.begin(); i != data.end(); ++i)
	{
		// Ignore zeros
		if (fabs(i->second) < Tolerance) continue;

		mRInd[pos] = i->first.first;
		mData[pos] = i->second;
		if (i->first.second != col)
			mCInd[++col] = pos;
	}
}

SpMat::SpMat(const SpMat& mat) :
	nRows(mat.nRows),
	nCols(mat.nCols),
	nNonZero(mat.nNonZero)
{
	mData = new double[nNonZero];
	mRInd = new size_t[nNonZero];
	mCInd = new size_t[nCols];

	memcpy(mData, mat.mData, nNonZero);
	memcpy(mRInd, mat.mRInd, nNonZero);
	memcpy(mCInd, mat.mCInd, nCols);
}

SpMat::~SpMat()
{
	delete[] mData;
	delete[] mRInd;
	delete[] mCInd;
}

};
