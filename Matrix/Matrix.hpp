#ifndef MATRIX_HPP
#define MATRIX_HPP


#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include <algorithm>
#include <functional>

#include "Vector.hpp"



namespace Mat
{

	///////////////////////////////////////////////
	//Struct XY for initialisation of MatrixIndices
	struct XY
	{
		unsigned int x;
		unsigned int y;
		XY() = default;
		XY(unsigned int _x, unsigned int _y);
	};


	///////////////////////////////////////////////
	//Struct MN for initialisation of MatrixIndices
	struct MN
	{
		unsigned int m;
		unsigned int n;
		MN() = default;
		MN(unsigned int _m, unsigned int _n);
	};


	//////////////////////////////////////////////////////////////////////////////////////////
	//Class MatrixIndices, which represents the indices of a matrix generalized from XY and MN
	class MatrixIndices
	{
	private:
		XY mXY;

	public:
		MatrixIndices() = default;
		MatrixIndices(XY const & xy);
		MatrixIndices(MN const & mn);

		void swap(MatrixIndices & other);

	public:
		void flip();

	public:
		XY xy() const;
		MN mn() const;

		unsigned int & x();
		unsigned int const & x() const;

		unsigned int & y();
		unsigned int const & y() const;

		unsigned int & m();
		unsigned int const & m() const;

		unsigned int & n();
		unsigned int const & n() const;

	public:
		bool operator==(MatrixIndices const & other);
		bool operator!=(MatrixIndices const & other);
		friend std::ostream& operator<<(std::ostream& oStream, MatrixIndices const & m);

	};


	typedef MatrixIndices MatrixSize;
	typedef MatrixIndices MatrixEntry;


	////////////////////////////////////////////////////////////////////////////
	//Struct InvalidIndicesException, which can be thrown if an index is invalid
	struct InvalidIndicesException
	{
		std::string message;
		MatrixIndices indicesPos;
		InvalidIndicesException(std::string const & _message, MatrixIndices const & _indicesPos);
	};


	///////////////////////////////////////////////////////////////////////////////////
	//Struct InvalidVecOfRowsException, which can be thrown if a vec of rows is invalid
	struct InvalidVecOfRowsException
	{
		std::string message;
		InvalidVecOfRowsException(std::string const & _message);
	};


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Struct IncompatibleSizesException, which can be thrown if an operation is not possible between two matrices due to their incompatible sizes
	struct IncompatibleMatrixSizesException
	{
	std::string message;
	MatrixSize size1;
	MatrixSize size2;
	IncompatibleMatrixSizesException(std::string const & _message, MatrixSize const & _size1, MatrixSize const & _size2);
	};



