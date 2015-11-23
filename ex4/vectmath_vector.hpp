/**
 * This is an implementation of a fixed size vector supporting the neccesary operations for vector algebra.
 */
#pragma once
#include <iomanip>
#include <initializer_list>
#include <algorithm>


//namespace vectmath
//{
	template < typename data_t >
	class Vector
	{
		private:
			size_t size;
			data_t* data;
			using iterator = data_t*;
			//using const_iterator = const data_t*;
		public:
			//{{{ Constructors

			Vector(size_t size=0);									// Fill with pseudo-random numbers
			Vector(size_t size, data_t initial_value);				// Fill with initial_value
			Vector(const std::initializer_list < data_t >& il);		// Fill with initialiser list
			Vector(const Vector &other);							// Copy another Vector
			Vector(Vector && other);								// Move from another Vector
			~Vector();
			//}}}

			//{{{ Iterators

			iterator begin(void) const;
			iterator end(void) const;
			//const_iterator cbegin(void) const;
			//const_iterator cend(void) const;
			//}}}

			//{{{ Operators

			Vector& operator=(Vector other);
			data_t& operator[] (const unsigned int idx);
			const data_t& operator[] (const unsigned int idx) const;
			Vector& operator+=(const Vector &other);
			Vector& operator-=(const Vector &other);

			//{{{ Friend operators

			template < typename data_tt > friend std::ostream& operator<< (std::ostream& stream, Vector < data_tt > const& vec);
			template < typename data_tt > friend data_tt operator*(const Vector<data_tt>& first, const Vector<data_tt>& second);
			template < typename data_tt > friend Vector<data_tt> operator+(Vector<data_tt> first, const Vector<data_tt>& second);
			template < typename data_tt > friend Vector<data_tt> operator-(Vector<data_tt> first, const Vector<data_tt>& second);
			//}}}
			//}}}

			size_t length(void) const;

			//{{{ Friends

			template < typename data_tt > friend void swap(Vector < data_tt >& first, Vector < data_tt >& second)
	{
		//using std::swap;
		std::swap(first.size, second.size);
		std::swap(first.data, second.data);
	}
			//}}}
	};




//{{{    Vector(size_t size)

	template < typename data_t >
	Vector < data_t > ::Vector(size_t size) : size(size), data(size ? new data_t[size] : nullptr)
	{
		for(data_t& it : *this)
		{
			it = std::rand();
		}
	}
//}}}
//{{{    Vector(size_t size, data_t initial_value)

	template < typename data_t >
	Vector < data_t > ::Vector(size_t size, data_t initial_value) : size(size), data(size ? new data_t[size] : nullptr)
	{
		for(data_t& it : *this)
		{
			it = initial_value;
		}
	}
//}}}
//{{{    Vector(initializer_list il)

	template < typename data_t >
	Vector < data_t > ::Vector(const std::initializer_list < data_t >& il) : size(il.size()), data(size ? new data_t[size] : nullptr)
	{
		std::copy(il.begin(), il.end(), this->begin());
	}
//}}}
//{{{    Vector(const Vector& other)

	template < typename data_t >
	Vector < data_t > ::Vector(const Vector &other) : size(other.size), data(size ? new data_t[size] : nullptr)
	{
		std::copy(other.begin(), other.end(), this->begin());
	}
//}}}
//{{{    Vector(Vector&& other)

	template < typename data_t >
	Vector < data_t > ::Vector(Vector && other) : Vector()
	{
		swap(*this, other);
	}
//}}}
//{{{    ~Vector()

	template < typename data_t >
	Vector < data_t > ::~Vector()
	{
		//std::cout<<"Destroying Vector "<<this<<std::endl;
		delete[]  data;
	}
//}}}


//{{{    Vector::begin()

	template < typename data_t >
	typename Vector < data_t > ::iterator Vector < data_t > ::begin(void) const
	{
		return data;
	}
//}}}
//{{{    Vector::end()

	template < typename data_t >
	typename Vector < data_t > ::iterator Vector < data_t > ::end(void) const
	{
		return &data[size];
	}
//}}}
////{{{    Vector::cbegin()
//
//	template < typename data_t >
//	typename Vector < data_t > ::const_iterator Vector < data_t > ::cbegin(void) const
//	{
//		return data;
//	}
////}}}
////{{{    Vector::cend()
//
//	template < typename data_t >
//	typename Vector < data_t > ::const_iterator Vector < data_t > ::cend(void) const
//	{
//		return &data[size];
//	}
////}}}


