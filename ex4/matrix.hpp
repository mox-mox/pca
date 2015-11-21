/**
 * \brief Implementation of a matrix multiplication
 *
 * The class 
 *
 *
 *
 */
#pragma once

#include <cstdint>
#include <ostream>




class Matrix
{
	private:
		const uint32_t N;
		bool transposed;
		double** array;
	public:
		Matrix(uint32_t N, bool init=false);
		Matrix(const Matrix &that);

		~Matrix();
		std::ostream& outputToStream(std::ostream& output) const;
		Matrix& operator=(Matrix that);

		void transpose();


		friend const Matrix operator*(Matrix const& lhs, Matrix const& rhs);
};