	///////////////////////
	//Class Template Matrix
	template <typename T> class Matrix
	{
	private:
		MatrixSize mSize;
		std::vector<std::vector<T>> mVecOfRows;

	public:
		//Standard constructor constructs 0x0 matrix
		Matrix()
			: mSize(XY(0u, 0u)), mVecOfRows()
		{
		}


		//Constructor that constructs matrix of size (sizeX, sizeY) with value
		explicit Matrix(MatrixSize const & size, T const & value = T())
			: mSize(size), mVecOfRows(size.y(), std::vector<T>(size.x(), value))
		{
		}


		//Constructor that constructs matrix from vec of rows
		explicit Matrix(std::vector<std::vector<T>> const & vecOfRows)
			: Matrix()
		{
			MatrixSize size;
			bool correctSize = Matrix<T>::checkIfVecOfRowsIsValidAndHandBackMatrixSize(vecOfRows, size);

			//If correctSize, construct matrix; else, throw exception
			if (correctSize)
			{
				this->mVecOfRows = vecOfRows;
				this->mSize = size;
			}
			else
			{
				throw InvalidVecOfRowsException("Matrix(std::vector<std::vector<T>> const & vecOfRows): Rows in vecOfRows have different sizes! Construction failed!");
			}
		}


		//Constructor that moves matrix from vec of rows
		explicit Matrix(std::vector<std::vector<T>> && vecOfRows)
			: Matrix()
		{
			MatrixSize size;
			bool correctSize = Matrix<T>::checkIfVecOfRowsIsValidAndHandBackMatrixSize(vecOfRows, size);

			//If correctSize, construct matrix; else, throw exception
			if (correctSize)
			{
				this->mVecOfRows = std::move(vecOfRows);
				this->mSize = size;
			}
			else
			{
				throw InvalidVecOfRowsException("Matrix(std::vector<std::vector<T>> && vecOfRows): Rows in vecOfRows have different sizes! Construction failed!");
			}
		}


		//Constructor that constructs matrix from vec, filling it with number rows or columns depending on asRows
		explicit Matrix(std::vector<T> const & vec, unsigned int number, bool asRows)
			: Matrix()
		{
			mSize = XY(vec.size(), number);
			mVecOfRows = std::vector<std::vector<T>>(number, vec);
			if (!asRows)
			{
				this->transpose();
			}
		}


		//Destructor and all other constructors stay default!


	public:
		//Returns the size of the matrix
		MatrixSize getSize() const
		{
			return mSize;
		}


		//Gives access to the components
		T& at(MatrixEntry const & pos)
		{
			if ((pos.x() >= mSize.x()) || (pos.y() >= mSize.y()))
			{
				throw InvalidIndicesException("T& at(MatrixEntry const & pos): pos is out of range!", pos);
			}
			return mVecOfRows.at(pos.y()).at(pos.x());
		}


		//Gives constant access to the components
		T const & at(MatrixEntry const & pos) const
		{
			if ((pos.x() >= mSize.x()) || (pos.y() >= mSize.y()))
			{
				throw InvalidIndicesException("T& at(MatrixEntry const & pos): pos is out of range!", pos);
			}
			return mVecOfRows.at(pos.y()).at(pos.x());
		}


		//Constructor that constructs matrix from matrix of other type
		template <typename S> explicit Matrix(Matrix<S> const & other)
			: Matrix(other.getSize())
		{
			for (unsigned int x = 0; x < mSize.x(); ++x)
			{
				for (unsigned int y = 0; y < mSize.y(); ++y)
				{
					mVecOfRows.at(y).at(x) = static_cast<T>(other.at(XY(x, y)));
				}
			}
		}


		//Calculates trace
		T trace() const
		{
			T sum = T(0);
			for (unsigned int i = 0; i < std::min(mSize.x(), mSize.y()); ++i)
			{
				sum += this->at(XY(i, i));
			}
			return sum;
		}


		//Swaps two rows
		void swapRows(unsigned int r1, unsigned int r2)
		{
			if (r1 >= mSize.y())
			{
				throw InvalidIndicesException("Matrix<T>::swapRows(unsigned int r1, unsigned int r2): r1 is no valid y index!", XY(0, r1));
			}
			if (r2 >= mSize.y())
			{
				throw InvalidIndicesException("Matrix<T>::swapRows(unsigned int r1, unsigned int r2): r2 is no valid y index!", XY(0, r1));
			}
			std::vector<T> row1 = mVecOfRows.at(r1);
			mVecOfRows.at(r1) = mVecOfRows.at(r2);
			mVecOfRows.at(r2) = row1;
		}


		//Multiplies one row by a factor
		void multiplyRowBy(unsigned int row, T factor)
		{
			if (row >= mSize.y())
			{
				throw InvalidIndicesException("Matrix<T>::multiplyRowBy(unsigned int row, T factor): row is no valid y index!", XY(0, row));
			}
			std::vector<T>& rowVec = mVecOfRows.at(row);
			for (auto & num : rowVec)
			{
				num *= factor;
			}
		}


		//Subtracts two rows and writes the result into the first row
		void subtractRows(unsigned int minuendRow, unsigned int subtrahendRow)
		{
			if (minuendRow >= mSize.y())
			{
				throw InvalidIndicesException("Matrix<T>::subtractRows(unsigned int minuendRow, unsigned int subtrahendRow): minuendRow is no valid y index!", XY(0, minuendRow));
			}
			if (subtrahendRow >= mSize.y())
			{
				throw InvalidIndicesException("Matrix<T>::subtractRows(unsigned int minuendRow, unsigned int subtrahendRow): subtrahendRow is no valid y index!", XY(0, subtrahendRow));
			}
			std::vector<T>& row1 = mVecOfRows.at(minuendRow);
			std::vector<T> const & row2 = mVecOfRows.at(subtrahendRow);
			for (unsigned int x = 0; x < mSize.x(); ++x)
			{
				row1.at(x) -= row2.at(x);
			}
		}


		//Returns the matrix in row echelon form
		Matrix<double> getRowEchelonForm(double& productOfGaussFactors) const
		{
			productOfGaussFactors = 1.0;
			Matrix<double> matrix(*this);
			for (unsigned int x = 0; x < mSize.x(); ++x)
			{
				//Sort zeros to the back
				for (unsigned int y = x; y < mSize.y(); ++y)
				{
					if (matrix.at(XY(x, y)) == 0.0)
					{
						for (unsigned int n = y + 1; n < mSize.y(); ++n)
						{
							if (matrix.at(XY(x, n)) != 0.0)
							{
								matrix.swapRows(y, n);
								productOfGaussFactors *= -1.0;
								break;
							}
						}
					}
				}

				//If first entry in column is not zero, multiply all non zero rows with the correct factor, so that they all have the same entry and then subtract first row from it
				if (matrix.at(XY(x, x)) != 0.0)
				{
					for (unsigned int y = x + 1; y < mSize.y(); ++y)
					{
						if (matrix.at(XY(x, y)) != 0.0)
						{
							double multiplicator = matrix.at(XY(x, x)) / matrix.at(XY(x, y));
							matrix.multiplyRowBy(y, multiplicator);
							matrix.subtractRows(y, x); //Minuend - Subtrahend
							productOfGaussFactors *= multiplicator;
						}
					}
				}
			}
			return matrix;
		}


		//Calculates determinant
		double det() const
		{
			//Non-quadratic matrices yield 0
			if (mSize.x() != mSize.y())
			{
				return 0.0;
			}

			//0x0 matrices yield 0
			if (mSize.x() == 0)
			{
				return 0.0;
			}

			//Calculate determinant from row echelon form
			double productOfGaussianFactors;
			Matrix<double> rowEchelonForm = this->getRowEchelonForm(productOfGaussianFactors);
			double det = 1.0;
			for (unsigned int x = 0; x < mSize.x(); ++x)
			{
				det *= rowEchelonForm.at(XY(x, x));
			}
			return det/productOfGaussianFactors;
		}


		//Finds all EntryPositions which have dist tolerance or less from val
		std::list<MatrixEntry> find(T const & val, T const & tolerance = T(0)) const
		{
			std::list<MatrixEntry> list;
			for (unsigned int x = 0; x < mSize.x(); ++x)
			{
				for (unsigned int y = 0; y < mSize.y(); ++y)
				{
					if (std::abs(this->at(XY(x, y)) - val) <= tolerance)
					{
						list.push_back(XY(x, y));
					}
				}
			}
			return list;
		}


		//Returns submatrix beginning at origin with size size (Non overlapping parts will be cut out! Example: 2x2 matrix with origin=(1,1) and size=(1,1) yields 1x1 matrix!)
		Matrix<T> getSubmatrix(MatrixEntry const & origin, MatrixSize const & size) const
		{
			//Determine full size without considering size
			unsigned int fullSizeX = static_cast<unsigned int>(std::max(0, static_cast<int>(mSize.x()) - static_cast<int>(origin.x())));
			unsigned int fullSizeY = static_cast<unsigned int>(std::max(0, static_cast<int>(mSize.y()) - static_cast<int>(origin.y())));

			//Take minimum of size and fullSize as real new size
			MatrixSize newSize = XY(std::min(fullSizeX, size.x()), std::min(fullSizeY, size.y()));

			//Create new matrix
			Matrix<T> newMatrix(newSize);
			for (unsigned int x = 0; x < newSize.x(); ++x)
			{
				for (unsigned int y = 0; y < newSize.y(); ++y)
				{
					newMatrix.at(XY(x, y)) = this->at(XY(origin.x() + x, origin.y() + y));
				}
			}
			return newMatrix;
		}


		//Returns transposed matrix without changing this
		Matrix<T> getTransposed() const
		{
			MatrixSize size(this->getSize());
			size.flip();
			Matrix<T> transposedMatrix(size);
			for (unsigned int x = 0; x < transposedMatrix.getSize().x(); ++x)
			{
				for (unsigned int y = 0; y < transposedMatrix.getSize().y(); ++y)
				{
					transposedMatrix.at(XY(x, y)) = this->at(XY(y, x));
				}
			}
			return transposedMatrix;
		}


		//Transposes this matrix
		void transpose()
		{
			*this = this->getTransposed();
		}


		//Resizes the matrix (If entries are created, they are filled with fillValue)
		void resize(MatrixSize size, T const & fillValue = T())
		{
			this->mSize = size;
			this->mVecOfRows.resize(size.y(), std::vector<T>(size.x(), fillValue));
			for (auto & row : this->mVecOfRows)
			{
				row.resize(size.x(), fillValue);
				row.shrink_to_fit();
			}
			this->mVecOfRows.shrink_to_fit();
		}


		//Fills every entry of the matrix with value
		void fillWith(T const & value)
		{
			for (auto & row : this->mVecOfRows)
			{
				for (auto & val : row)
				{
					val = value;
				}
			}
		}


		//Does some action for every entry in this
		void doForEveryEntry(std::function<void(T& entry, MatrixEntry const & entryPos)> action)
		{
			for (unsigned int x = 0; x < mSize.x(); ++x)
			{
				for (unsigned int y = 0; y < mSize.y(); ++y)
				{
					action(this->at(XY(x, y)), XY(x, y));
				}
			}
		}


	private:
		static bool checkIfVecOfRowsIsValidAndHandBackMatrixSize(std::vector<std::vector<T>> const & vecOfRows, MatrixSize& matrixSize)
		{
			//Construct size candidate
			MatrixSize size;
			if (vecOfRows.empty())
			{
				size = XY(0u, 0u);
			}
			else
			{
				//In this case, there is at least one row in vecOfRows
				size.y() = vecOfRows.size();
				size.x() = vecOfRows.front().size();
			}

			//Now check if all rows have the correct same size
			bool correctSize = true;
			for (auto const & row : vecOfRows)
			{
				if (row.size() != size.x())
				{
					correctSize = false;
					break;
				}
			}

			//If correctSize, return size; else, throw exception
			if (correctSize)
			{
				matrixSize = size;
				return true;
			}
			else
			{
				return false;
			}
		}


	}; //Class Template: Matrix



