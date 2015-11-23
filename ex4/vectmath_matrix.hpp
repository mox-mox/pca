/**
 * This is an implementation of a fixed size matrix supporting the neccesary operations for matrix algebra.
 */
#pragma once
#include "vectmath_vector.hpp"
#include <iomanip>
#include <initializer_list>
#include <algorithm>
#include <memory>

#ifdef VECTORISE
extern int num_threads;	// When using the vectorised version, the number of threads to use has to be a global variable.
#include <pthread.h>
#endif

namespace vectmath
{
	template < typename data_t >
	class Matrix
	{
		private:
			size_t row_size;
			size_t column_size;
			Vector < data_t > *data;
			using iterator = Vector < data_t > *;
		public:
			//{{{ Constructors

			Matrix(size_t size=0);													// Fill with pseudo-random numbers
			Matrix(size_t row_size, size_t column_size);							// Fill with pseudo-random numbers
			Matrix(size_t row_size, size_t column_size, data_t initial_value);		// Fill with initial_value
			Matrix(const std::initializer_list < std::initializer_list < data_t>>&il);	// Fill with initialiser list
			Matrix(const Matrix &other);											// Copy another Vector
			Matrix(Matrix && other);												// Move from another Vector
			~Matrix();
			//}}}

			//{{{ Iterators

			iterator begin(void) const;
			iterator end(void) const;
			//}}}

			//{{{ Operators

			Matrix&               operator= (Matrix other);
			Vector<data_t>&       operator[] (const unsigned int idx);
			const Vector<data_t>& operator[] (const unsigned int idx) const;
			Matrix&               operator+=(const Matrix &other);
			Matrix&               operator-=(const Matrix &other);
			//}}}

			//{{{ Thread workers

#ifdef VECTORISE
			struct Thread_args {const Matrix<data_t>* arg1=nullptr; const Vector<data_t>* arg2=nullptr; Vector<data_t>* retval=nullptr; unsigned int thread_number=0; };
			static void* matrix_by_vector_thread(void* args);
#endif

			//}}}

			//{{{ Helper functions

			inline size_t rows(void) const;
			inline size_t columns(void) const;
			template < typename data_tt > friend void swap(Matrix < data_tt >& first, Matrix < data_tt >& second);
			//}}}


			//{{{ Static unittests

#ifdef DEBUG
			static bool test(void);
			static bool test_vector_product(void);
#endif
			//}}}
	};



	//{{{ Constructors

//{{{    Matrix(size_t size)

	template < typename data_t >
	Matrix < data_t > ::Matrix(size_t size) : row_size(size), column_size(size), data(row_size ? new Vector < data_t >[row_size] : nullptr)
	{
		for(Vector < data_t > &it : *this) { new(&it) Vector < data_t > (column_size); }
	}
//}}}
//{{{    Matrix(size_t row_size, size_t column_size)

	template < typename data_t >
	Matrix < data_t > ::Matrix(size_t row_size, size_t column_size) : row_size(row_size), column_size(column_size), data(row_size ? new Vector < data_t >[row_size] : nullptr)
	{
		for(Vector < data_t > &it : *this) { new(&it) Vector < data_t > (column_size); }
	}
//}}}
//{{{    Matrix(size_t row_size, size_t column_size, data_t initial_value)

	template < typename data_t >
	Matrix < data_t > ::Matrix(size_t row_size, size_t column_size, data_t initial_value) : row_size(row_size), column_size(column_size), data(row_size ? new Vector < data_t >[row_size] : nullptr)
	{
		for(Vector < data_t > &it : *this) { new(&it) Vector < data_t > (column_size, initial_value); }
	}
//}}}
//{{{    Matrix(initializer_list<initializer_list<data_t>>)

	template < typename data_t >
	Matrix < data_t > ::Matrix(const std::initializer_list < std::initializer_list < data_t >>&il) : row_size(il.size()), column_size(il.begin()->size()), data(row_size ? new Vector < data_t >[row_size] : nullptr)
	{
		const std::initializer_list < data_t > * init_list(il.begin());
		for(Vector < data_t > &it : *this) { new(&it) Vector < data_t > (*init_list++); }
	}
//}}}
//{{{    Matrix(const Matrix& other)

