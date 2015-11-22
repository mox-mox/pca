/**
 * This is an implementation of a fixed size matrix supporting the neccesary operations for matrix algebra.
 */
#pragma once
#include "vectmath_vector.hpp"
#include <iomanip>
#include <initializer_list>
#include <algorithm>


template < typename data_t >
class Matrix
{
	private:
		size_t row_size;
		size_t column_size;
		Vector < data_t > **data;
		using iterator = Vector < data_t > **;
	public:
		//{{{ Constructors

		Matrix(size_t size=0);														// Fill with pseudo-random numbers
		Matrix(size_t row_size, size_t column_size);								// Fill with pseudo-random numbers
		Matrix(size_t row_size, size_t column_size, data_t initial_value);			// Fill with initial_value
		Matrix(std::initializer_list < std::initializer_list < data_t >> il);		// Fill with initialiser list
		//Vector(const Vector &other);											// Copy another Vector
		//Vector(Vector && other);												// Move from another Vector
		//~Vector();
		//}}}


		//{{{ Iterators

		iterator begin(void) const;
		iterator end(void) const;
		//const_iterator cbegin(void) const;
		//const_iterator cend(void) const;
		//}}}

//			//{{{ Operators
//
//			Vector& operator=(Vector other);
//			data_t& operator[] (const unsigned int idx);
//			const data_t& operator[] (const unsigned int idx) const;
//			Vector& operator+=(const Vector &other);
//			Vector& operator-=(const Vector &other);
//
//			//{{{ Friend operators
//
		template < typename data_tt > friend std::ostream& operator<< (std::ostream& stream, Matrix < data_tt > const& mat);
//			template < typename data_tt > friend data_tt operator*(const Vector<data_tt>& first, const Vector<data_tt>& second);
//			template < typename data_tt > friend data_tt operator+(const Vector<data_tt> first, const Vector<data_tt>& second);
//			template < typename data_tt > friend data_tt operator-(const Vector<data_tt> first, const Vector<data_tt>& second);
//			//}}}
//			//}}}
//
//			//{{{ Friends
//
//			template < typename data_tt > friend void swap(Vector < data_tt >& first, Vector < data_tt >& second);
//			//}}}
//
};




//{{{    Matrix(size_t size)

template < typename data_t >
Matrix < data_t > ::Matrix(size_t size) : row_size(size), column_size(size), data(row_size ? new Vector < data_t > *[row_size] : nullptr)
{
	for(auto& it : *this)
		it = new Vector < data_t > (column_size);
}
//}}}
//{{{    Matrix(size_t row_size, size_t column_size)

template < typename data_t >
Matrix < data_t > ::Matrix(size_t row_size, size_t column_size) : row_size(row_size), column_size(column_size), data(row_size ? new Vector < data_t > *[row_size] : nullptr)
{
	for(auto& it : *this)
		it = new Vector < data_t > (column_size);
}
//}}}
//{{{    Matrix(size_t row_size, size_t column_size, data_t initial_value)

template < typename data_t >
Matrix < data_t > ::Matrix(size_t row_size, size_t column_size, data_t initial_value) : row_size(row_size), column_size(column_size), data(row_size ? new Vector < data_t > *[row_size] : nullptr)
{
	for(auto& it : *this)
		it = new Vector < data_t > (column_size, initial_value);
}
//}}}
//{{{    Matrix(initializer_list il)

template < typename data_t >
Matrix < data_t > ::Matrix(std::initializer_list < std::initializer_list < data_t >> il) : row_size(il.size()), column_size (il.begin().size()), data(row_size ? new Vector < data_t > *[row_size] : nullptr)
{
	//std::copy(il.begin(), il.end(), this->begin());
}
//}}}





//{{{    Matrix::begin()

	                                                                                                                         template < typename data_t >
	                                                                                                                         typename Matrix < data_t > ::iterator Matrix < data_t > ::begin(void) const
{
	return data+0;
}
//}}}
//{{{    Matrix::end()

	                                                                                                                         template < typename data_t >
	                                                                                                                         typename Matrix < data_t > ::iterator Matrix < data_t > ::end(void) const
{
	return data+row_size;
}
//}}}





//{{{  operator<< (ostream lhs, Vector rhs)

	                                                                                                                         template < typename data_t >
	                                                                                                                         std::ostream& operator<< (std::ostream& stream, Matrix < data_t > const& mat)
	    {
	    stream<<std::endl;
	    for(const auto it : mat)
	    {
	        stream<<"["<<*it<<"]"<<std::endl;
	        ;
		}
	    if(!mat.row_size) stream<<"--empty-- "<<std::endl; ;
	    return stream;
		}
//}}}
