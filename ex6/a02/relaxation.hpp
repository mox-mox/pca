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
//extern int num_threads;	                                                        // Removed this as unnecessary for ex6
//#include <pthread.h>                                                              // Removed this as unnecessary for ex6
#include <omp.h>                                                                    // New addition for ex6
#endif

template < typename T, int64_t size >
class Relaxation
{
	private:
		//{{{ Private Variables

		const T max_theta;
		const T phi;
		const int64_t I; // Number of Rows
		const int64_t J; // Number of Colums
		const int64_t R; // Radius of the initial stimulus
		const int64_t H; // Initial heat
		std::array < std::array < T, size >, size >* grid;
		//}}}

		//{{{ Operations

		T init_value(int64_t i, int64_t j); // Set the start value for a particle
#ifdef DEBUG                                     // Calculate a new value for a particle
		T new_value(int64_t i, int64_t j) throw(std::logic_error);
#else   // The debug version does boundary checks.
		T new_value(int64_t i, int64_t j);
#endif
		//}}}

	public:
		//{{{Constructors

		Relaxation(T max_theta, T phi, int64_t I, int64_t J, T R, T H) throw(std::logic_error);
		Relaxation(T R, T H) throw(std::logic_error);
		~Relaxation();
		//}}}

		void iterate();
		void print_grid();
};

//{{{ Constructors

//{{{Relaxation()

//{{{
template < typename T, int64_t size >
Relaxation < T, size > ::Relaxation(T max_theta, T phi, int64_t I, int64_t J, T R, T H) throw(std::logic_error) : max_theta(max_theta), phi(phi), I(I), J(J), R(R), H(H),  grid(new std::array < std::array < T, size >, size >)
{
	//{{{ Sanity checks

	if(R < 1)
	{
		throw std::logic_error("The radius \"R\" has to be at least 1.");
	}
	if((I < 1) || (I >= size-1) || (J < 1) || (J >= size-1))
	{
		throw std::logic_error("The focus of the stimulus has to be within the inner part of the grid (not on the edge).");
	}
	if((I-R < 0) || (I+R > size) || (J-R < 0) || (J+R > size))
	{
		throw std::logic_error("The stimulated area exceeds the allowed area (the inner part of the grid, that is not the edge).");
	}
	if( H > max_theta)
	{
		throw std::logic_error("The stimulus value \"H\" must be smaller than the maximum stimulus \"max_theta\".");
	}
	//}}}

	for(int64_t j=0; j < size; j++) // ->
		(*grid)[0][j] = (*grid)[size-1][j] = 0;
#pragma omp parallel for
	for(int64_t i=1; i < size-1; i++)     // | parallel working phase
	{                                     // V
		(*grid)[i][0] = 0;
		for(int64_t j=1; j < size-1; j++) // ->
		{
			(*grid)[i][j] = init_value(i, j);
		}
		(*grid)[i][size-1] = 0;
	}
}
//}}}

template < typename T, int64_t size >
Relaxation < T, size > ::Relaxation(T R, T H) throw(std::logic_error) : Relaxation < T, size > ::Relaxation(127.0, 6.0/25.0, size/2, size/2, R, H) {}
//}}}
//{{{~Relaxation()

template<typename T,int64_t size>
Relaxation<T,size>::~Relaxation()
{
//#ifdef VECTORISE
//	current_op = nullptr;
//	pthread_barrier_wait (&start_barrier);
//	for(int i=0; i<num_threads; i++)
//	{
//		pthread_join(threads[i], nullptr);
//	}
//	delete[] threads;
//#endif
	delete[] grid;
}
//}}}
//}}}
//
////{{{ Thread_worker
//
//#ifdef VECTORISE
//template<int64_t size>
//void* Relaxation<size>::thread_worker(void* args)
//{
//	if(!args) { std::cerr<<"Thread worker: No args supplied. Aborting..."<<std::endl; exit(-1); }
//
//	struct Args { Relaxation* me; int thread_number; };
//	Relaxation* me=reinterpret_cast<Args*>(args)->me;
//	int num=reinterpret_cast<Args*>(args)->thread_number;
//
//	for(;;)
//	{
//		pthread_barrier_wait (&me->start_barrier);
//		if(!me->current_op) return nullptr;
//		//if(!me->current_op) { std::string td="Thread worker "+std::to_string(num)+": returning.\n"; std::cout<<td<<std::flush; return nullptr; }
//		//else { std::string td="Thread worker "+std::to_string(num)+": Starting parallel working phase.\n"; std::cout<<td<<std::flush; }
//		for(int64_t i=num+1; i < size-1; i+=num_threads)     // | parallel working phase
//		{                                  // V
//			(*me->new_grid)[i][0] = 0;
//			for(int64_t j=1; j < size-1; j++) // ->
//			{
//				(*me->new_grid)[i][j] = (me->*(me->current_op))(i, j);
//			}
//			(*me->new_grid)[i][size-1] = 0;
//		}
//		pthread_barrier_wait (&me->stop_barrier);
//	}
//	return nullptr;
//}
//#endif
////}}}
//
//{{{ Operations

