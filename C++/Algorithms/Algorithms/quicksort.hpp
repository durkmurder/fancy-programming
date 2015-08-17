#pragma once

#include <vector>
#include <random>

namespace Algorithms
{
	template < class T >
	int partition(std::vector<T> &A, int p, int r)
	{
		T x = A[p]; // pivot element
		int i = p;
		for ( int j = p + 1; j < r; ++j )
		{ 
			if (A[j] <= x)
				std::swap(A[++i], A[j]);
		}
		std::swap(A[i], A[p]);

		return i;
	}

	template <class T>
	void quickSort(std::vector<T> &A, int p, int r)
	{
		if ( p < r )
		{
			static std::random_device generator;
			std::uniform_int_distribution<int> distribution(p, r - 1);
			int i = distribution(generator);
			std::swap( A[p], A[i] );

			int q = partition(A, p, r);
			quickSort(A, p, q);
			quickSort(A, q + 1, r);
		}
	}

	void testQuickSort()
	{
		std::vector<int> data = { 2, 8, 7, 1, 3, 5, 6, 4 };

		std::cout << "---------------- Testing quicksort ----------------" << std::endl;

		std::cout << "Unsorted: ";
		std::for_each(data.begin(), data.end(), [](int value) { std::cout << value << " "; });
		std::cout << std::endl;

		Algorithms::quickSort(data, 0, data.size());

		std::cout << "Sorted: ";
		std::for_each(data.begin(), data.end(), [](int value) { std::cout << value << " "; });
		std::cout << std::endl;

		std::cout << "---------------- End of testing quicksort ----------------" << std::endl;
	}
}

