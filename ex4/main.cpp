//#include "matrix.hpp"
#include <iostream>
#include "getopt_pp.hpp"
#include <ctime>
#include "vectmath_vector.hpp"
#include "vectmath_matrix.hpp"
#include "utils.h"


int main(int argc, char** argv)
{
	//{{{ Argument handling:

	int dimensions;
	int threads;
	int seed;
	int iterations;
	bool use_float;
#ifdef DEBUG
	bool verify;
#endif

	GetOpt::GetOpt_pp ops(argc, argv);
	ops.exceptions(std::ios::failbit | std::ios::eofbit);
	try
	{
		ops >> GetOpt::Option('d', "dimensions", dimensions, 4);
		ops >> GetOpt::Option('t', "threads", threads, 1);
		ops >> GetOpt::Option('s', "seed", seed, 0);
		ops >> GetOpt::Option('i', "iterations", iterations, 1);
		ops >> GetOpt::OptionPresent('f', "float", use_float);
#ifdef DEBUG
		ops >> GetOpt::OptionPresent('v', "verify", verify);
#endif
	}
	catch(GetOpt::GetOptEx ex)
	{
		std::cerr << "Error in arguments" << std::endl;
		std::cerr << "Usage: "<<argv[0]<<" [options]"<<std::endl;
		std::cerr << "Options:"<<std::endl;
		std::cerr << "-d|--dimensions <NUM>: Set the matrix to be a NUMxNUM matrix."<<std::endl;
		std::cerr << "-t|--threads <NUM>:    Set the number of threads that are started for the computation of the multiplication."<<std::endl;
		std::cerr << "-s|--seed <NUM>:       Set the seed for the random number generator. This is usefull for testing purposes."<<std::endl;
		std::cerr << "-i|--iterations <NUM>: Set the number of iterations. Longer will probably yield more stable results but take longer."<<std::endl;
		std::cerr << "-f|--float:            Use single precission floats instead of the default double precission floating point values."<<std::endl;
#ifdef DEBUG
		std::cerr << "-v|--verify:           Call the unittest verification routines for the vectmath classes. CAUTION: These tests are quite thin."<<std::endl;
#endif
		exit(-1);
	}
	std::srand(ops >> GetOpt::OptionPresent('s', "seed") ? seed : std::time(0));
#ifdef DEBUG
	if(verify)
	{
		std::cout<<"Running unittests for";
		bool retval=true;
		if(use_float)
		{
			std::cout<<"single precission floating point values."<<std::endl;
			!vectmath::Vector<float>::test() && (retval=false);
			!vectmath::Matrix<float>::test() && (retval=false);
		}
		else
		{
			std::cout<<"double precission floating point values."<<std::endl;
			!vectmath::Vector<double>::test() && (retval=false);
			!vectmath::Matrix<double>::test() && (retval=false);
		}
		std::cout<<"Unittests "<<(retval?"":"NOT")<<" passed."<<std::endl;
		exit(!retval);
	}
#endif
	//}}}

	std::cout<<"Measuring execution time for calculation of y = A*x with A ∈ Mat("<<dimensions<<"x"<<dimensions<<") and x,y ∈ |R^"<<dimensions<<"."<<std::endl;
	std::cout<<"	- Data type is "<<(use_float ? "float" : "double")<<"."<<std::endl;
	std::cout<<"	- Using up to "<<threads<<" threads."<<std::endl;
	std::cout<<"	- Using "<<iterations<<" iterations."<<std::endl;

	uint64_t t0, t1, t_ges=0;
	if(use_float)
	{
		vectmath::Matrix < float > A(dimensions, dimensions);
		vectmath::Vector < float > x(dimensions);
		vectmath::Vector < float > y;
		for(int i=0; i < iterations; i++)
		{
			rdtsc(t0);
			y = A*x;
			rdtsc(t1);
			t_ges += t1-t0;
		}
	}
	else
	{
		vectmath::Matrix < double > A(dimensions, dimensions);
		vectmath::Vector < double > x(dimensions);
		vectmath::Vector < double > y;
		for(int i=0; i < iterations; i++)
		{
			rdtsc(t0);
			y = A*x;
			rdtsc(t1);
			t_ges += t1-t0;
		}
	}



	std::cout<<"Average execution time was "<<(t_ges/iterations)<<"."<<std::endl;

	return 0;
}
