#pragma once
#include <iomanip>
#include <initializer_list>	// std::initializer_list
#include <algorithm>


namespace vectmath
{
	template < typename data_t >
	class Vector
	{
		private:
			size_t size;
			data_t* data;
			using iterator = data_t*;
			using const_iterator = const data_t*;
		public:
			//{{{ Constructors

			Vector(size_t size=0);									// Fill with pseudo-random numbers
			Vector(size_t size, data_t initial_value);				// Fill with initial_value
			Vector(std::initializer_list < data_t > il);			// Fill with initialiser list
			Vector(const Vector &other);							// Copy another Vector
			Vector(Vector && other);								// Move from another Vector
			~Vector();
			//}}}

			//{{{ Iterators

			iterator begin(void) const;
			iterator end(void) const;
			const_iterator cbegin(void) const;
			const_iterator cend(void) const;
			//}}}

			//{{{ Operators

			Vector& operator=(Vector other);
			data_t& operator[] (const unsigned int idx);
			const data_t& operator[] (const unsigned int idx) const;
			Vector& operator+=(const Vector &other);
			//}}}


			//{{{ Friends
			template < typename data_tt >
			friend void swap(Vector < data_tt >& first, Vector < data_tt >& second);
			template < typename data_tt >
			friend data_tt operator*(const Vector<data_tt>& first, const Vector<data_tt>& second);
			template < typename data_tt >
			friend std::ostream& operator<< (std::ostream& stream, Vector < data_tt > const& vect);
			//}}}
	};




//{{{    Vector(size_t size)

	template < typename data_t >
	Vector < data_t > ::Vector(size_t size) : size(size), data(size ? new data_t[size] : 0)
	{
		for(data_t& it : *this)
		{
			it = std::rand();
		}
	}
//}}}
//{{{    Vector(size_t size, data_t initial_value)

	template < typename data_t >
	Vector < data_t > ::Vector(size_t size, data_t initial_value) : size(size), data(size ? new data_t[size] : 0)
	{
		for(data_t& it : *this)
		{
			it = initial_value;
		}
	}
//}}}
//{{{    Vector(initializer_list il)

	template < typename data_t >
	Vector < data_t > ::Vector(std::initializer_list < data_t > il) : size(il.size()), data(size ? new data_t[size] : 0)
	{
		std::copy(il.begin(), il.end(), this->begin());
	}
//}}}
//{{{    Vector(const Vector& other)

	template < typename data_t >
	Vector < data_t > ::Vector(const Vector &other) : size(other.size), data(size ? new data_t[size] : 0)
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
//{{{    Vector::cbegin()

	template < typename data_t >
	typename Vector < data_t > ::const_iterator Vector < data_t > ::cbegin(void) const
	{
		return data;
	}
//}}}
//{{{    Vector::cend()

	template < typename data_t >
	typename Vector < data_t > ::const_iterator Vector < data_t > ::cend(void) const
	{
		return &data[size];
	}
//}}}


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
//{{{  void swap(Vector& first, Vector& second)

	template < typename data_t >
	void swap(Vector < data_t >& first, Vector < data_t > second)
	{
		using std::swap;
		swap(first.size, second.size);
		swap(first.data, second.data);
	}
//}}}
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











}	/* namespace vectmath */
