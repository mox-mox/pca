#ifndef RALAXATION_HPP
#define RALAXATION_HPP

#include <cstdint>
#include <array>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <utility>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include "utils.h"

#define STRINGIFY(s) XSTRINGIFY(s)
#define XSTRINGIFY(s) #s



//#define OPTIMISE RUNTIME
//
//#if !defined OPTIMISE || (OPTIMISE != CACHE) && (OPTIMISE != RUNTIME)
//	#error "Please define OPTIMISE to CACHE or RUNTIME."
//#endif

template < int64_t N >
class Relaxation
{
	private:
		const double max_theta;
		const double phi;
		const int64_t I;
		const int64_t J;
		const int64_t R;
		const int64_t H;
		std::array < std::array < double, N >, N > grid;
	public:
		Relaxation(double max_theta, double phi, int64_t I, int64_t J, double R, double H) throw(std::logic_error);
		Relaxation(double R, double H) throw(std::logic_error);


		void fill_grid(double(Relaxation < N > ::*grid_point_value_function)(int64_t i, int64_t j), std::array < std::array < double, N >, N >& grid);
		inline void fill_line(double(Relaxation < N > ::*grid_point_value_function)(int64_t i, int64_t j), std::array < double, N >& grid, int64_t i);


#ifdef DEBUG
		double new_value(int64_t i, int64_t j) throw(std::logic_error);
#else
		double new_value(int64_t i, int64_t j);
#endif

		double init_value(int64_t i, int64_t j);

		void iterate();

		void print_grid();

		void export_data(std::ofstream& myfile);
};
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
	for(int64_t j=1; j < N-1; j++) // ->
		grid[0][j] = grid[N-1][j] = 0;
	for(int64_t i=1; i < N-1; i++)     // |
	{                                  // V
		grid[i][0] = 0;
		for(int64_t j=1; j < N-1; j++) // ->
		{
			grid[i][j] = (this->*grid_point_value_function)(i, j);
		}
		grid[i][N-1] = 0;
		//fill_line(&Relaxation::new_value, grid[i], i);
	}
}
//}}}

//{{{ fill_line(grid_point_value_function, &grid)

template < int64_t N >
void Relaxation < N > ::fill_line(double(Relaxation < N > ::*grid_point_value_function)(int64_t i, int64_t j), std::array < double, N > &grid, int64_t i)
{
	grid[0] = 0;
	for(int64_t j=1; j < N-1; j++) // ->
	{
		grid[j] = (this->*grid_point_value_function)(i, j);
	}
	grid[N-1] = 0;
}
//}}}

//{{{ init_value(i, j)

template < int64_t N >
double Relaxation < N > ::init_value(int64_t i, int64_t j)
{
	double dist;
	int64_t i_dist = I-i;
	int64_t j_dist = J-j;
	dist = std::sqrt((i_dist*i_dist)+(j_dist*j_dist));
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
#endif /* RALAXATION_HPP */
