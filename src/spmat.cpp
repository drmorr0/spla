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
	size_t pos = 0;
	size_t col = 0;
	mCInd[col] = 0;

	// Data should be inserted in the correct order, since we're using a custom
	// sort function for the map, so now we just have to traverse the elements
	for (auto i = data.begin(); i != data.end(); ++i)
	{
		// Ignore zeros
		if (fabs(i->second) < Tolerance) continue;

		mRInd[pos] = i->first.first;
		mData[pos] = i->second;
		while (i->first.second != col)
			mCInd[++col] = pos;
		++pos;
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

	memcpy(mData, mat.mData, sizeof(size_t) * nNonZero);
	memcpy(mRInd, mat.mRInd, sizeof(size_t) * nNonZero);
	memcpy(mCInd, mat.mCInd, sizeof(size_t) * nCols);
}

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

SpMat::~SpMat()
{
	delete[] mData;
	delete[] mRInd;
	delete[] mCInd;
}

SpMatData SpMat::data() const
{
	size_t col = 0;
	SpMatData data;
	for (size_t i = 0; i < nNonZero; ++i)
	{
		while (col < nCols && mCInd[col] == i) ++col;
		data[{mRInd[i], col-1}] = mData[i];
	}
	return data;
}

};
