#include "Vector.hpp"


namespace Mat
{

	/////////////////////////////
	//Class InvalidIndexException

	InvalidIndexException::InvalidIndexException(std::string const & _message, VectorIndex const & _indexPos)
		: message(_message), indexPos(_indexPos)
	{}




	////////////////////////////////////////
	//Class IncompatibleVectorSizesException

	IncompatibleVectorSizesException::IncompatibleVectorSizesException(std::string const & _message, VectorSize const & _size1, VectorSize const & _size2)
		: message(_message), size1(_size1), size2(_size2)
	{}




} //Namespace: Mat

