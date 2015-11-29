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






#define BOX_SIZE 100

#include "getopt_pp.hpp"

using namespace GetOpt;

int main(int argc, char* argv[])
{
	int steps;
	int radius;
	double heat;
	bool timing;

	int ret=1;

	GetOpt_pp ops(argc, argv);

	ops.exceptions(std::ios::failbit | std::ios::eofbit);

	try
	{
		ops >> Option('s', "steps", steps, 100)
		>> Option('r', "radius", radius, 5)
		>> Option('h', "heat", heat, 127.0)
		>> OptionPresent('t', "timing", timing);
	}
	catch(GetOpt::GetOptEx ex)
	{
		std::cerr << "Error in arguments" << std::endl;
		std::cerr << "Usage: "<<argv[0]<<" [options]"<<std::endl;
		std::cerr << "Options:"<<std::endl;
		std::cerr << "-s <NUM>: Set the number of simulation steps."<<std::endl;
		std::cerr << "-r <NUM>: Set the radius of the stimuli."<<std::endl;
		std::cerr << "-h <NUM>: Set the heat of the stimuli."<<std::endl;
		std::cerr << "-t      : Do a timing run. There won't be much output except the timing results."<<std::endl;
		return -1;
	}

		uint64_t t0;
		uint64_t t1;
		uint64_t t_ges=0;
		std::cout<<"Timing for N = "<<BOX_SIZE<<" with "<<steps<<" iterations"<<std::endl;

#define NUM_ITERATIONS 100
		for(int i=0; i < NUM_ITERATIONS; i++)
		{
			rdtsc(t0);
			Relaxation < BOX_SIZE > relax(radius, heat);
			for(int s=1; s <= steps; s++)
			{
				relax.iterate();
			}
			rdtsc(t1);
			t_ges += t1-t0;
			std::cout<<"This run took "<<(t1-t0)<<" clock cycles"<<std::endl;
		}
		std::cout<<"Timing: Used "<<t_ges<<" clock cycles total, "<<t_ges/NUM_ITERATIONS<<" cycles per run on average. (for N = "<<BOX_SIZE<<", "<<steps<<" iterations)"<<std::endl;

	return ret;
}
