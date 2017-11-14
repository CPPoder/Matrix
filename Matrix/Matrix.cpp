#include "Matrix.hpp"

namespace Mat
{

	///////////
	//Struct XY
	
	XY::XY(unsigned int _x, unsigned int _y) 
		: x(_x), y(_y) 
	{}



	///////////
	//Struct MN

	MN::MN(unsigned int _m, unsigned int _n) 
		: m(_m), n(_n)
	{}




	/////////////////////
	//Class MatrixIndices

	MatrixIndices::MatrixIndices(XY const & xy)
		: mXY(xy)
	{}

	MatrixIndices::MatrixIndices(MN const & mn)
		: mXY(mn.n, mn.m)
	{}

	void MatrixIndices::swap(MatrixIndices & other)
	{
		std::swap(this->mXY, other.mXY);
	}

	void MatrixIndices::flip()
	{
		std::swap(mXY.x, mXY.y);
	}


	XY MatrixIndices::xy() const
	{
		return mXY;
	}
	MN MatrixIndices::mn() const
	{
		return MN(mXY.y, mXY.x);
	}

	unsigned int & MatrixIndices::x()
	{
		return mXY.x;
	}
	unsigned int const & MatrixIndices::x() const
	{
		return mXY.x;
	}

	unsigned int & MatrixIndices::y()
	{
		return mXY.y;
	}
	unsigned int const & MatrixIndices::y() const
	{
		return mXY.y;
	}

	unsigned int & MatrixIndices::m()
	{
		return mXY.y;
	}
	unsigned int const & MatrixIndices::m() const
	{
		return mXY.y;
	}

	unsigned int & MatrixIndices::n()
	{
		return mXY.x;
	}
	unsigned int const & MatrixIndices::n() const
	{
		return mXY.x;
	}


	bool MatrixIndices::operator==(MatrixIndices const & other)
	{
		return ((this->x() == other.x()) && (this->y() == other.y()));
	}
	bool MatrixIndices::operator!=(MatrixIndices const & other)
	{
		return !(*this == other);
	}


	std::ostream& operator<<(std::ostream& oStream, MatrixIndices const & m)
	{
		oStream << "(" << m.x() << ", " << m.y() << ")";
		return oStream;
	}




	///////////////////////////////
	//Class InvalidIndicesException

	InvalidIndicesException::InvalidIndicesException(std::string const & _message, MatrixIndices const & _indicesPos)
		: message(_message), indicesPos(_indicesPos) 
	{}








	/////////////////////////////////
	//Class InvalidVecOfRowsException

	InvalidVecOfRowsException::InvalidVecOfRowsException(std::string const & _message)
		: message(_message) 
	{}












	//////////////////////////////////
	//Class IncompatibleSizesException

	IncompatibleSizesException::IncompatibleSizesException(std::string const & _message, MatrixSize const & _size1, MatrixSize const & _size2)
		: message(_message), size1(_size1), size2(_size2)
	{}







} //Namespace: Mat

