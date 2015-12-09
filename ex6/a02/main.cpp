// Compile with g++ a1.cpp -std=c++11 -Wall -Wextra -Wpedantic -Werror -Wfatal-errors && ./a.out
#include "relaxation.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <utility>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include "utils.h"
#include "getopt_pp.hpp"




#ifdef VECTORISE
	#include <omp.h>																// New addition for ex6
//int num_threads;                                                                  // Removed the global variable for ex6...
#endif

//#ifndef N
//	#define N 700
//#endif

#ifndef REPETITIONS
	#define REPETITIONS 700
#endif


template < typename T, int64_t n >
uint64_t benchmark(int threads);

int steps;
int radius;
double heat;

int main(int argc, char* argv[])
{
//#ifdef VECTORISE
//	int num_threads;                                                                // ... and moved it here.
//#endif

	//{{{ Argument handling

	GetOpt::GetOpt_pp ops(argc, argv);
	ops.exceptions(std::ios::failbit | std::ios::eofbit);
	try
	{
		ops >> GetOpt::Option('s', "steps", steps, 100);
		ops >> GetOpt::Option('r', "radius", radius, 3);
		ops >> GetOpt::Option('h', "heat", heat, 127.0);
//#ifdef VECTORISE
//		ops >> GetOpt::Option('t', "threads", num_threads, 3);
//#endif
	}
	catch(GetOpt::GetOptEx ex)
	{
		std::cerr << "Error in arguments" << std::endl;
		std::cerr << "Usage: "<<argv[0]<<" [options]"<<std::endl;
		std::cerr << "Options:"<<std::endl;
		std::cerr << "-s|--steps   <NUM>: Set the number of simulation steps."<<std::endl;
		std::cerr << "-r|--radius  <NUM>: Set the radius of the stimuli."<<std::endl;
		std::cerr << "-h|--heat    <NUM>: Set the heat of the stimuli."<<std::endl;
//#ifdef VECTORISE
//		std::cerr << "-t|--threads <NUM>:    Set the number of threads that are started for the computation of the multiplication."<<std::endl;
//#endif
		return -1;
	}
	//}}}







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

	{
		std::ofstream data_output;
		std::string filename = path+"benchmark_results_float.dat";
		bool file_new=false;
		{
			struct stat info;
			if(stat(filename.c_str(), &info)) file_new=true;
		}
		data_output.open(filename, std::ios_base::app);
		if(file_new) data_output<<"#Dimension \"1 Thread\" \"2 Threads\" \"4 Threads\" \"8 Threads\" \"16 Threads\" \"32 Threads\"       "<<std::endl;
		constexpr int dims[4] = { 100, 500, 1000, 10000 };

		data_output<< std::setfill(' ') << std::setw(6) <<std::to_string(dims[0]);
		for(int num_threads=1; num_threads < 64; num_threads<<=1)
		{
			std::cout<<"	- Using "<<num_threads<<" threads."<<std::endl;
			int avg_cpu_ticks = benchmark < float, dims[0] > (num_threads);																		// To the benchmark for float;
			data_output<< std::setfill(' ') << std::setw(20) <<std::to_string(avg_cpu_ticks);
		}
		data_output<<std::endl;
		data_output<< std::setfill(' ') << std::setw(6) <<std::to_string(dims[1]);
		for(int num_threads=1; num_threads < 64; num_threads<<=1)
		{
			std::cout<<"	- Using "<<num_threads<<" threads."<<std::endl;
			int avg_cpu_ticks = benchmark < float, dims[1] > (num_threads);																		// To the benchmark for float;
			data_output<< std::setfill(' ') << std::setw(20) <<std::to_string(avg_cpu_ticks);
		}
		data_output<<std::endl;
		data_output<< std::setfill(' ') << std::setw(6) <<std::to_string(dims[2]);
		for(int num_threads=1; num_threads < 64; num_threads<<=1)
		{
			std::cout<<"	- Using "<<num_threads<<" threads."<<std::endl;
			int avg_cpu_ticks = benchmark < float, dims[2] > (num_threads);																		// To the benchmark for float;
			data_output<< std::setfill(' ') << std::setw(20) <<std::to_string(avg_cpu_ticks);
		}
		data_output<<std::endl;
		data_output<< std::setfill(' ') << std::setw(6) <<std::to_string(dims[3]);
		for(int num_threads=1; num_threads < 64; num_threads<<=1)
		{
			std::cout<<"	- Using "<<num_threads<<" threads."<<std::endl;
			int avg_cpu_ticks = benchmark < float, dims[3] > (num_threads);																		// To the benchmark for float;
			data_output<< std::setfill(' ') << std::setw(20) <<std::to_string(avg_cpu_ticks);
		}
		data_output<<std::endl;
		data_output.close();
	}
	{
		std::ofstream data_output;
		std::string filename = path+"benchmark_results_double.dat";
		bool file_new=false;
		{
			struct stat info;
			if(stat(filename.c_str(), &info)) file_new=true;
		}
		data_output.open(filename, std::ios_base::app);
		if(file_new) data_output<<"#Dimension \"1 Thread\" \"2 Threads\" \"4 Threads\" \"8 Threads\" \"16 Threads\" \"32 Threads\"       "<<std::endl;
		constexpr int dims[4] = { 100, 500, 1000, 10000 };

		data_output<< std::setfill(' ') << std::setw(6) <<std::to_string(dims[0]);
		for(int num_threads=1; num_threads < 64; num_threads<<=1)
		{
			std::cout<<"	- Using "<<num_threads<<" threads."<<std::endl;
			int avg_cpu_ticks = benchmark < double, dims[0] > (num_threads);																		// To the benchmark for double;
			data_output<< std::setfill(' ') << std::setw(20) <<std::to_string(avg_cpu_ticks);
		}
		data_output<<std::endl;
		data_output<< std::setfill(' ') << std::setw(6) <<std::to_string(dims[1]);
		for(int num_threads=1; num_threads < 64; num_threads<<=1)
		{
			std::cout<<"	- Using "<<num_threads<<" threads."<<std::endl;
			int avg_cpu_ticks = benchmark < double, dims[1] > (num_threads);																		// To the benchmark for double;
			data_output<< std::setfill(' ') << std::setw(20) <<std::to_string(avg_cpu_ticks);
		}
		data_output<<std::endl;
		data_output<< std::setfill(' ') << std::setw(6) <<std::to_string(dims[2]);
		for(int num_threads=1; num_threads < 64; num_threads<<=1)
		{
			std::cout<<"	- Using "<<num_threads<<" threads."<<std::endl;
			int avg_cpu_ticks = benchmark < double, dims[2] > (num_threads);																		// To the benchmark for double;
			data_output<< std::setfill(' ') << std::setw(20) <<std::to_string(avg_cpu_ticks);
		}
		data_output<<std::endl;
		data_output<< std::setfill(' ') << std::setw(6) <<std::to_string(dims[3]);
		for(int num_threads=1; num_threads < 64; num_threads<<=1)
		{
			std::cout<<"	- Using "<<num_threads<<" threads."<<std::endl;
			int avg_cpu_ticks = benchmark < double, dims[3] > (num_threads);																		// To the benchmark for double;
			data_output<< std::setfill(' ') << std::setw(20) <<std::to_string(avg_cpu_ticks);
		}
		data_output<<std::endl;
		data_output.close();
	}





	//std::cout<<"Timing: Used "<<t_ges<<" clock cycles total, "<<t_ges/REPETITIONS<<" cycles per run on average. (for N = "<<N<<", "<<steps<<" steps and "<<REPETITIONS<<" repetitions)"<<std::endl;

	return 0;
}




template < typename T, int64_t n >
uint64_t benchmark(int threads)
{
#ifdef VECTORISE
	omp_set_num_threads(threads);
	std::cout<<"Benchmarking with "<<omp_get_max_threads()<<" threads."<<std::endl;
#else
	(void) threads;
#endif
	uint64_t t0;
	uint64_t t1;
	uint64_t t_ges=0;
	std::cout<<"Timing for n = "<<n<<" with "<<steps<<" iterations in "<<REPETITIONS<<" runs."<<std::endl;

	for(int i=0; i < REPETITIONS; i++)
	{
		rdtsc(t0);
		Relaxation < T, n > relax(radius, heat);
#ifdef DEBUG
		std::cout<<"Relaxation created"<<std::endl;
		relax.print_grid();
#endif
		for(int s=1; s <= steps; s++)
		{
#ifdef DEBUG
			std::cout<<"Iteration"<<std::endl;
#endif
			relax.iterate();
#ifdef DEBUG
			relax.print_grid();
#endif
		}
		rdtsc(t1);
		t_ges += t1-t0;
#ifdef DEBUG
		std::cout<<"This run took "<<(t1-t0)<<" clock cycles"<<std::endl;
#endif
	}
	return t_ges/REPETITIONS;
}
