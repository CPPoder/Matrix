#ifndef MATRIX_HPP
#define MATRIX_HPP


#include <iostream>
#include <vector>
#include <list>



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
		void flip() const;

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
	};


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


	typedef MatrixIndices MatrixSize;
	typedef MatrixIndices MatrixEntry;


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
			mVecOfRows = std::vector<T>(number, vec);
			if (!asRows)
			{
				this->transpose();
			}
		}


		//Destructor and all other constructors stay default!


	public:
		//Returns the size of the matrix
		MatrixSize getSize() const;


		//Gives access to the components
		T& at(MatrixEntry const & pos);


		//Gives constant access to the components
		T const & at(MatrixEntry const & pos) const;


		//Calculates trace
		T trace() const;


		//Calculates determinant
		T det() const;


		//Finds all EntryPositions which are equal to val
		std::list<MatrixEntry> find(T const & val) const;


		//Returns submatrix beginning at origin with size size
		Matrix<T> getSubmatrix(MatrixEntry const & origin, MatrixSize const & size) const;


		//Transposes this matrix
		void transpose();


		//Returns transposed matrix without changing this
		Matrix<T> getTransposed() const;


		//Resizes the matrix (If entries are created, they are filled with fillValue)
		void resize(MatrixSize size, T const & fillValue = T());


		//Fills every entry of the matrix with value
		void fillWith(T const & value);


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
				if (row.size() != size.x)
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



	//Performs entrywise addition
	template <typename T> Matrix<T> operator+(Matrix<T> const & m1, Matrix<T> const & m2);


	//Performs entrywise substraction
	template <typename T> Matrix<T> operator-(Matrix<T> const & m1, Matrix<T> const & m2);


	//Performs matrix multiplication
	template <typename T> Matrix<T> operator*(Matrix<T> const & m1, Matrix<T> const & m2);


	//Performs multiplication with scalar from left
	template <typename T> Matrix<T> operator*(T const & s, Matrix<T> const & m);


	//Performs multiplication with scalar from right
	template <typename T> Matrix<T> operator*(Matrix<T> const & m, T const & s);


	//Returns matrix
	template <typename T> Matrix<T> operator+(Matrix<T> const & m);


	//Returns negative matrix
	template <typename T> Matrix<T> operator-(Matrix<T> const & m);


	//Adds m2 to m1
	template <typename T> Matrix<T>& operator+=(Matrix const & m1, Matrix const & m2);


	//Subtracts m2 from m1
	template <typename T> Matrix<T>& operator-=(Matrix const & m1, Matrix const & m2);


	//Multiplies m by s
	template <typename T> Matrix<T>& operator*=(Matrix const & m, T const & s);






} //Namespace Mat

#endif //MATRIX_HPP

