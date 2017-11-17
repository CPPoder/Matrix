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
		Vector()
			: mVec()
		{}


		//Constructor that constructs vector of size size with value value
		explicit Vector(VectorSize const & size, T const & value = T())
			: mVec(size, value)
		{}


		//Constructor that constructs a vector from std::vector
		explicit Vector(std::vector<T> const & vec)
			: mVec(vec)
		{}


		//Constructor that constructs a vector by moving from std::vector
		explicit Vector(std::vector<T> && vec)
			: mVec(std::move(vec))
		{}


	public:
		//Gives access to components
		T& at(VectorEntry const & entry)
		{
			if (entry >= mVec.size())
			{
				throw InvalidIndexException("Vector<T>::at(VectorEntry const & entry): entry is not a valid index!", entry);
			}
			return mVec.at(entry);
		}
		

		//Gives constant access to components
		T const & at(VectorEntry const & entry) const
		{
			if (entry >= mVec.size())
			{
				throw InvalidIndexException("Vector<T>::at(VectorEntry const & entry) const: entry is not a valid index!", entry);
			}
			return mVec.at(entry);
		}


		//Returns size of vector
		VectorSize getSize() const
		{
			return mVec.size();
		}


		//Constructor that constructs vector from vector of other type
		template <typename S> explicit Vector(Vector<S> const & other)
			: Vector(other.getSize())
		{
			for (unsigned int i = 0; i < other.getSize(); ++i)
			{
				mVec.at(i) = static_cast<T>(other.at(i));
			}
		}


		//Returns a list of all VectorEntries whose valuese differ from val less or equal than tolerance
		std::list<VectorEntry> find(T const & val, T const & tolerance = T(0)) const
		{
			std::list<VectorEntry> list;
			for (unsigned int i = 0; i < this->getSize(); ++i)
			{
				if (std::abs(this->at(i) - val) <= tolerance)
				{
					list.push_back(i);
				}
			}
			return list;
		}


		//Returns a subvector from origin on with size size (If subvector doesn't fit into this vector, the resulting size will be smaller than size)
		Vector<T> getSubvector(VectorEntry const & origin, VectorSize const & size) const
		{
			VectorSize resultSize = std::min(size, this->getSize() - origin);
			Vector<T> subVec(resultSize);
			for (unsigned int i = 0; i < resultSize; ++i)
			{
				subVec.at(i) = this->at(origin + i);
			}
			return subVec;
		}


		//Resizes this vector and in case of enhancing the size fills the new components with fillValue
		void resize(VectorSize size, T const & fillValue = T())
		{
			mVec.resize(size, fillValue);
		}


		//Fills the whole vector with value
		void fillWith(T const & value)
		{
			for (auto & val : mVec)
			{
				val = value;
			}
		}


		//Does action for every entry in this vector
		void doForEveryEntry(std::function<void(T& entry, VectorEntry const & entryPos)> action)
		{
			for (unsigned int i = 0; i < this->getSize(); ++i)
			{
				T& entry = this->at(i);
				action(entry, i);
			}
		}


	}; //Class Template: Vector


	template <typename T> std::ostream& operator<<(std::ostream& oStream, Vector<T> const & vec)
	{
		bool first = true;
		for (unsigned int i = 0; i < vec.getSize(); ++i)
		{
			if (first)
			{
				first = false;
			}
			else
			{
				oStream << " ";
			}
			oStream << vec.at(i);
		}
		return oStream;
	}




	//Returns this vector
	template <typename T> Vector<T> operator+(Vector<T> const & vec)
	{
		return vec;
	}


	//Returns the negation of this vector
	template <typename T> Vector<T> operator-(Vector<T> const & vec)
	{
		Vector<T> res(vec);
		res.doForEveryEntry([](T& entry, VectorEntry const & pos) {entry = -entry; });
		return res;
	}


	//Componentwise addition
	template <typename T> Vector<T> operator+(Vector<T> const & vec1, Vector<T> const & vec2)
	{
		if (vec1.getSize() != vec2.getSize())
		{
			throw IncompatibleVectorSizesException("operator+(Vector<T> const & vec1, Vector<T> const & vec2: The vectors don't have the same size!", vec1.getSize(), vec2.getSize());
		}
		Vector<T> res(vec1);
		for (unsigned int i = 0; i < vec1.getSize(); ++i)
		{
			res.at(i) += vec2.at(i);
		}
		return res;
	}


	//Componentwise subtraction
	template <typename T> Vector<T> operator-(Vector<T> const & vec1, Vector<T> const & vec2)
	{
		if (vec1.getSize() != vec2.getSize())
		{
			throw IncompatibleVectorSizesException("operator-(Vector<T> const & vec1, Vector<T> const & vec2: The vectors don't have the same size!", vec1.getSize(), vec2.getSize());
		}
		Vector<T> res(vec1);
		for (unsigned int i = 0; i < vec1.getSize(); ++i)
		{
			res.at(i) -= vec2.at(i);
		}
		return res;
	}


	//Entrywise multiplication with scalar from right
	template <typename T> Vector<T> operator*(Vector<T> const & vec, T const & scalar)
	{
		Vector<T> res(vec);
		res.doForEveryEntry([&scalar](T& entry, VectorEntry const & pos) {entry *= scalar; });
		return res;
	}


	//Entrywise multiplication with scalar from left
	template <typename T> Vector<T> operator*(T const & scalar, Vector<T> const & vec)
	{
		Vector<T> res(vec);
		res.doForEveryEntry([&scalar](T& entry, VectorEntry const & pos) {entry *= scalar; });
		return res;
	}


	//Entrywise division by scalar
	template <typename T> Vector<T> operator/(Vector<T> const & vec, T const & scalar)
	{
		Vector<T> res(vec);
		res.doForEveryEntry([&scalar](T& entry, VectorEntry const & pos) {entry /= scalar; });
		return res;
	}


	//Inner product
	template <typename T> T operator*(Vector<T> const & vec1, Vector<T> const & vec2)
	{
		if (vec1.getSize() != vec2.getSize())
		{
			throw IncompatibleVectorSizesException("operator*(Vector<T> const & vec1, Vector<T> const & vec2: The vectors don't have the same size!", vec1.getSize(), vec2.getSize());
		}
		T sum = T(0);
		for (unsigned int i = 0; i < vec1.getSize(); ++i)
		{
			sum += vec1.at(i) * vec2.at(i);
		}
		return sum;
	}


	//Adds vec2 to vec1
	template <typename T> Vector<T>& operator+=(Vector<T>& vec1, Vector<T> const & vec2)
	{
		vec1 = vec1 + vec2;
		return vec1;
	}


	//Subtracts vec2 from vec1
	template <typename T> Vector<T>& operator-=(Vector<T>& vec1, Vector<T> const & vec2)
	{
		vec1 = vec1 - vec2;
		return vec1;
	}


	//Multiplies vec with scalar
	template <typename T> Vector<T>& operator*=(Vector<T>& vec, T const & scalar)
	{
		vec = vec * scalar;
		return vec;
	}


	//Divides vec by scalar
	template <typename T> Vector<T>& operator/=(Vector<T>& vec, T const & scalar)
	{
		vec = vec / scalar;
		return vec;
	}



} //Namespace: Mat

#endif //VECTOR_HPP

