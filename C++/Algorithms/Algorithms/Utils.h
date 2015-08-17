#pragma once

#include <algorithm>

template <typename Collection, typename Predicate>
Collection filterNot(Collection col, Predicate predicate) 
{
	auto returnIterator = std::remove_if(col.begin(), col.end(), predicate);
	col.erase(returnIterator, std::end(col));
	return col;
}

template <typename Collection, typename Predicate>
Collection filterNot(typename Collection::const_iterator start, typename Collection::const_iterator end, Predicate predicate)
{
	Collection result( std::distance(start, end) );
	std::copy_if(start, end, result.begin(), predicate);
	return result;
}