	template <typename T> std::ostream& operator<<(std::ostream& oStream, Matrix<T> const & mat)
	{
		for (unsigned int y = 0; y < mat.getSize().y(); ++y)
		{
			for (unsigned int x = 0; x < mat.getSize().x(); ++x)
			{
				if (x != 0)
				{
					oStream << " ";
				}
				oStream << mat.at(XY(x, y));
			}
			oStream << std::endl;
		}
		return oStream;
	}



	//Performs entrywise addition
	template <typename T> Matrix<T> operator+(Matrix<T> const & m1, Matrix<T> const & m2)
	{
		if (m1.getSize() != m2.getSize())
		{
			throw IncompatibleMatrixSizesException("operator+(Matrix<T> const & m1, Matrix<T> const & m2): m1 and m2 do not have the same size!", m1.getSize(), m2.getSize());
		}
		Matrix<T> newMatrix(m1);
		for (unsigned int x = 0; x < m1.getSize().x(); ++x)
		{
			for (unsigned int y = 0; y < m1.getSize().y(); ++y)
			{
				newMatrix.at(XY(x, y)) = m1.at(XY(x, y)) + m2.at(XY(x, y));
			}
		}
		return newMatrix;
	}


	//Performs entrywise substraction
	template <typename T> Matrix<T> operator-(Matrix<T> const & m1, Matrix<T> const & m2)
	{
		if (m1.getSize() != m2.getSize())
		{
			throw IncompatibleMatrixSizesException("operator-(Matrix<T> const & m1, Matrix<T> const & m2): m1 and m2 do not have the same size!", m1.getSize(), m2.getSize());
		}
		Matrix<T> newMatrix(m1);
		for (unsigned int x = 0; x < m1.getSize().x(); ++x)
		{
			for (unsigned int y = 0; y < m1.getSize().y(); ++y)
			{
				newMatrix.at(XY(x, y)) = m1.at(XY(x, y)) - m2.at(XY(x, y));
			}
		}
		return newMatrix;
	}


