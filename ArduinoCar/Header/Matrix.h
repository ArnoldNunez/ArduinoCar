#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

namespace ArduinoCar_Core
{
	class Matrix
	{
	public:

		/**
		 * Default Constructor
		 */
		Matrix();

		/**
		 * Constructor
		 * \param value The value the matrix will have.
		 */
		Matrix(std::vector<std::vector<double>> value);

		/**
		 * Copy Constructor (disabled)
		 */
		Matrix(const Matrix& mat);

		/**
		 * Destructor
		 */
		virtual ~Matrix();


		/**
		 * Makes matrix of a certain size and sets each element to zero
		 */
		void Zero(int dimX, int dimY);

		/**
		 *
		 */
		void Identity(int dim);

		/**
		 *
		 */
		void Show();

		/**
		 *
		 */
		Matrix Transpose();

		/**
		 *
		 */
		Matrix Take(const std::vector<int>& list1, const std::vector<int>& list2);

		/**
		 *
		 */
		Matrix Expand(int dimX, int dimY, const std::vector<int>& list1, const std::vector<int>& list2);

		/**
		 * Computes the upper triangular cholesky factorization of a positive definite matrix.
		 * \param zTol Specifies the value at which a number is considered to still be zero.
		 */
		Matrix Cholesky(double zTol = 0.00001);

		/**
		 * Computes inverse of a matrix given its Cholesky upper triangular
		 * decomposition of matrix.
		 */
		Matrix CholeskyInverse();

		/**
		 * Computes the inverse of the matrix.
		 */
		Matrix Inverse();

		/**
		 * Performs element-wise multiplication.
		 */
		Matrix Multiply(const Matrix& mat);


		/**
		 * Getter. Gets the number of rows in the matrix.
		 */
		int NumRows() { return mDimX; }

		/**
		 * Getter. Gets the number of cols in the matrix.
		 */
		int NumCols() { return mDimY; }


		/**
		 * Operator Overload ==
		 */
		bool operator==(const Matrix rhs) const;		

		/**
		 * Operator overload for (row, col)
		 */
		double operator()(unsigned row, unsigned col) const;

		/**
		 * Operator overlaod for (row, col, val) which allows
		 * to set matrix values using () syntax
		 */
		void operator()(unsigned row, unsigned col, double newVal);

		/**
		 * Operator overload +. Defines an element-wise matrix addition.
		 * Both matrices must be of equal dimensions
		 */
		Matrix operator+(const Matrix& rhs);

		/**
		 * Operator overload +. Defines an element-wise matrix addition.
		 * Both matrices must be of equal dimensions
		 */
		Matrix operator-(const Matrix& rhs);

	private:
		std::vector<std::vector<double> > mValue;
		int mDimX;
		int mDimY;
	};
}


#endif // !MATRIX_H
