#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include <algorithm>
#include <functional>



namespace Mat
{
	typedef unsigned int VectorIndex;
	typedef VectorIndex VectorSize;
	typedef VectorIndex VectorEntry;

	////////////////////////////////////////////////////////////////////////////
	//Struct InvalidIndicesException, which can be thrown if the index is invalid
	struct InvalidIndexException
	{
		std::string message;
		VectorIndex indexPos;
		InvalidIndexException(std::string const & _message, VectorIndex const & _indexPos);
	};


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Struct IncompatibleSizesException, which can be thrown if an operation is not possible between two matrices due to their incompatible sizes
	struct IncompatibleVectorSizesException
	{
		std::string message;
		VectorSize size1;
		VectorSize size2;
		IncompatibleVectorSizesException(std::string const & _message, VectorSize const & _size1, VectorSize const & _size2);
	};


	////////////////////////
	//Class Template: Vector
	template <typename T> class Vector
	{
	private:
		std::vector<T> mVec;

	public:
		//Standard constructor that constructs a 0-dim vector
		Vector();


		//Constructor that constructs vector of size size with value value
		explicit Vector(VectorSize const & size, T const & value = T());


		//Constructor that constructs a vector from std::vector
		explicit Vector(std::vector<T> const & vec);


		//Constructor that constructs a vector by moving from std::vector
		explicit Vector(std::vector<T> && vec);


		//Constructor that constructs vector from vector of other type
		template <typename S> explicit Vector(Vector<S> const & other);

	public:
		//Gives access to components
		T& at(VectorEntry const & entry);
		

		//Gives constant access to components
		T const & at(VectorEntry const & entry) const;


		//Returns size of vector
		VectorSize getSize() const;


		//Returns a list of all VectorEntries whose valuese differ from val less or equal than tolerance
		std::list<VectorEntry> find(T const & val, T const & tolerance = T(0)) const;


		//Returns a subvector from origin on with size size (If subvector doesn't fit into this vector, the resulting size will be smaller than size)
		Vector<T> getSubvector(VectorEntry const & origin, VectorSize const & size) const;


		//Resizes this vector and in case of enhancing the size fills the new components with fillValue
		void resize(VectorSize size, T const & fillValue = T());


		//Fills the whole vector with value
		void fillWith(T const & value);


		//Does action for every entry in this vector
		void doForEveryEntry(std::function<void(T& entry, VectorEntry const & entryPos)> action);


	}; //Class Template: Vector


	template <typename T> std::ostream& operator<<(std::ostream& oStream, Vector<T> const & mat);




	//Returns this vector
	template <typename T> Vector<T> operator+(Vector<T> const & vec);


	//Returns the negation of this vector
	template <typename T> Vector<T> operator-(Vector<T> const & vec);


	//Componentwise addition
	template <typename T> Vector<T> operator+(Vector<T> const & vec1, Vector<T> const & vec2);


	//Componentwise subtraction
	template <typename T> Vector<T> operator-(Vector<T> const & vec1, Vector<T> const & vec2);


	//Entrywise multiplication with scalar from right
	template <typename T> Vector<T> operator*(Vector<T> const & vec, T const & scalar);


	//Entrywise multiplication with scalar from left
	template <typename T> Vector<T> operator*(T const & scalar, Vector<T> const & vec);


	//Entrywise division by scalar
	template <typename T> Vector<T> operator/(Vector<T> const & vec, T const & scalar);


	//Inner product
	template <typename T> T operator*(Vector<T> const & vec1, Vector<T> const & vec2);


	//Adds vec2 to vec1
	template <typename T> Vector<T>& operator +=(Vector<T>& vec1, Vector<T> const & vec2);


	//Subtracts vec2 from vec1
	template <typename T> Vector<T>& operator -=(Vector<T>& vec1, Vector<T> const & vec2);


	//Multiplies vec with scalar
	template <typename T> Vector<T>& operator *=(Vector<T>& vec, T const & scalar);


	//Divides vec by scalar
	template <typename T> Vector<T>& operator /=(Vector<T>& vec, T const & scalar);



} //Namespace: Mat

#endif //VECTOR_HPP