	//Performs matrix multiplication
	template <typename T> Matrix<T> operator*(Matrix<T> const & m1, Matrix<T> const & m2)
	{
		if (m1.getSize().n() != m2.getSize().m())
		{
			throw IncompatibleMatrixSizesException("operator*(Matrix<T> const & m1, Matrix<T> const & m2): m1 and m2 cannot be multiplied!", m1.getSize(), m2.getSize());
		}
		Matrix<T> matrix(MN(m1.getSize().m(), m2.getSize().n()));
		for (unsigned int m = 0; m < matrix.getSize().m(); ++m)
		{
			for (unsigned int n = 0; n < matrix.getSize().n(); ++n)
			{
				T sum = T(0);
				for (unsigned int i = 0; i < m1.getSize().n(); ++i)
				{
					sum += m1.at(MN(m, i)) * m2.at(MN(i, n));
				}
				matrix.at(MN(m, n)) = sum;
			}
		}
		return matrix;
	}


	//Performs multiplication with scalar from left
	template <typename T> Matrix<T> operator*(T const & s, Matrix<T> const & m)
	{
		Matrix<T> matrix(m);
		matrix.doForEveryEntry([&s](T& entry, MatrixEntry const & entryPos) {entry = s*entry; });
		return matrix;
	}


	//Performs multiplication with scalar from right
	template <typename T> Matrix<T> operator*(Matrix<T> const & m, T const & s)
	{
		return (s*m);
	}


