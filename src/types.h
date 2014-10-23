#ifndef TYPES_H
#define TYPES_H

/* 
 * types.h: (c) David R. Morrison, 2014
 *
 * Miscellaneous types for the spase matrix library
 */

#include <utility>
#include <map>

namespace spla
{

const double Tolerance = 10e-7;

struct DataComp
{
	bool operator()(const std::pair<size_t,size_t>& ind1, const std::pair<size_t,size_t>& ind2)
	{
		return (ind1.second < ind2.second ||
			   (ind1.second == ind2.second && ind1.first < ind2.first));
	}
};

typedef std::map<std::pair<size_t,size_t>, double, DataComp> SpMatData;

};

#endif // TYPES_H
