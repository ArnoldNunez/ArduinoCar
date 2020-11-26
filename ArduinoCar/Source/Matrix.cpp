#include "..\Header\Matrix.h"

#include <stdexcept>
#include <string>
#include <iostream>

using namespace std;

ArduinoCar_Core::Matrix::Matrix()
{
	mDimX = 0;
	mDimY = 0;
}

ArduinoCar_Core::Matrix::Matrix(vector<vector<double>> value)
{
	mDimX = value.size();
	mDimY = value[0].size();
	value = value;
}

ArduinoCar_Core::Matrix::Matrix(const Matrix& mat)
{
	mDimX = mat.mDimX;
	mDimY = mat.mDimY;
	mValue = mat.mValue;
}

ArduinoCar_Core::Matrix::~Matrix()
{
}

void ArduinoCar_Core::Matrix::Zero(int dimX, int dimY)
{
	if (dimX < 1 || dimY < 1)
	{
		throw invalid_argument("Invalid size of matrix");
	}
	else
	{
		mDimX = dimX;
		mDimY = dimY;
		
		vector<vector<double>> temp;
		for (int i = 0; i < dimX; i++)
		{
			vector<double> row;
			for (int j = 0; j < dimY; j++)
			{
				row.push_back(0);
			}

			temp.push_back(row);
		}

		mValue.swap(temp);
	}
}

void ArduinoCar_Core::Matrix::Identity(int dim)
{
	if (dim < 1)
	{
		throw invalid_argument("Invalid size of matrix");
	}
	else
	{
		mDimX = dim;
		mDimY = dim;

		vector<vector<double>> temp;
		for (int i = 0; i < dim; i++)
		{
			vector<double> row;
			for (int j = 0; j < dim; j++)
			{
				if (i == j) 
				{
					row.push_back(1);
				}
				else
				{
					row.push_back(0);
				}
			}

			temp.push_back(row);
		}

		mValue.swap(temp);
	}
}

void ArduinoCar_Core::Matrix::Show()
{
	string text = "";
	for (int i = 0; i < mDimX; i++)
	{
		for (int j = 0; j < mDimY; j++)
		{
			text.append(to_string(mValue[i][j]));
			text.append("  ");
		}

		text.append("\n");
	}

	cout << text;
}

ArduinoCar_Core::Matrix ArduinoCar_Core::Matrix::Transpose()
{
	Matrix res;
	res.Zero(mDimY, mDimX);

	for (int i = 0; i < mDimX; i++)
	{
		for (int j = 0; j < mDimY; j++)
		{
			res.mValue[j][i] = mValue[i][j];
		}
	}

	return res;
}

ArduinoCar_Core::Matrix ArduinoCar_Core::Matrix::Take(const std::vector<int>& list1, const std::vector<int>& list2)
{
	vector<int> listTwo;
	listTwo = list2.size() == 0 ? list1 : list2;

	if (list1.size() > mDimX || listTwo.size() > mDimY)
	{
		throw invalid_argument("List invalid in take()");
	}

	Matrix res;
	res.Zero(list1.size(), listTwo.size());

	for (int i = 0; i < list1.size(); i++)
	{
		for (int j = 0; j < listTwo.size(); j++)
		{
			res.mValue[i][j] = mValue[list1[i]][listTwo[j]];
		}
	}

	return res;
}

ArduinoCar_Core::Matrix ArduinoCar_Core::Matrix::Expand(int dimX, int dimY, const std::vector<int>& list1, const std::vector<int>& list2)
{
	vector<int> listTwo;
	listTwo = list2.size() == 0 ? list1 : list2;

	if (list1.size() > mDimX || listTwo.size() > mDimY)
	{
		throw invalid_argument("List invalid in take()");
	}

	Matrix res;
	res.Zero(dimX, dimY);

	for (int i = 0; i < list1.size(); i++)
	{
		for (int j = 0; j < list2.size(); j++)
		{
			res.mValue[list1[i]][listTwo[j]] = mValue[i][j];
		}
	}

	return res;
}