	//Performs division with scalar
	template <typename T> Matrix<T> operator/(Matrix<T> const & m, T const & s)
	{
		Matrix<T> matrix(m);
		matrix.doForEveryEntry([&s](T& entry, MatrixEntry const & entryPos) {entry = entry/s; });
		return matrix;
	}


	//Returns matrix
	template <typename T> Matrix<T> operator+(Matrix<T> const & m)
	{
		return m;
	}


	//Returns negative matrix
	template <typename T> Matrix<T> operator-(Matrix<T> const & m)
	{
		Matrix<T> matrix(m);
		matrix.doForEveryEntry([](T& entry, MatrixEntry const & entryPos) {entry = -entry; });
		return matrix;
	}


	//Adds m2 to m1
	template <typename T> Matrix<T>& operator+=(Matrix<T> & m1, Matrix<T> const & m2)
	{
		m1 = m1 + m2;
		return m1;
	}


	//Subtracts m2 from m1
	template <typename T> Matrix<T>& operator-=(Matrix<T> & m1, Matrix<T> const & m2)
	{
		m1 = m1 - m2;
		return m1;
	}


	//Multiplies m by s
	template <typename T> Matrix<T>& operator*=(Matrix<T> & m, T const & s)
	{
		m = m * s;
		return m;
	}


	//Divide m by s
	template <typename T> Matrix<T>& operator/=(Matrix<T> & m, T const & s)
	{
		m = m / s;
		return m;
	}


	//Matrix vector product
	template <typename T> Vector<T> operator*(Matrix<T> const & mat, Vector<T> const & vec)
	{
		if (mat.getSize().x() != vec.getSize())
		{
			throw IncompatibleMatrixSizesException("operator*(Matrix<T> const & mat, Vector<T> const & vec): mat's and vec's sizes are not compatible for matrix vector multiplication!", mat.getSize(), XY(1, vec.getSize()));
		}
		Vector<T> res(mat.getSize().y());
		for (unsigned int m = 0; m < res.getSize(); ++m)
		{
			T sum = T(0);
			for (unsigned int n = 0; n < vec.getSize(); ++n)
			{
				sum += mat.at(MN(m, n)) * vec.at(n);
			}
			res.at(m) = sum;
		}
		return res;
	}



} //Namespace Mat

#endif //MATRIX_HPP

