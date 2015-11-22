/**
 * This is an implementation of a fixed size matrix supporting the neccesary operations for matrix algebra.
 */
#pragma once
#include "vectmath_vector.hpp"
#include <iomanip>
#include <initializer_list>
#include <algorithm>
#include <memory>


namespace vectmath
{
	template < typename data_t >
	class Matrix
	{
		private:
			size_t row_size;
			size_t column_size;
			Vector<data_t>* data;
			using iterator = Vector<data_t>*;
		public:
			//{{{ Constructors

			Matrix(size_t size=0);													// Fill with pseudo-random numbers
			Matrix(size_t row_size, size_t column_size);							// Fill with pseudo-random numbers
			Matrix(size_t row_size, size_t column_size, data_t initial_value);		// Fill with initial_value
			Matrix(const std::initializer_list<std::initializer_list<data_t>>& il);	// Fill with initialiser list
			Matrix(const Matrix &other);											// Copy another Vector
			Matrix(Matrix && other);												// Move from another Vector
			~Matrix();
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

			//{{{ Friends

			template < typename data_tt > friend void swap(Matrix < data_tt >& first, Matrix < data_tt >& second);
			//}}}

	};




//{{{    Matrix(size_t size)

	template < typename data_t >
	Matrix < data_t > ::Matrix(size_t size) : row_size(size), column_size(size), data(row_size ? new Vector<data_t>[row_size] : nullptr)
	{
		for(Vector<data_t>& it : *this)
		{
			//std::cout<<"it: "<<&it<<std::endl;
			new(&it) Vector<data_t>(column_size);
		}
	}
//}}}
//{{{    Matrix(size_t row_size, size_t column_size)

	template < typename data_t >
	Matrix < data_t > ::Matrix(size_t row_size, size_t column_size) : row_size(row_size), column_size(column_size), data(row_size ? new Vector<data_t>[row_size] : nullptr)
	{
		for(Vector<data_t>& it : *this)
		{
			//std::cout<<"it: "<<&it<<std::endl;
			new(&it) Vector<data_t>(column_size);
		}
	}
//}}}
//{{{    Matrix(size_t row_size, size_t column_size, data_t initial_value)

	template < typename data_t >
	Matrix < data_t > ::Matrix(size_t row_size, size_t column_size, data_t initial_value) : row_size(row_size), column_size(column_size), data(row_size ? new Vector<data_t>[row_size] : nullptr)
	{
		for(Vector<data_t>& it : *this)
		{
			//std::cout<<"it: "<<&it<<std::endl;
			new(&it) Vector<data_t>(column_size, initial_value);
		}
	}
//}}}
//{{{    Matrix(initializer_list<initializer_list<data_t>>)

	template < typename data_t >
	Matrix<data_t>::Matrix(const std::initializer_list < std::initializer_list<data_t >>& il) : row_size(il.size()), column_size(il.begin()->size()), data(row_size ? new Vector<data_t>[row_size] : nullptr)
	{
		const std::initializer_list<data_t>* init_list(il.begin());
		for(Vector<data_t>& it : *this)
		{
			//std::cout<<"it: "<<&it<<std::endl;
			new(&it) Vector<data_t>(*init_list++);
		}
	}
//}}}
//{{{    Matrix(const Matrix& other)

	template < typename data_t >
	Matrix < data_t > ::Matrix(const Matrix &other) : row_size(other.row_size), column_size(other.column_size), data(column_size ? new Vector<data_t>[row_size] : nullptr)
	{
		const Vector<data_t>* orig = other.begin();
		for(Vector<data_t>& it : *this)
		{
			//std::cout<<"it: "<<&it<<std::endl;
			new(&it) Vector<data_t>(*orig++);
		}
	}
//}}}
//{{{    Matrix(Matrix&& other)

	template < typename data_t >
	Matrix < data_t > ::Matrix(Matrix && other) : Matrix()
	{
		swap(*this, other);
	}
//}}}

//{{{    ~Matrix()

	template < typename data_t >
	Matrix < data_t > ::~Matrix()
	{
		//std::cout<<"Destroying Matrix "<<this<<std::endl;
		delete[]  data;
	}
//}}}




//{{{    Matrix::begin()

	template < typename data_t >
	typename Matrix < data_t > ::iterator Matrix < data_t > ::begin(void) const
	{
		return data;
	}
//}}}
//{{{    Matrix::end()

	template < typename data_t >
	typename Matrix < data_t > ::iterator Matrix < data_t > ::end(void) const
	{
		return &data[row_size];
	}
//}}}





//{{{  void swap(Matrix& first, Vector& second)

	template < typename data_t >
	void swap(Matrix < data_t >& first, Matrix < data_t > second)
	{
		//using std::swap;
		std::swap(first.row_size, second.row_size);
		std::swap(first.column_size, second.column_size);
		std::swap(first.data, second.data);
	}
//}}}
//{{{  operator<< (ostream lhs, Vector rhs)

	template < typename data_t >
	std::ostream& operator<< (std::ostream& stream, Matrix < data_t > const& mat)
    {
		stream<<std::endl;
		for(const auto& it : mat)
		{
			stream<<"["<<it<<"]"<<std::endl;;
		}
		if(!mat.row_size) stream<<"--empty-- "<<std::endl;;
		return stream;
	}
//}}}



}	/* namespace vectmath */