	template < typename data_t >
	Matrix < data_t > ::Matrix(const Matrix &other) : row_size(other.row_size), column_size(other.column_size), data(column_size ? new Vector < data_t >[row_size] : nullptr)
	{
		const Vector < data_t > * orig = other.begin();
		for(Vector < data_t > &it : *this) { new(&it) Vector < data_t > (*orig++); }
	}
//}}}
//{{{    Matrix(Matrix&& other)

	template < typename data_t >
	Matrix < data_t > ::Matrix(Matrix && other) : Matrix()
	{
		swap((*this), other);
	}
//}}}
//{{{    ~Matrix()

	template < typename data_t >
	Matrix < data_t > ::~Matrix()
	{
		delete[]  data;
	}
//}}}
	//}}}

	//{{{ Iterators

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
	//}}}

	//{{{ Operators

//{{{    Matrix&       Matrix::operator= (Matrix other)

	template < typename data_t >
	Matrix < data_t >& Matrix < data_t > ::operator=(Matrix other)
	{
		swap((*this), other);
		return *this;
	}
//}}}
//{{{    Vector&       Matrix::operator[](const unsigned int idx)

	template < typename data_t >
	Vector < data_t > &Matrix < data_t > ::operator[] (const unsigned int idx)
	{
#ifdef DEBUG
		if(idx >= row_size) throw std::out_of_range("Trying to access element "+std::to_string (idx)+"of a Matrix with row_length "+std::to_string (row_size)+".");
#endif
		return data[idx];
	}
//}}}
//{{{    const Vector& Matrix::operator[](const unsigned int idx)

	template < typename data_t >
	const Vector < data_t > &Matrix < data_t > ::operator[] (const unsigned int idx) const
	{
#ifdef DEBUG
		if(idx >= row_size) throw std::out_of_range("Trying to access element "+std::to_string (idx)+"of a Matrix with row_length "+std::to_string (row_size)+".");
#endif
		return data[idx];
	}
//}}}
//{{{    Matrix&       Matrix::operator+=(Matrix other)

	template < typename data_t >
	Matrix < data_t >& Matrix < data_t > ::operator+=(const Matrix &other)
	{
#ifdef DEBUG
		if(other.row_size != row_size) throw std::logic_error("Trying to add Matrixs of different sizes.");
#endif
		for(unsigned int i=0; i < row_size; i++) (*this)[i] += other[i];
		return *this;
	}
//}}}
//{{{    Matrix&       Matrix::operator-=(Matrix other)

	template < typename data_t >
	Matrix < data_t >& Matrix < data_t > ::operator-=(const Matrix &other)
	{
#ifdef DEBUG
		if(other.row_size != row_size) throw std::logic_error("Trying to add Matrixs of different sizes.");
#endif
		for(unsigned int i=0; i < row_size; i++) (*this)[i] -= other[i];
		return *this;
	}
//}}}

//{{{    bool                  operator==(Matrix& first, Matrix& second)

	template < typename data_t >
	bool operator == (const Matrix < data_t > &first, const Matrix < data_t > &second)
	{
		if(first.rows() != second.rows()) return false;
		for(unsigned int i=0; i < first.rows(); i++) if(first[i] != second[i]) return false;

		return true;
	}
//}}}
//{{{    bool                  operator!=(Matrix& first, Matrix& second)

	template < typename data_t >
	bool operator != (const Matrix < data_t > &first, const Matrix < data_t > &second)
	{
		return !(first == second);
	}
//}}}
//{{{    Vector                operator* (Vector& first, Vector& second)

