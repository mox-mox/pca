#include "matrix.hpp"
#include <iostream>
#include <ctime>







#define size 2048
//#define size 5
int main()
{
	std::srand(std::time(0));		// use current time as seed for random generator

	Matrix m1(size, true);
	Matrix m2(size, true);

#ifdef OPTIMISE_TRANSPOSE
#pragma message"Using transposed optimisation."
	m2.transpose();
#else
#pragma message"NOT using transposed optimisation."
#endif

	clock_t t = clock();
	Matrix m3(m1*m2);
	t = clock() - t;

	//std::cout<<"Calculated "<<m1<<"*"<<m2<<"="<<m3<<" in "<<t<<" clicks ("<<static_cast < double > (t)/CLOCKS_PER_SEC<<"s)."<<std::endl;
	std::cout<<"Calculated in "<<t<<" clicks ("<<static_cast < double > (t)/CLOCKS_PER_SEC<<"s)."<<std::endl;


	return 0;
}