//{{{    Vector::operator=(Vector other)

	template < typename data_t >
	Vector < data_t >& Vector < data_t > ::operator=(Vector other)
	{
		swap(*this, other);
		return *this;
	}
//}}}
//{{{    Vector::operator[](const unsigned int idx)

	template < typename data_t >
	data_t& Vector < data_t > ::operator[] (const unsigned int idx)
	{
#ifdef DEBUG
		if(idx >= size) throw std::out_of_range("Trying to access element "+std::to_string (idx)+"of a vector with length "+std::to_string (size)+".");
#endif
		return data[idx];
	}
//}}}
//{{{    Vector::operator[](const unsigned int idx)

	template < typename data_t >
	const data_t& Vector < data_t > ::operator[] (const unsigned int idx) const
	{
#ifdef DEBUG
		if(idx >= size) throw std::out_of_range("Trying to access element "+std::to_string (idx)+"of a vector with length "+std::to_string (size)+".");
#endif
		return data[idx];
	}
//}}}
//{{{    Vector::operator+=(Vector other)

	template < typename data_t >
	Vector < data_t >& Vector < data_t > ::operator+=(const Vector &other)
	{
#ifdef DEBUG
		if(other.size != size) throw std::logic_error("Trying to add vectors of different sizes.");
#endif
		std::transform(other.begin(), other.end(), this->begin(), this->begin(), std::plus < data_t > ());
		return *this;
	}
//}}}
//{{{    Vector::operator-=(Vector other)

	template < typename data_t >
	Vector < data_t >& Vector < data_t > ::operator-=(const Vector &other)
	{
#ifdef DEBUG
		if(other.size != size) throw std::logic_error("Trying to add vectors of different sizes.");
#endif
		std::transform(other.begin(), other.end(), this->begin(), this->begin(), std::minus < data_t > ());
		return *this;
	}
//}}}


////{{{  void swap(Vector& first, Vector& second)
//
//	template < typename data_t >
//	void swap(Vector < data_t >& first, Vector < data_t > second)
//	{
//		//using std::swap;
//		std::swap(first.size, second.size);
//		std::swap(first.data, second.data);
//	}
////}}}
//{{{  operator<< (ostream lhs, Vector rhs)

	template < typename data_t >
	std::ostream& operator<< (std::ostream& stream, Vector < data_t > const& vec)
    {
		stream<<"[ ";
		for(const auto it : vec)
		{
			stream << std::setprecision(4) << std::setfill(' ') << std::setw(9) << it << " ";
		}
		if(!vec.size) stream<<"--empty-- ";
		stream<<"]";
		return stream;
	}
//}}}
//{{{  bool operator==(Vector& first, Vector& second)

	template < typename data_t >
	bool operator==(const Vector<data_t>& first, const Vector<data_t>& second)
	{
		if(first.length() != second.length()) return false;
		for(unsigned int i=0; i<first.length();i++) if(first[i] != second[i]) return false;
		return true;
	}
//}}}
//{{{  bool operator!=(Vector& first, Vector& second)

	template < typename data_t >
	bool operator!=(const Vector<data_t>& first, const Vector<data_t>& second)
	{
		return !(first==second);
	}
//}}}
//{{{  data_t operator*(Vector& first, Vector& second)

	template < typename data_t >
	data_t operator*(const Vector<data_t>& first, const Vector<data_t>& second)
	{
#ifdef DEBUG
		if(first.size != second.size) throw std::logic_error("Trying to multiply vectors of different sizes.");
#endif
		data_t retval=0;
		for(unsigned int i=0; i<first.size;i++) retval+=first[i]*second[i];
		return retval;
	}
//}}}
//{{{  data_t operator+(Vector first, Vector& second)

	template < typename data_t >
	Vector<data_t> operator+(Vector<data_t> first, const Vector<data_t>& second) // First is not a reference because we need a temporary object anyways.
	{
		return first+=second;
	}
//}}}
//{{{  data_t operator-(Vector first, Vector& second)

	template < typename data_t >
	data_t operator-(const Vector<data_t> first, const Vector<data_t>& second) // First is not a reference because we need a temporary object anyways.
	{
		return first-=second;
	}
//}}}

//{{{  size_t Vector::length()

	template < typename data_t >
	size_t Vector<data_t>::length(void) const
	{
		return size;
	}
//}}}

//}	/* namespace vectmath */
