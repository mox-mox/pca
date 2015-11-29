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




#ifdef VECTORISE
int num_threads;
#endif


//#define NUM_ITERATIONS 100 // Number of runns for a Benchmark
#define NUM_ITERATIONS 1 // Number of runns for a Benchmark
//#define N 700              // Size of the array
#define N 20              // Size of the array

#include "getopt_pp.hpp"

//using namespace GetOpt;

int main(int argc, char* argv[])
{
	int steps;
	int radius;
	double heat;

	int ret=1;

	GetOpt::GetOpt_pp ops(argc, argv);

	ops.exceptions(std::ios::failbit | std::ios::eofbit);

	try
	{
		ops >> GetOpt::Option('s', "steps", steps, 100);
		ops >> GetOpt::Option('r', "radius", radius, 3);
		ops >> GetOpt::Option('h', "heat", heat, 127.0);
#ifdef VECTORISE
		ops >> GetOpt::Option('t', "threads", num_threads, 12);
#endif
	}
	catch(GetOpt::GetOptEx ex)
	{
		std::cerr << "Error in arguments" << std::endl;
		std::cerr << "Usage: "<<argv[0]<<" [options]"<<std::endl;
		std::cerr << "Options:"<<std::endl;
		std::cerr << "-s|--steps   <NUM>: Set the number of simulation steps."<<std::endl;
		std::cerr << "-r|--radius  <NUM>: Set the radius of the stimuli."<<std::endl;
		std::cerr << "-h|--heat    <NUM>: Set the heat of the stimuli."<<std::endl;
#ifdef VECTORISE
		std::cerr << "-t|--threads <NUM>:    Set the number of threads that are started for the computation of the multiplication."<<std::endl;
#endif
		return -1;
	}

		uint64_t t0;
		uint64_t t1;
		uint64_t t_ges=0;
		std::cout<<"Timing for N = "<<N<<" with "<<steps<<" iterations"<<std::endl;

		for(int i=0; i < NUM_ITERATIONS; i++)
		{
			rdtsc(t0);
			Relaxation < N > relax(radius, heat);
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
		std::cout<<"Timing: Used "<<t_ges<<" clock cycles total, "<<t_ges/NUM_ITERATIONS<<" cycles per run on average. (for N = "<<N<<", "<<steps<<" steps and "<<NUM_ITERATIONS<<" repetitions)"<<std::endl;

	return ret;
}
