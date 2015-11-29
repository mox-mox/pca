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

#ifdef VECTORISE
extern int num_threads;	// When using the vectorised version, the number of threads to use has to be a global variable.
#include <pthread.h>
#endif

template < int64_t N >
class Relaxation
{
	private:
		//{{{ Private Variables

		const double max_theta;
		const double phi;
		const int64_t I; // Number of Rows
		const int64_t J; // Number of Colums
		const int64_t R; // Radius of the initial stimulus
		const int64_t H; // Initial heat
		std::array < std::array < double, N >, N > grid;
		//}}}

		//{{{ Thread Control
#ifdef VECTORISE

		pthread_t* threads;
		double(Relaxation<N>::*current_op)(int64_t i, int64_t j);
		std::array < std::array < double, N >, N >* new_grid;
		pthread_barrier_t start_barrier;
		pthread_barrier_t stop_barrier;
		static void* thread_worker(void*);
#endif
		//}}}

		//{{{ Operations

		double init_value(int64_t i, int64_t j); // Set the start value for a particle
#ifdef DEBUG                                     // Calculate a new value for a particle
		double new_value(int64_t i, int64_t j) throw(std::logic_error);
#else   // The debug version does boundary checks.
		double new_value(int64_t i, int64_t j);
#endif
		//}}}

	public:
		//{{{Constructors

		Relaxation(double max_theta, double phi, int64_t I, int64_t J, double R, double H) throw(std::logic_error);
		Relaxation(double R, double H) throw(std::logic_error);
#ifdef VECTORISE
		~Relaxation();
#endif
		//}}}

		void iterate();
		void print_grid();
};





//{{{ Constructors

//{{{Relaxation()

//{{{
#ifdef VECTORISE
template < int64_t N >
Relaxation < N > ::Relaxation(double max_theta, double phi, int64_t I, int64_t J, double R, double H) throw(std::logic_error) : max_theta(max_theta), phi(phi), I(I), J(J), R(R), H(H), threads(new pthread_t[num_threads])
{
	//{{{ Sanity checks

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
	//}}}

    pthread_barrier_init (&start_barrier, NULL, num_threads+1);
    pthread_barrier_init (&stop_barrier, NULL, num_threads+1);
	struct Thread_args
	{
		Relaxation* me;
		int thread_number;
		Thread_args(){}
		Thread_args(Relaxation* m, int tn): me(m), thread_number(tn) {}
	};
	Thread_args* ta=new Thread_args[num_threads];
	for(int i=0; i<num_threads; i++)
	{
		new(&ta[i]) Thread_args(this, i); // Initialise each thread data structure
		pthread_create(&threads[i], nullptr, &thread_worker, reinterpret_cast<void*>(&ta[i]));
	}

	current_op=&Relaxation::init_value;
	new_grid=&grid;
    pthread_barrier_wait (&start_barrier);
	delete[] ta;
	for(int64_t j=0; j < N; j++) // ->
		grid[0][j] = grid[N-1][j] = 0;
    pthread_barrier_wait (&stop_barrier);
}
#else
//}}}
//{{{
template < int64_t N >
Relaxation < N > ::Relaxation(double max_theta, double phi, int64_t I, int64_t J, double R, double H) throw(std::logic_error) : max_theta(max_theta), phi(phi), I(I), J(J), R(R), H(H)
{
	//{{{ Sanity checks

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
	//}}}
	//TODO
	for(int64_t j=0; j < N; j++) // ->
		grid[0][j] = grid[N-1][j] = 0;
	for(int64_t i=1; i < N-1; i++)     // | parallel working phase
	{                                  // V
		grid[i][0] = 0;
		for(int64_t j=1; j < N-1; j++) // ->
		{
			grid[i][j] = init_value(i, j);
		}
		grid[i][N-1] = 0;
	}
}
#endif

template < int64_t N >
Relaxation < N > ::Relaxation(double R, double H) throw(std::logic_error) : Relaxation < N > ::Relaxation(127.0, 6.0/25.0, N/2, N/2, R, H) {}
//}}}
//{{{~Relaxation()

