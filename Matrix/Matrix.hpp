#ifndef MATRIX_HPP
#define MATRIX_HPP


#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include <algorithm>
#include <functional>



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
	struct IncompatibleSizesException
	{
	std::string message;
	MatrixSize size1;
	MatrixSize size2;
	IncompatibleSizesException(std::string const & _message, MatrixSize const & _size1, MatrixSize const & _size2);
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


		//Calculates determinant
		T det() const
		{
			//Some nice algorithm
			return T(0);
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


	public:
		template <typename T> friend std::ostream& operator<<(std::ostream& oStream, Matrix<T> const & mat)
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


	}; //Class Template: Matrix



	//Performs entrywise addition
	template <typename T> Matrix<T> operator+(Matrix<T> const & m1, Matrix<T> const & m2)
	{
		if (m1.getSize() != m2.getSize())
		{
			throw IncompatibleSizesException("operator+(Matrix<T> const & m1, Matrix<T> const & m2): m1 and m2 do not have the same size!", m1.getSize(), m2.getSize());
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
			throw IncompatibleSizesException("operator-(Matrix<T> const & m1, Matrix<T> const & m2): m1 and m2 do not have the same size!", m1.getSize(), m2.getSize());
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
			throw IncompatibleSizesException("operator*(Matrix<T> const & m1, Matrix<T> const & m2): m1 and m2 cannot be multiplied!", m1.getSize(), m2.getSize());
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



} //Namespace Mat

#endif //MATRIX_HPP