ArduinoCar_Core::Matrix ArduinoCar_Core::Matrix::Cholesky(double zTol)
{
	/**
	 * Computes upper triangular cholesky factorization of a positive definite matrix.
	 * http://adorio-research.org/wordpress/?p=4560
	 */

	Matrix res;
	res.Zero(mDimX, mDimY);

	for (int i = 0; i < mDimX; i++)
	{
		double s = 0.0;
		for (int k = 0; k < i; k++)
		{
			s += pow(res.mValue[k][i], 2.0);
		}

		double d = mValue[i][i] - s;
		if (abs(d) < zTol) 
		{
			res.mValue[i][i] = 0.0;
		}
		else
		{
			if (d < 0.0) { throw domain_error("Matrix not positive-definite"); }
			res.mValue[i][i] = sqrt(d);
		}

		
		for (int j = i + 1; j < mDimX; j++)
		{
			s = 0.0;
			for (int k = 0; k < i; k++)
			{
				s += res.mValue[k][i] * res.mValue[k][j];
			}

			if (abs(s) < zTol) { s = 0.0; }

			try
			{
				res.mValue[i][j] = (mValue[i][j] - s) / res.mValue[i][i];
			}
			catch (const std::exception&)
			{
				throw domain_error("Zero diagonal");
			}
		}
	}

	return res;
}

ArduinoCar_Core::Matrix ArduinoCar_Core::Matrix::CholeskyInverse()
{
	Matrix res;
	res.Zero(mDimX, mDimY);

	for (int j = mDimX - 1; j >= 0; j--)
	{
		double tjj = mValue[j][j];

		double s = 0.0;
		for (int k = j + 1; k < mDimX; k++)
		{
			s += mValue[j][k] * res.mValue[j][k];
		}

		res.mValue[j][j] = 1.0 / pow(tjj, 2.0) - s / tjj;

		// TODO: Fix this, should be going backwards
		for (int i = 0; i >= 0; i--)
		{
			double a = 0.0;
			for (int k = i + 1; k < mDimX; k++)
			{
				a += mValue[i][k] * res.mValue[k][j];
			}

			res.mValue[i][j] = -a;
			res.mValue[j][i] = res.mValue[i][j];
		}
	}

	return res;
}

ArduinoCar_Core::Matrix ArduinoCar_Core::Matrix::Inverse()
{
	Matrix aux = Cholesky();
	Matrix res = aux.CholeskyInverse();
	return res;
}

ArduinoCar_Core::Matrix ArduinoCar_Core::Matrix::Multiply(const Matrix& mat)
{
	if (mDimY != mat.mDimX)
	{
		throw invalid_argument("Invalid size of matrix");
	}
	else
	{
		Matrix res;
		res.Zero(mDimX, mat.mDimY);

		for (int i = 0; i < mDimX; i++)
		{
			for (int j = 0; j < mat.mDimY; j++)
			{
				for (int k = 0; k < mDimY; k++)
				{
					res.mValue[i][j] = mValue[i][k] * mat.mValue[k][j];
				}
			}
		}

		return res;
	}
}

bool ArduinoCar_Core::Matrix::operator==(const Matrix rhs) const
{
	double epsilon = 0.01;

	if (mDimX != rhs.mDimX || mDimY != rhs.mDimY) { return false; }

	for (int i = 0; i < mDimX; i++)
	{
		for (int j = 0; j < mDimY; j++)
		{
			if (abs(mValue[i][j] - rhs.mValue[i][j]) > epsilon)
			{
				return false;
			}
		}
	}

	return true;
}

double ArduinoCar_Core::Matrix::operator()(unsigned row, unsigned col) const
{
	return mValue[row][col];
}

void ArduinoCar_Core::Matrix::operator()(unsigned row, unsigned col, double newVal)
{
	mValue[row][col] = newVal;
}

ArduinoCar_Core::Matrix ArduinoCar_Core::Matrix::operator+(const Matrix& rhs)
{
	if (mDimX != rhs.mDimX || mDimY != rhs.mDimY)
	{
		throw invalid_argument("Invalid size of matrix");
	}
	else
	{
		Matrix res;
		res.Zero(mDimX, mDimY);

		for (int i = 0; i < mDimX; i++)
		{
			for (int j = 0; j < mDimY; j++)
			{
				res.mValue[i][j] = mValue[i][j] + rhs.mValue[i][j];
			}
		}

		return res;
	}
}

ArduinoCar_Core::Matrix ArduinoCar_Core::Matrix::operator-(const Matrix& rhs)
{
	if (mDimX != rhs.mDimX || mDimY != rhs.mDimY)
	{
		throw invalid_argument("Invalid size of matrix");
	}
	else
	{
		Matrix res;
		res.Zero(mDimX, mDimY);

		for (int i = 0; i < mDimX; i++)
		{
			for (int j = 0; j < mDimY; j++)
			{
				res.mValue[i][j] = mValue[i][j] - rhs.mValue[i][j];
			}
		}

		return res;
	}
}