#ifdef VECTORISE
template<int64_t N>
Relaxation<N>::~Relaxation()
{
	current_op = nullptr;
	pthread_barrier_wait (&start_barrier);
	for(int i=0; i<num_threads; i++)
	{
		pthread_join(threads[i], nullptr);
	}
	delete[] threads;
}
#endif
//}}}
//}}}

//{{{ Thread_worker

#ifdef VECTORISE
template<int64_t N>
void* Relaxation<N>::thread_worker(void* args)
{
	if(!args) { std::cerr<<"Thread worker: No args supplied. Aborting..."<<std::endl; exit(-1); }

	struct Args { Relaxation* me; int thread_number; };
	Relaxation* me=reinterpret_cast<Args*>(args)->me;
	int num=reinterpret_cast<Args*>(args)->thread_number;

	for(;;)
	{
		pthread_barrier_wait (&me->start_barrier);
		if(!me->current_op) return nullptr;
		//if(!me->current_op) { std::string td="Thread worker "+std::to_string(num)+": returning.\n"; std::cout<<td<<std::flush; return nullptr; }
		//else { std::string td="Thread worker "+std::to_string(num)+": Starting parallel working phase.\n"; std::cout<<td<<std::flush; }
		for(int64_t i=num+1; i < N-1; i+=num_threads)     // | parallel working phase
		{                                  // V
			(*me->new_grid)[i][0] = 0;
			for(int64_t j=1; j < N-1; j++) // ->
			{
				(*me->new_grid)[i][j] = (me->*(me->current_op))(i, j);
			}
			(*me->new_grid)[i][N-1] = 0;
		}
		pthread_barrier_wait (&me->stop_barrier);
	}
	return nullptr;
}
#endif
//}}}

//{{{ Operations

//{{{ init_value(i, j)

template < int64_t N >
double Relaxation < N > ::init_value(int64_t i, int64_t j)
{
	//std::cout<<"I";
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
	if((i < 1) || (i > N-2) || (j < 1) || (j > N-2)) throw std::logic_error("new_value(): i="+std::to_string(i)+" or j="+std::to_string(j)+" are out of bounds.");
#else
double Relaxation < N > ::new_value(int64_t i, int64_t j)
{
#endif
	//std::cout<<"N";
	double retval = grid[i][j] + phi * (-4*grid[i][j] + grid[i+1][j] + grid[i-1][j] + grid[i][j+1] + grid[i][j-1]);
	// Implement saturation
	if( retval < 0) retval = 0;
	else if(retval > max_theta) retval = max_theta;

	return retval;
}
//}}}
//}}}

//{{{ iterate()

#ifdef VECTORISE
template < int64_t N >
void Relaxation < N > ::iterate()
{
	std::array < std::array < double, N >, N > t_plus;
	current_op=&Relaxation::new_value;
	new_grid=&t_plus;
    pthread_barrier_wait (&start_barrier);
	for(int64_t j=0; j < N; j++) // ->
		t_plus[0][j] = t_plus[N-1][j] = 0;
    pthread_barrier_wait (&stop_barrier);
	grid = std::move(t_plus);
}
#else
template < int64_t N >
void Relaxation < N > ::iterate()
{
	std::array < std::array < double, N >, N > t_plus;
	//TODO
	for(int64_t j=0; j < N; j++) // ->
		t_plus[0][j] = grid[N-1][j] = 0;
	for(int64_t i=1; i < N-1; i++)     // | parallel working phase
	{                                  // V
		t_plus[i][0] = 0;
		for(int64_t j=1; j < N-1; j++) // ->
		{
			t_plus[i][j] = new_value(i, j);
		}
		t_plus[i][N-1] = 0;
	}
	grid = std::move(t_plus);
}
#endif
//}}}

//{{{ print_grid()

template < int64_t N >
void Relaxation < N > ::print_grid()
{
	for(int64_t i=0; i < N; i++)     // |
	{                                // V
		for(int64_t j=0; j < N; j++) // ->
		{
			std::cout<<std::setprecision(4)<<std::setfill(' ')<<std::setw(9)<<grid[i][j]<<" ";
		}
		std::cout<<std::endl;
	}
}
//}}}

#endif /* RALAXATION_HPP */
