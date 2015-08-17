#pragma once

#include <vector>
#include <random>

namespace Algorithms
{

	// k - 0..k range
	std::vector<int> countingSort(const std::vector<int> &A, int k)
	{
		++k;
		std::vector<int> C(k, 0);
		std::vector<int> B(A.size());
		for (int j = 0; j < A.size(); ++j)
			C[A[j]] += 1;


		for (int i = 1; i < k; ++i)
			C[i] = C[i] + C[i - 1];

		for (int j = 0; j < A.size(); ++j)
		{
			C[A[j]] -= 1;
			B[C[A[j]]] = A[j];
		}
		return B;
	}

	void testCountingSort()
	{
		std::vector<int> data = { 2, 8, 7, 1, 3, 5, 6, 4 };

		std::cout << "---------------- Testing countingsort ----------------" << std::endl;

		std::cout << "Unsorted: ";
		std::for_each(data.begin(), data.end(), [](int value) { std::cout << value << " "; });
		std::cout << std::endl;

		auto result = Algorithms::countingSort(data, *std::max_element( data.begin(), data.end()) );

		std::cout << "Sorted: ";
		std::for_each(result.begin(), result.end(), [](int value) { std::cout << value << " "; });
		std::cout << std::endl;

		std::cout << "---------------- End of testing countingsort ----------------" << std::endl;
	}
}
