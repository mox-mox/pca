// Compile with g++ a1.cpp -std=c++11 -Wall -Wextra -Wpedantic -Werror -Wfatal-errors && ./a.out
#include "a1.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <utility>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include "utils.h"




//{{{Relaxation()

template < int64_t N >
Relaxation < N > ::Relaxation(double max_theta, double phi, int64_t I, int64_t J, double R, double H) throw(std::logic_error) : max_theta(max_theta), phi(phi), I(I), J(J), R(R), H(H)
{
	if(R < 1)
	{
		throw std::logic_error("The radius \"R\" has to be at least 1.");
	}
	if((I < 1) || (I >= N-1) || (J < 1) || (J >= N-1))
	{
		throw std::logic_error("The focus of the stimulus has to be within the inner part of the grid (not on the edge).");
	}
	if((I-R < 0) || (I+R > N) || (J-R < 0) || (J+R > N))
	{
		throw std::logic_error("The stimulated area exceeds the allowed area (the inner part of the grid, that is not the edge).");
	}
	if( H > max_theta)
	{
		throw std::logic_error("The stimulus value \"H\" must be smaller than the maximum stimulus \"max_theta\".");
	}


	fill_grid(&Relaxation::init_value, grid);
}

template < int64_t N >
Relaxation < N > ::Relaxation(double R, double H) throw(std::logic_error) : Relaxation < N > ::Relaxation(127.0, 6.0/25.0, N/2, N/2, R, H) {}
//}}}

//{{{ fill_grid(grid_point_value_function, &grid)

template < int64_t N >
void Relaxation < N > ::fill_grid(double(Relaxation < N > ::*grid_point_value_function)(int64_t i, int64_t j), std::array < std::array < double, N >, N > &grid)
{
#if OPTIMISE == RUNTIME	// When optimising the "runtime" use logic to reduce number of operations.
	// Initialise the edges (which are always 0) before the rest.
	for(int64_t i=0; i < N; i++)
	{
		grid[i][0] = 0;
	}
	for(int64_t i=0; i < N; i++)
	{
		grid[i][N-1] = 0;
	}
	for(int64_t j=0; j < N; j++)
	{
		grid[0][j] = 0;
	}
	for(int64_t j=0; j < N; j++)
	{
		grid[N-1][j] = 0;
	}
	// Initialise only the inner part of the grid.
	for(int64_t j=1; j < N-1; j++)				// |
	{											// V
		for(int64_t i=1; i < N-1; i++)			// ->
		{
			grid[i][j] = (this->*grid_point_value_function)(i, j);
			//int64_t i_dist = I-i;
			//int64_t j_dist = J-j;
			//int64_t dist = std::sqrt((i_dist*i_dist)+(j_dist*j_dist));
			//grid[i][j] = dist < R ? H : 0;
		}
	}
#elif OPTIMISE == CACHE	// When optimising for cache coherence, accept a branch within the loops but fetch the data only once
	// Initialise all of the array in one run.
	for(int64_t j=0; j < N; j++)				// |
	{											// V
		for(int64_t i=0; i < N; i++)			// ->
		{
			grid[i][j] = (this->*grid_point_value_function)(i, j);
		}
	}
#else
	#error "Please define OPTIMISE to CACHE or RUNTIME."
#endif
}
//}}}

//{{{ init_value(i, j)

template < int64_t N >
double Relaxation < N > ::init_value(int64_t i, int64_t j)
{
	double dist;
	// For N>>0 it is probably faster, to avoid the branch
	//if(!j || j==N-1 || !i || i==N-1)
	//	dist=0;
	//else {
	int64_t i_dist = I-i;
	int64_t j_dist = J-j;
	dist = std::sqrt((i_dist*i_dist)+(j_dist*j_dist));
	//}
	return dist < R ? H : 0;
}
//}}}

//{{{ new_value(i, j)

