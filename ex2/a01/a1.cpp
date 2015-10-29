// Compile with g++ -Wall -Wextra -Wpedantic -std=c++14
#include "a1.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>


//#define OPTIMISE CACHE
//#define OPTIMISE RUNTIME


#define OPTIMISE CACHE

#if !defined OPTIMISE || (OPTIMISE != CACHE) && (OPTIMISE != RUNTIME)
	#error "Please define OPTIMISE to CACHE or RUNTIME."
#endif


//{{{Relaxation<N>()

template < int64_t N >
Relaxation < N > ::Relaxation(double max_theta, double phi, int64_t I, int64_t J, double R, double H) throw(std::logic_error) : max_theta(max_theta), phi(phi), I(I), J(J), R(R), H(H)
{
	if(R < 1) throw std::logic_error("The radius \"R\" has to be at least 1.");
	if((I < 1) || (I >= N-1) || (J < 1) || (J >= N-1))
	{
		throw std::logic_error("The focus of the stimulus has to be within the inner part of the grid (not on the edge).");
	}
	if((I-R < 0) || (I+R > N) || (J-R < 0) || (J+R > N))
	{
		throw std::logic_error("The stimulated area exceeds the allowed area (the inner part of the grid, that is not the edge).");
	}
	if( H > max_theta) throw std::logic_error("The stimulus value \"H\" must be smaller than the maximum stimulus \"max_theta\".");

	grid = new std::array < std::array < double, N >, N >;


	fill_grid(&Relaxation::init_value, *grid);
}
//}}}



//template < int64_t N >
//Relaxation < N > ::Relaxation(double R, double H) : Relaxation < N > ::Relaxation(127.0, 6.0/25.0, N/2, N/2, R, H) {};

//template < int64_t N >
//Relaxation < N > ::Relaxation(double R, double H) : Relaxation < N > ::Relaxation((127.0), (6.0/25.0), (N/2), (N/2), R, H) {};

//{{{~Relaxation<N>

template < int64_t N >
Relaxation < N > ::~Relaxation()
{
	delete grid;
}
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
	double retval = (*grid)[i][j] + phi * (-4*(*grid)[i][j] + (*grid)[i+1][j] + (*grid)[i-1][j] + (*grid)[i][j+1] + (*grid)[i][j-1]);
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
	std::array < std::array < double, N >, N > *t_plus = new std::array < std::array < double, N >, N >;
	fill_grid(&Relaxation::new_value, *t_plus);
	delete grid;
	grid = t_plus;
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
			std::cout<<std::setprecision(4)<<std::setfill(' ')<<std::setw(9)<<(*grid)[i][j]<<" ";
		}
		std::cout<<std::endl;
	}
}
//}}}



int main()
{
	Relaxation < 30 > relax(127.0, 6.0/25.0, 10, 10, 7, 100);
	relax.print_grid();
	for( ; ; )
	{
		std::cout<<"Press Enter to do one more iteration, Ctrl+C to exit: "<<std::endl;
		std::cin.get();
		relax.iterate();
		relax.print_grid();
	}
	return 0;
}
