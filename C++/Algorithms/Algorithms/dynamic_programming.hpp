#pragma once
#include <vector>
#include <iostream>
#include <functional>
#include <tuple>
#include <stack>
#include <algorithm>

typedef std::vector<std::vector<int>> Matrix;

namespace Algorithms
{


	void fastestWay()
	{
		Matrix a{ { 7, 9, 3, 4, 8, 4 },{ 8, 5, 6, 4, 5, 7 } };
		Matrix t{ { 2, 3, 1, 3, 4 },{ 2, 1, 2, 2, 1 } };
		int e[2] = { 2, 4 };
		int x[2] = { 3, 2 };

		const int n = 6;

		auto fastest_way = [&]()
		{
			std::vector<int> f1(n, 0);
			std::vector<int> f2(n, 0);
			Matrix l{ { 0, 0, 0, 0, 0, 0 },{ 0, 0, 0, 0, 0, 0 } };
			f1[0] = e[0] + a[0][0];
			f2[0] = e[1] + a[1][0];
			for (int i = 1; i < n; ++i)
			{
				if (f1[i - 1] + a[0][i] <= f2[i - 1] + t[1][i - 1] + a[0][i])
				{
					f1[i] = f1[i - 1] + a[0][i];
					l[0][i] = 1;
				}
				else
				{
					f1[i] = f2[i - 1] + t[1][i - 1] + a[0][i];
					l[0][i] = 2;
				}

				if (f2[i - 1] + a[1][i] <= f1[i - 1] + t[0][i - 1] + a[1][i])
				{
					f2[i] = f2[i - 1] + a[1][i];
					l[1][i] = 2;
				}
				else
				{
					f2[i] = f1[i - 1] + t[0][i - 1] + a[1][i];
					l[1][i] = 1;
				}
			}

			if (f1[n - 1] + x[0] <= f2[n - 1] + x[1])
				return std::make_tuple(f1[n - 1] + x[0], 1, l);
			else
				return std::make_tuple(f2[n - 1] + x[1], 2, l);
		};

		int lstar, fstar;
		Matrix l;
		std::tie(fstar, lstar, l) = fastest_way();

		auto print_stations = [l, lstar, n]
		{
			int i = lstar;
			std::cout << "Container: " << i << " place: " << n << std::endl;
			for (int j = n - 1; j >= 1; --j)
			{
				i = l[i][j] - 1;
				std::cout << "Container: " << i + 1 << " place: " << j << std::endl;
			}
		};

		auto print_reverse = [l, n, lstar]
		{
			std::function< void(int, int) > helper = [&](int i, int j)
			{
				if (j > 0)
					helper(l[i][j] - 1, j - 1);

				std::cout << "Container: " << i + 1 << " place: " << j + 1 << std::endl;
			};

			helper(lstar, n - 2);
			std::cout << "Container: " << lstar << " place: " << n << std::endl;
		};

		std::cout << fstar << " " << lstar << std::endl;
		//print_stations();
		print_reverse();
	}

	void lis()
	{
		std::vector<int> A = { 4, 5, 7, 2, 1, 5, 18, 3 };
		std::vector<int> d(A.size(), 0);
		std::vector<int> prev(A.size(), 0);
		d[0] = 1;

		// d[j] - max length of subseq that ends in A[j]
		for (int i = 1; i < d.size(); ++i)
		{
			auto filtered = filterNot< std::vector<int>, std::function<bool(int)> >(d.cbegin(), d.cbegin() + i, [&](int j) { return A[j] < A[i]; });
			int max = 0;
			auto it = std::max_element(filtered.begin(), filtered.end());
			if (it != filtered.end())
				max = *it;
			d[i] = 1 + max;
		}

		std::cout << "Length of longest subsequence is: " << *std::max_element(d.begin(), d.end()) << std::endl;
	}

	void editDistance()
	{
		std::string A = "rosettacode";
		std::string B = "raisethysword";
		int p = A.length() + 1;
		int q = B.length() + 1;
		Matrix D; // D[i,j] - optimal edit distance of prefixes which ends in i and j positions
		D.resize(p);
		for (int i = 0; i < p; ++i)
			D[i].resize(q);

		for (int i = 0; i < p; ++i)
			D[i][0] = i;
		for (int j = 0; j < q; ++j)
			D[0][j] = j;

		for (int i = 1; i < p; ++i)
			for (int j = 1; j < q; ++j)
				D[i][j] = std::min(std::min(D[i - 1][j] + 1, D[i][j - 1] + 1), D[i - 1][j - 1] + (A[i - 1] == B[j - 1] ? 0 : 1));

		std::cout << "Edit distance between " << A << " and " << B << " is: " << D[p - 1][q - 1] << std::endl;
	}

	void pocket()
	{
		int W = 10;
		std::vector<int> C { 8, 2 }; // cost of objects
		std::vector<int> Weights { 5, 1 }; // weight of objects
		std::vector<int> A(W); // A[w] - optimal cost of objects that we can fit in pocket of size w 

		A[0] = 0;
		for (int w = 1; w < W; ++w)
		{
			int max = -1;
			for (int i = 0; i < Weights.size(); ++i)
				if (Weights[i] <= w && (A[w - Weights[i]] + C[i]) > max)
					max = A[w - Weights[i]] + C[i];
			A[w] = max;
		}

		std::cout << A[W - 1] << std::endl;
	}

	void uniquePocket()
	{
		int W = 10;
		std::vector<int> C{ 3, 2, 10, 1 }; // cost of objects
		std::vector<int> Weights{ 5, 7, 4, 2 }; // weight of objects
		Matrix A(Weights.size()); // A - optimal cost of first i objects that we can fit in pocket of size w 
		A[0].resize(W);

		for (int w = 0; w < W; ++w)
			A[0][w] = w >= Weights[0] ? C[0] : 0; // fill first row with first object


		for (int i = 1; i < Weights.size(); ++i)
		{
			A[i].resize(W);
			for (int w = 0; w < W; ++w)
				A[i][w] = w >= Weights[i] ? std::max(A[i - 1][w - Weights[i]] + C[i], A[i - 1][w]) : A[i - 1][w];
		}

		for (auto row : A)
		{
			for (auto elem : row)
				std::cout << elem << " ";
			std::cout << std::endl;
		}

		std::cout << A[A.size() - 1][W - 1] << std::endl;
	}

}