	template < typename data_t >
	Vector < data_t > operator*(const Matrix < data_t > &first, const Vector < data_t > &second)
	{
#ifdef DEBUG
		if(first.columns() != second.length()) throw std::logic_error("Trying to multiply vectors of different sizes.");
#endif
		Vector < data_t > retval(first.rows()); // Where to store the result
#ifdef VECTORISE
		pthread_t* threads= new pthread_t[num_threads];
		typename Matrix<data_t>::Thread_args* thread_args = new typename Matrix<data_t>::Thread_args[num_threads];						// Having no VLA in C++ sucks
		for(int i=0; i<num_threads; i++)
		{
			new(&thread_args[i]) typename Matrix<data_t>::Thread_args {&first, &second, &retval, static_cast<unsigned int>(i) };		// Initialise each thread data structure
			pthread_create(&threads[i], nullptr, &Matrix<data_t>::matrix_by_vector_thread, reinterpret_cast<void*>(&thread_args[i]));	// And having to cast everything to void* is a disappointment, too.
		}
		for(int i=0; i<num_threads; i++)
		{
			 pthread_join(threads[i], nullptr);
		}
		delete[] threads;
		delete[] thread_args;
#else
		for(unsigned int i=0; i < first.rows(); i++) retval[i]=first[i]*second;
#endif
		return retval;
	}
//}}}
//{{{                          operator<<(ostream lhs, Matrix rhs)

	template < typename data_t >
	std::ostream& operator<< (std::ostream& stream, Matrix < data_t > const& mat)
    {
	    stream<<std::endl;
	    for(const auto& it : mat) { stream<<"["<<it<<"]"<<std::endl; }
	    if(!mat.rows()) stream<<"--empty-- "<<std::endl; ;
	    return stream;
	}
//}}}
	//}}}

//{{{     Thread workers


#ifdef VECTORISE
//{{{    void* Matrix::matrix_by_vector_thread(void* args)

	    template < typename data_t >
		void* Matrix<data_t>::matrix_by_vector_thread(void* arguments)
		{
			typename Matrix<data_t>::Thread_args* args = reinterpret_cast<typename Matrix<data_t>::Thread_args*>(arguments);
			for(unsigned int i=args->thread_number; i < args->arg1->rows(); i+=num_threads)
			{
				//std::cout<<"Thread_worker["<<args->thread_number<<"], slice("<<i<<")"<<std::endl;
				(*(args->retval))[i]=(*(args->arg1))[i]*(*(args->arg2));
			}
			return nullptr;
		}
//}}}
#endif

//}}}

//{{{     Helper functions

//{{{    size_t Matrix::rows()

	    template < typename data_t >
	    size_t Matrix < data_t > ::rows(void) const
	    {
		    return row_size;
		}
//}}}
//{{{    size_t Matrix::columns()

	    template < typename data_t >
	    size_t Matrix < data_t > ::columns(void) const
	    {
		    return column_size;
		}
//}}}
//{{{    void swap(Matrix&, Matrix&)

	    template < typename data_tt >
	    void swap(Matrix < data_tt >& first, Matrix < data_tt >& second)
	    {
		    std::swap(first.row_size, second.row_size);
		    std::swap(first.column_size, second.column_size);
		    std::swap(first.data, second.data);
		}
//}}}
//}}}

//{{{     Static unittests

#ifdef DEBUG
//{{{    bool Matrix::test()

	    template < typename data_t >
	    bool Matrix<data_t>::test(void)
	    {
			bool retval = true;
			std::cout<<"	Testing Matrix class:"<<std::endl;
			!test_vector_product() && (retval=false);

			std::cout<<(retval?"	-> Success!":"-> Failure!")<<std::endl;
			return retval;
		}
//}}}
//{{{    bool Matrix::test_vector_product()

	    template < typename data_t >
	    bool Matrix<data_t>::test_vector_product(void)
	    {
			bool retval=true;
			std::cout<<"		- Vector product: ";
			Vector<data_t>x={1,2,3};
			Matrix<data_t>A={{1,2,3},{4,5,6},{7,8,9},{10,11,12}};
			!(A*x == Vector<data_t>{14,32,50,68}) && (retval=false);
			std::cout<<(retval?"...passed":"...failed")<<std::endl;
			return retval;
		}
//}}}
#endif
//}}}

}	/* namespace vectmath */
