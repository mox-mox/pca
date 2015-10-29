// Compile with g++ -Wall -Wextra -Wpedantic -std=c++14
#include <iostream>
#include <iomanip>
#include <cmath>


//#define OPTIMISE CACHE
//#define OPTIMISE RUNTIME


#define OPTIMISE CACHE

#if !defined OPTIMISE || (OPTIMISE != CACHE) && (OPTIMISE != RUNTIME)
	#error "Please define OPTIMISE to CACHE or RUNTIME."
#endif



template < int64_t N=1024 >
                      class Relaxation
{
	private:
		const double max_theta;
		double grid[N][N];
	public:
		Relaxation(double max_theta, int64_t I, int64_t J, double R, double H) throw(std::logic_error) : max_theta(max_theta)
		{
			if(R < 1) throw std::logic_error("The radius \"R\" has to be at least 1.");
			if((I < 1) || (I >= N-1) || (J < 1) || (J >= N-1)) throw std::logic_error("The focus of the stimulus has to be within the inner part of the grid (not on the edge).");
			if((I-R < 0) || (I+R > N) || (J-R < 0) || (J+R > N)) throw std::logic_error("The stimulated area exceeds the allowed area (the inner part of the grid, that is not the edge).");
			if( H > max_theta) throw std::logic_error("The stimulus value \"H\" must be smaller than the maximum stimulus \"max_theta\".");

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
			for(int64_t j=1; j < N-1; j++)		// |
			{									// V
				for(int64_t i=1; i < N-1; i++)	// ->
				{
					int64_t i_dist = I-i;
					int64_t j_dist = J-j;
					int64_t dist = std::sqrt((i_dist*i_dist)+(j_dist*j_dist));
					grid[i][j] = dist < R ? H : 0;
				}
			}
#elif OPTIMISE == CACHE	// When optimising for cache coherence, accept a branch within the loops but fetch the data only once
			// Initialise all of the array in one run.
			for(int64_t j=0; j < N; j++)		// |
			{									// V
				for(int64_t i=0; i < N; i++)	// ->
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
					grid[i][j] = dist < R ? H : 0;
				}
			}
#else
	#error "Please define OPTIMISE to CACHE or RUNTIME."
#endif
		}

		void print_grid()
		{
			for(int64_t j=0; j < N; j++)		// |
			{									// V
				for(int64_t i=0; i < N; i++)	// ->
				{
					std::cout<<std::setprecision(4)<<std::setfill(' ')<<std::setw(5)<<grid[i][j]<<" ";
				}
				std::cout<<std::endl;
			}
		}
};



int main()
{
	//Relaxation < 1024 > relax(127.0, 512, 512, 20, 100);
	Relaxation < 20 > relax(127.0, 10, 10, 7, 100);
	relax.print_grid();
	return 0;
}