//{{{ init_value(i, j)

template < typename T, int64_t size >
T Relaxation < T, size > ::init_value(int64_t i, int64_t j)
{
	//std::cout<<"I";
	T dist;
	int64_t i_dist = I-i;
	int64_t j_dist = J-j;
	dist = std::sqrt((i_dist*i_dist)+(j_dist*j_dist));
	return dist < R ? H : 0;
}
//}}}

//{{{ new_value(i, j)

template < typename T, int64_t size >
#ifdef DEBUG
T Relaxation < T, size > ::new_value(int64_t i, int64_t j) throw(std::logic_error)
{
	if((i < 1) || (i > size-2) || (j < 1) || (j > size-2)) throw std::logic_error("new_value(): i="+std::to_string(i)+" or j="+std::to_string(j)+" are out of bounds.");
#else
T Relaxation < T, size > ::new_value(int64_t i, int64_t j)
{
#endif
	T retval = (*grid)[i][j] + phi * (-4*(*grid)[i][j] + (*grid)[i+1][j] + (*grid)[i-1][j] + (*grid)[i][j+1] + (*grid)[i][j-1]);
	// Implement saturation
	if( retval < 0) retval = 0;
	else if(retval > max_theta) retval = max_theta;

	return retval;
}
//}}}
//}}}

//{{{ iterate()

//#ifdef VECTORISE
//template < typename T, int64_t size >
//void Relaxation < T, size > ::iterate()
//{
//	//std::array < std::array < T, size >, size > t_plus;
//	new_grid = new std::array < std::array < T, size >, size >;
//	current_op=&Relaxation::new_value;
//    pthread_barrier_wait (&start_barrier);
//	for(int64_t j=0; j < size; j++) // ->
//		(*new_grid)[0][j] = (*new_grid)[size-1][j] = 0;
//    pthread_barrier_wait (&stop_barrier);
//	delete[] grid;
//	grid = new_grid;
//}
//#else
template < typename T, int64_t size >
void Relaxation < T, size > ::iterate()
{
	std::array < std::array < T, size >, size >* t_plus = new std::array < std::array < T, size >, size >;
	for(int64_t j=0; j < size; j++) // ->
		(*t_plus)[0][j] = (*grid)[size-1][j] = 0;
#pragma omp parallel for
	for(int64_t i=1; i < size-1; i++)     // | parallel working phase
	{                                  // V
		(*t_plus)[i][0] = 0;
		for(int64_t j=1; j < size-1; j++) // ->
		{
			(*t_plus)[i][j] = new_value(i, j);
		}
		(*t_plus)[i][size-1] = 0;
	}
	delete[] grid;
	grid = t_plus;
	//(*grid) = std::move((*t_plus));
}
//#endif
//}}}

//{{{ print_grid()

template < typename T, int64_t size >
void Relaxation < T, size > ::print_grid()
{
	for(int64_t i=0; i < size; i++)     // |
	{                                // V
		for(int64_t j=0; j < size; j++) // ->
		{
			std::cout<<std::setprecision(4)<<std::setfill(' ')<<std::setw(9)<<(*grid)[i][j]<<" ";
		}
		std::cout<<std::endl;
	}
}
//}}}

#endif /* RALAXATION_HPP */
