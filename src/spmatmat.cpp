// spmatmat.cpp: (c) David R. Morrison, 2014

#include "spmat.h"

#include <cstring>

using namespace std;

namespace spla
{

// Double the size of the available memory for matrix entries if we've run out of space
size_t mat_realloc(size_t size, size_t capacity, double*& data, size_t*& rows)
{
	capacity *= 2; 
	double* dataRealloc = new double[capacity];
	size_t* rowsRealloc = new size_t[capacity];
	memcpy(dataRealloc, data, sizeof(double) * size);
	memcpy(rowsRealloc, rows, sizeof(size_t) * size);
	delete[] data; delete[] rows;
	data = dataRealloc; rows = rowsRealloc;

	return capacity;
}

// Once we've computed all the entries in a column, write it to the output matrix
size_t writeColumn(const SpVec& col, size_t ind, size_t& nzPos, size_t& nzCap, double*& data, 
		size_t*& rows, size_t* cind, vector<bool>& emptyCols)
{
	size_t nz = 0;
	for (size_t i = 0; i < col.size(); ++i)
	{
		// Only write non-zero entries
		if (col[i] > Tolerance)
		{
			// Record the starting position of the new column 
			if (emptyCols[ind]) cind[ind] = nzPos; 
			emptyCols[ind] = false;

			// If we've run out of space, realloc
			if (nzPos >= nzCap - 1)
				nzCap = mat_realloc(nzPos, nzCap, data, rows);

			// Fill in the data for this column entry
			data[nzPos] = col[i];
			rows[nzPos] = i;
			++nz; ++nzPos;
		}
	}
	return nz;
}

// Sparse matrix-matrix product
SpMat operator*(const SpMat& m1, const SpMat& m2)
{
	// Create an empty matrix, and fill it in column-by-column
	SpMat out(m1.nRows, m2.nCols, {});

	// We allocate enough space for the smaller of the two matrices initially
	size_t nzPos = 0;
	size_t nzCap = m1.nNonZero < m2.nNonZero ? m1.nNonZero : m2.nNonZero;
	out.mData = new double[nzCap];
	out.mRow = new size_t[nzCap];

	// Start by working column-wise through the second matrix
	size_t m2ccol, m2ncol, m2nind;
	m2.init_cptrs(m2ccol, m2ncol, m2nind);
	vector<bool> emptyCols(out.nCols, true);
	
	SpVec col(m1.nRows, 0);

	// Need to make sure all columns get written, so we'll manually terminate
	// the loop after the last column is processed
	for (size_t j = 0; ; ++j)
	{
		// If we've reached the start of the next column, write out the previous
		// one and reset
		if (j == m2nind)
		{
			out.nNonZero += writeColumn(col, m2ccol, nzPos, nzCap, 
					out.mData, out.mRow, out.mCInd, emptyCols);
			if (j == m2.nNonZero) break;

			m2.incr_cptrs(m2ccol, m2ncol, m2nind);
			col.clear(); col.resize(m1.nRows, 0);
		}

		// For each entry, we loop through all elements in the column of m1 corresponding to the
		// entry's row in m2; (we're just exploiting the commutativity of addition to add up the
		// entries in a different order).
		size_t m1ccol = m2.mRow[j]; 
		size_t m1cind = m1.mCInd[m1ccol];
		size_t m1ncol = m1ccol + 1; 
		size_t m1nind = m1.nextc(m1ncol);

		for (size_t i = m1cind; i < m1nind; ++i)
			col[m1.mRow[i]] += m1.mData[i] * m2.mData[j];
	}

	// We need to mark all of the empty columns in mCInd
	for (size_t i = 0; i < emptyCols.size(); ++i)
		if (emptyCols[i]) out.mCInd[i] = out.nNonZero;

	return out;
}

};