template < int64_t N >
#ifdef DEBUG
double Relaxation < N > ::new_value(int64_t i, int64_t j) throw(std::logic_error)
{
	if((i < 1) || (i > N-2) || (j < 1) || (j > N-2)) throw std::logic_error("new_value(): i or j are out of bounds.");
#else
double Relaxation < N > ::new_value(int64_t i, int64_t j)
{
#endif
	double retval = grid[i][j] + phi * (-4*grid[i][j] + grid[i+1][j] + grid[i-1][j] + grid[i][j+1] + grid[i][j-1]);
	// Implement saturation
	if( retval < 0) retval = 0;
	else if(retval > max_theta) retval = max_theta;

	return retval;
}
//}}}

//{{{ iterate()

template < int64_t N >
void Relaxation < N > ::iterate()
{
	std::array < std::array < double, N >, N > t_plus;
	fill_grid(&Relaxation::new_value, t_plus);
	grid = std::move(t_plus);
}
//}}}

//{{{ print_grid()

template < int64_t N >
void Relaxation < N > ::print_grid()
{
	for(int64_t j=0; j < N; j++)				// |
	{											// V
		for(int64_t i=0; i < N; i++)			// ->
		{
			std::cout<<std::setprecision(4)<<std::setfill(' ')<<std::setw(9)<<grid[i][j]<<" ";
		}
		std::cout<<std::endl;
	}
}
//}}}

//{{{ export_data()

template < int64_t N >
void Relaxation < N > ::export_data(std::ofstream& myfile)
{
	for(int64_t j=0; j < N; j++)				// |
	{											// V
		for(int64_t i=0; i < N; i++)			// ->
		{
			myfile << std::setfill(' ') << std::setw(16) << i << " "
			       << std::setfill(' ') << std::setw(16) << j << " "
			       << std::setfill(' ') << std::setw(16) << grid[i][j] << std::endl;
		}
	}
}
//}}}




#ifndef BOX_SIZE
	#error "Please define BOX_SIZE to set the size of the grid."
#endif

#include "getopt_pp.h"

using namespace GetOpt;

int main(int argc, char* argv[])
{
	int steps;
	int radius;
	double heat;
	bool interactive;
	bool timing;
	bool export_data;
	bool get_N;

	int ret;

	GetOpt_pp ops(argc, argv);

	ops.exceptions(std::ios::failbit | std::ios::eofbit);

	try
	{
		ops >> Option('s', "steps", steps, 100)
		>> Option('r', "radius", radius, 5)
		>> Option('h', "heat", heat, 127.0)
		>> OptionPresent('i', "interactive", interactive)
		>> OptionPresent('t', "timing", timing)
		>> OptionPresent('e', "export", export_data)
		>> OptionPresent('g', "get_N", get_N);

		ret = 0;
	}
	catch(GetOpt::GetOptEx ex)
	{
		std::cerr << "Error in arguments" << std::endl;
		std::cerr << "Usage: "<<argv[0]<<" [options]"<<std::endl;
		std::cerr << "Options:"<<std::endl;
		// War diese einfache Dokumentation soo viel verlangt, Dennis?
		std::cerr << "-s <NUM>: Set the number of simulation steps."<<std::endl;
		std::cerr << "-r <NUM>: Set the radius of the stimuli."<<std::endl;
		std::cerr << "-h <NUM>: Set the heat of the stimuli."<<std::endl;
		std::cerr << "-i      : Interactive mode: Press enter and watch the simulation in the console."<<std::endl;
		std::cerr << "-t      : Do a timing run. There won't be much output except the timing results."<<std::endl;
		std::cerr << "-e      : Export data to files in ./data. Used by the visualisation target in the makefile."<<std::endl;
		std::cerr << "-g      : Return the \"N\" parameter and exit. This is used to know if the file has to be recompiled."<<std::endl;
		ret = -1;
	}

	if(get_N)
	{
		return BOX_SIZE;
	}

	if(timing && interactive)
	{
		std::cerr<<"Trust me, there is no sense in doing interactive timing analysis.		...Aborting"<<std::endl;
		return 1;
	}





	if(interactive)
	{
		Relaxation < BOX_SIZE > relax(radius, heat);
		relax.print_grid();
		for( ; ; )
		{
			std::cout<<"Press Enter to do one more iteration, Ctrl+C to exit: "<<std::endl;
			std::cin.get();
			relax.iterate();
			relax.print_grid();
		}
	}
	else if(export_data)
	{
		Relaxation < BOX_SIZE > relax(radius, heat);
		std::cout<<"Writing raw relaxation data to files..."<<std::endl;
		int length = std::to_string(steps).length();
		std::string path="./data/";
		struct stat info;
		if(stat(path.c_str(), &info))
		{
			std::cout<<"creating folder"<<std::endl;
			int temp = system("mkdir data");
			temp=temp;
		}

		std::ofstream data_output;
		std::ostringstream out;
		out << std::setfill('0') << std::setw(length) << 0;
		data_output.open("data/iteration_" + out.str() + ".dat");
		relax.export_data(data_output);
		data_output.close();
		for(int s=1; s <= steps; s++)
		{
			relax.iterate();
			std::ostringstream out;
			out << std::setfill('0') << std::setw(length) << s;
			data_output.open("data/iteration_" + out.str() + ".dat");
			relax.export_data(data_output);
			data_output.close();
			std::cout<<out.str()<<"	"<<std::flush;
		}
		std::cout<<std::endl<<std::endl;
	}
	else if(timing)
	{
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
	}

	return ret;
}
