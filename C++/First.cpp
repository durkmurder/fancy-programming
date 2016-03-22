#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

const double epsilon = 0.00001F;

vector< vector<double> > multiply(const vector< vector<double> > &left, const vector< vector<double> > &right)
{
	vector< vector<double> > product;
	product.resize(right.size());
    for(int i = 0; i < right.size(); ++i)
		product[i].resize(right[0].size());

    for (int row = 0; row < left.size(); row++)
	{
        for (int col = 0; col < right[0].size(); col++)
		{
            // Multiply the row of A by the column of B to get the row, column of product.
            for (int inner = 0; inner < left[0].size(); inner++)
			{
                product[row][col] += left[row][inner] * right[inner][col];
            }
           // std::cout << product[row][col] << "  ";
        }
       // std::cout << "\n";
    }
	return product;
}

void print(const vector< vector<double> > &matrix)
{
    for(int i = 0; i < matrix.size(); ++i)
	{
        for(int j = 0; j < matrix[0].size(); ++j)
		{
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

double innerProduct(const vector<double> &left, const vector<double> &right)
{
    double result = 0;
    for(int i = 0; i < left.size(); ++i)
		result += left[i] * right[i];
	return result;
}

vector<vector<double> > calculateY(const vector<vector<double> > &x0, const vector<vector<double> > &a, int k)
{
	vector<vector<double> > result = a;
    for(int i = 0; i < k - 1; ++i)
	{
		result = multiply(result, a);
	}
	return multiply(result, x0);
}
void readFromFile(vector< vector<double> > &matrix, const string &filename)
{
	ifstream stream(filename);
    for(int i = 0; i < matrix.size(); ++i)
	{
        for(int j = 0; j < matrix[0].size(); ++j)
		{
			stream >> matrix[i][j];
		}
	}
	stream.close();
}

vector<double> columnToRow(const vector<vector <double> > &column)
{
    vector<double> row;
    row.resize(column.size());
    for(int i = 0; i < row.size(); ++i)
    {
        row[i] = column[i][0];
    }
    return row;
}

void eigenValue2(const vector<vector<double> > &a1, const vector<vector<double> > &y0)
{
    double lambda1;
    int k = 1;
    vector<double> x, y, z;
	vector<double> temp;
	temp.resize(3);
    while(true)
	{
        x = columnToRow(calculateY(y0, a1, k));
        y = columnToRow(calculateY(y0, a1, k + 1));
		lambda1 = innerProduct(y, x) / innerProduct(x, x);
		for(int i = 0; i < 3; ++i)
		{
			temp[i] = y[i] - lambda1 * x[i];
		}
		if(sqrt(innerProduct(temp, temp)) < epsilon)
			break;
		else
			++k;
    }



    cout << lambda1 << endl;
    k = 1;
    double lambda2;
    while(true)
    {
        x = columnToRow(calculateY(y0, a1, k));
        y = columnToRow(calculateY(y0, a1, k + 1));
        z = columnToRow(calculateY(y0, a1, k - 1));
        lambda2 = (innerProduct(y, x) - lambda1 * innerProduct(x, x)) / (innerProduct(x, x) - lambda1 * innerProduct(x, z));

		for(int i = 0; i < 3; ++i)
		{
			temp[i] = y[i] - lambda1 * x[i];
		}
		if(sqrt(innerProduct(temp, temp)) < epsilon)
			break;
		else
			++k;
    }
    cout << lambda2 << endl;
}

void eigenValue1(const vector<vector<double> > &a1, const vector<vector<double> > &y0)
{
    double lambda1[3], lambda1k[3];
    int k = 1;
    vector<double> x, y, z;
    while(true)
    {
        x = columnToRow(calculateY(y0, a1, k));
        y = columnToRow(calculateY(y0, a1, k + 1));
		for(int i = 0; i < 3; ++i)
			lambda1[i] = y[i] / x[i];

		x = columnToRow(calculateY(y0, a1, k + 1));
        y = columnToRow(calculateY(y0, a1, k + 2));

		for(int i = 0; i < 3; ++i)
		{
			lambda1k[i] = y[i] / x[i];
		}
		if(true)
		{
			if(*std::max_element(lambda1, lambda1 + 3) - *std::min_element(lambda1, lambda1 + 3) < epsilon)
				break;
			
		}
		else
		{
			if(abs(*std::max_element(lambda1, lambda1 + 3) - *std::max_element(lambda1k, lambda1k + 3)) < epsilon)
				break;
		}
		++k;
    }
    cout << lambda1[0] << endl;
	x = columnToRow(calculateY(y0, a1, k));
	double norm = sqrt(innerProduct(x, x));
	cout << "Eiegen vector: ";
	for(int i = 0; i < x.size(); ++i)
	{
		cout << x[i] / norm << " ";
	}
	cout << endl;
    k = 1;
    double lambda2[3], lambda2k[3];
    while(true)
    {
        x = columnToRow(calculateY(y0, a1, k));
        y = columnToRow(calculateY(y0, a1, k + 1));
        z = columnToRow(calculateY(y0, a1, k - 1));
		for(int i = 0; i < 3; ++i)
			lambda2[i] = (y[i] - lambda1[0] * x[i]) / (x[i] - lambda1[i] * z[i]);

        x = columnToRow(calculateY(y0, a1, k + 1));
        y = columnToRow(calculateY(y0, a1, k + 2));
        z = columnToRow(calculateY(y0, a1, k));

		for(int i = 0; i < 3; ++i)
			lambda2k[i] = (y[i] - lambda1[0] * x[i]) / (x[i] - lambda1[i] * z[i]);

        if(false)
		{
			if(*std::max_element(lambda2, lambda2 + 3) - *std::min_element(lambda2, lambda2 + 3) < epsilon)
			{
				break;
			}
			
		}
		else
		{
			if(*std::max_element(lambda2k, lambda2k + 3) - *std::max_element(lambda2k, lambda2k + 3) < epsilon)
				break;
		}

        ++k;
    }
    cout << lambda2[0] << endl;
	x = columnToRow(calculateY(y0, a1, k));
	y = columnToRow(calculateY(y0, a1, k + 1));
	norm = sqrt(innerProduct(x, x));
	double norm2 = sqrt(innerProduct(y, y));
	cout << "Eiegen vector: ";
	for(int i = 0; i < x.size(); ++i)
	{
		cout << y[i] / norm2 - lambda1[0] * x[i] / norm << " ";
	}
	cout << endl;
}

int main()
{

	vector< vector<double> > a1;
	a1.resize(3);
    for(int i = 0; i < 3; ++i)
	{
		a1[i].resize(3);
	}
	readFromFile(a1, "info.txt");
    vector< vector<double> > y0;
    y0.resize(3);
    y0[0].push_back(0.2f);
    y0[1].push_back(0.3f);
    y0[2].push_back(0.4f);
    eigenValue1(a1, y0);
    eigenValue2(a1, y0);
	
//	print(a1);
//	print(a2);
//	print(calculateY(a2, a1, 5));

	return 0;
}
