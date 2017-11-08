#ifndef MATRIX_HPP
#define MATRIX_HPP


#include <iostream>
#include <vector>
#include <list>


template <typename T> class Matrix
{
public:
	struct Size
	{
		unsigned int x;
		unsigned int y;
		
		Size(unsigned int _x, unsigned int _y) : x(_x), y(_y) {}
	};

	struct EntryPos
	{
		unsigned int x;
		unsigned int y;

		EntryPos(unsigned int _x, unsigned int _y) : x(_x), y(_y) {}
	};

	struct InvalidIndexException{
		std::string message;
		EntryPos indexPos;
		InvalidIndexException(std::string const & _message, EntryPos const & _indexPos) : message(_message), indexPos(_indexPos) {}
	};

private:
	Size mSize;
	std::vector<std::vector<T>> mVecOfRows;

public:
	//Standard constructor constructs 0x0 matrix
	Matrix();

	//Constructor that constructs matrix of size (sizeX, sizeY) with value
	explicit Matrix(Size const & size, T const & value = T());

	//Constructor that constructs matrix from vec of rows
	explicit Matrix(std::vector<std::vector<T>> const & vecOfRows);

	//Constructor that moves matrix from vec of rows
	explicit Matrix(std::vector<std::vector<T>> && vecOfRows);

	//Constructor that constructs matrix from vec, filling it with number rows or columns depending on asRows
	explicit Matrix(std::vector<T> const & vec, unsigned int number, bool asRows);

	//Destructor and all other constructors stay default!

public:
	//Returns the size of the matrix
	Size getSize() const;

	//Gives access to the components
	T& at(unsigned int x, unsigned int y);

	//Gives constant access to the components
	T const & at(unsigned int x, unsigned int y) const;

	//Gives access to the components
	T& at(EntryPos const & pos);

	//Gives constant access to the components
	T const & at(EntryPos const & pos) const;

	//Calculates trace
	T trace() const;

	//Calculates determinant
	T det() const;

	//Finds all EntryPositions which are equal to val
	std::list<EntryPos> find(T const & val) const;

	//Returns submatrix beginning at origin with size size
	Matrix<T> getSubmatrix(EntryPos const & origin, Size const & size) const;

	//Transposes this matrix
	void transpose();

	//Returns transposed matrix without changing this
	Matrix<T> getTransposed() const;

};














#endif //MATRIX_HPP

