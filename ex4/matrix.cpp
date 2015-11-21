#include "matrix.hpp"
#include <cstdlib>
#include <iostream>
#include <algorithm>


//{{{
Matrix::Matrix(uint32_t N_, bool init) : N(N_), transposed(false)
{
	//std::cout<<"Matrix(...)"<<std::endl;
	array = new double*[N];
	for(uint32_t i = 0; i < N; i++)
	{
		array[i] = new double[N];
	}
	if(init)
	{
		for(uint32_t i = 0; i < N; i++)
		{
			for(uint32_t j=0; j < N; j++)
			{
				array[i][j] = static_cast < unsigned int > (std::rand() % 10);
			}
		}
	}
}
//}}}

//{{{
Matrix::Matrix(const Matrix& that) : N(that.N), transposed(that.transposed)
{
	//std::cout<<"foobar"<<std::endl;
	array = new double*[N];
	for(uint32_t i = 0; i < N; i++)
	{
		array[i] = new double[N];
		for(uint32_t j=0; j < N; j++)
		{
			//std::cout<<that.array[i][j]<<std::endl;
			array[i][j] = that.array[i][j];
		}
	}
}
//}}}

//{{{
Matrix::~Matrix()
{
	//std::cout<<"~Matrix()"<<std::endl;
	for(uint32_t i = 0; i < N; ++i)
	{
		delete[] array[i];
	}
	delete[] array;
}
//}}}

//{{{
//Matrix& Matrix::operator=(Matrix that)
//{
//	std::cout<<"asdfasfda"<<std::endl;
//	std::swap(array, that.array);
//	return *this;
//}
////}}}

//{{{
Matrix& Matrix::operator=(Matrix that)
{
	std::cout<<"asdfasfda"<<std::endl;
	for(uint32_t i = 0; i < N; i++)
	{
		for(uint32_t j=0; j < N; j++)
		{
			std::cout<<that.array[i][j]<<std::endl;
			array[i][j] = that.array[i][j];
		}
	}
	return *this;
}
//}}}

//{{{
std::ostream& Matrix::outputToStream(std::ostream& output) const
{
	output<<std::endl<<std::endl;
	for(uint32_t i = 0; i < N; i++)
	{
		output<<"[ ";
		for(uint32_t j=0; j < N; j++)
		{
			output << array[i][j] << " ";
		}
		if(transposed && !i) output <<"] ^ T"<< std::endl;
		else output <<"]"<< std::endl;
	}
	output<<std::endl<<std::endl;
	return output;
}
//}}}

//{{{
const Matrix operator*(Matrix const& lhs, Matrix const& rhs)
{
	if(lhs.transposed)	// lhs must not be transposed, so untranspose it.
	{
		std::cerr<<"Matrix multiplication with transposed first multiplicant attempted. Performance will be bad."<<std::endl;
		Matrix temp(lhs);
		temp.transpose();
		return temp*rhs;
	}

	const uint32_t N = lhs.N;
	Matrix retval(N, false);
	if(rhs.transposed)
	{
		for(uint32_t j=0; j < N; j++)
		{
			for(uint32_t i = 0; i < N; i++)
			{
				double scalar_product = 0;
				for(uint32_t k=0; k < N; k++)	// scalar product
				{
					scalar_product += rhs.array[j][k]*lhs.array[i][k];
				}
				retval.array[i][j] = scalar_product;
			}
		}
	}
	else
	{
		for(uint32_t j=0; j < N; j++)
		{
			for(uint32_t i = 0; i < N; i++)
			{
				double scalar_product = 0;
				for(uint32_t k=0; k < N; k++)	// scalar product
				{
					scalar_product += rhs.array[k][j]*lhs.array[i][k];
				}
				retval.array[i][j] = scalar_product;
			}
		}
	}

	return retval;
}
//}}}

//{{{
void Matrix::transpose()
{
	transposed = !transposed;
	for(uint32_t i = 1; i < N; i++)
	{
		for(uint32_t j=0; j < i; j++)
		{
			std::swap(array[i][j], array[j][i]);
		}
	}
}
//}}}

//{{{
std::ostream& operator<< (std::ostream& lhs, Matrix const& rhs)
{
    return rhs.outputToStream(lhs);
}
//}}}
