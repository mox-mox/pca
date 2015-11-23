//#include "matrix.hpp"
#include <iostream>
#include "getopt_pp.hpp"
#include <ctime>
#include "vectmath_vector.hpp"
#include "vectmath_matrix.hpp"
#include "utils.h"
#include <fstream>
#include <sys/stat.h>
#include <sys/time.h>

#ifdef VECTORISE
int num_threads;
#endif

int main(int argc, char** argv)
{
	//{{{ Argument handling:

	int dimensions;
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
#ifdef VECTORISE
		ops >> GetOpt::Option('t', "threads", num_threads, 1);
#endif
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
#ifdef VECTORISE
		std::cerr << "-t|--threads <NUM>:    Set the number of threads that are started for the computation of the multiplication."<<std::endl;
#endif
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
			!vectmath::Vector < float > ::test() && (retval=false);
			!vectmath::Matrix < float > ::test() && (retval=false);
		}
		else
		{
			std::cout<<"double precission floating point values."<<std::endl;
			!vectmath::Vector < double > ::test() && (retval=false);
			!vectmath::Matrix < double > ::test() && (retval=false);
		}
		std::cout<<"Unittests "<<(retval ? "" : "NOT")<<" passed."<<std::endl;
		exit(!retval);
	}
#endif
	//}}}

	std::cout<<"Measuring execution time for calculation of y = A*x with A ∈ Mat("<<dimensions<<"x"<<dimensions<<") and x,y ∈ |R^"<<dimensions<<"."<<std::endl;
	std::cout<<"	- Data type is "<<(use_float ? "float" : "double")<<"."<<std::endl;
#ifdef VECTORISE
	std::cout<<"	- Using up to "<<num_threads<<" threads."<<std::endl;
#endif
	std::cout<<"	- Using "<<iterations<<" iterations."<<std::endl;

	timeval start, end;
	uint64_t microseconds=0;
	uint64_t t0, t1, t_ges=0;
	if(use_float)
	{
		vectmath::Matrix < float > A(dimensions, dimensions);
		vectmath::Vector < float > x(dimensions);
		vectmath::Vector < float > y;
		for(int i=0; i < iterations; i++)
		{
			gettimeofday(&start, 0);
			rdtsc(t0);
			y = A*x;
			rdtsc(t1);
			gettimeofday(&end, 0);
			microseconds += ((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec));
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
			gettimeofday(&start, 0);
			rdtsc(t0);
			y = A*x;
			rdtsc(t1);
			gettimeofday(&end, 0);
			microseconds += ((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec));
			t_ges += t1-t0;
		}
	}

	// Write the results to a file
	std::string path="benchmark_data/";
	{
		struct stat info;
		if(stat(path.c_str(), &info))
		{
			int temp = system(("mkdir "+path).c_str());
			temp=temp;
		}
	}
	std::ofstream data_output;
	std::ostringstream out;
	out << std::setfill('0') << std::setw(5) << dimensions;
	std::string filename = path+"dimension_" + out.str() + (use_float?"_float":"_double") + ".dat";
	bool file_new=false;

	{
		struct stat info;
		if(stat(filename.c_str(), &info)) file_new=true;
	}

	data_output.open(filename, std::ios_base::app);
	if(file_new) data_output<<"# Threads Processor_ticks Runtime"<<std::endl;
	data_output<< std::setfill(' ') << std::setw(2) <<std::to_string(num_threads)<<" "<< std::setfill(' ') << std::setw(20) <<std::to_string(t_ges/iterations)<<" "<<std::to_string(microseconds/iterations)<<std::endl;
	data_output.close();


	std::cout<<"Average execution time was "<<(t_ges/iterations)<<"."<<std::endl;

	return 0;
}
