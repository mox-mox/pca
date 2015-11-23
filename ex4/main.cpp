//#include "matrix.hpp"
#include <iostream>
#include "getopt_pp.hpp"
#include <ctime>
#include "vectmath_vector.hpp"
#include "vectmath_matrix.hpp"







//#define size 2048
//#define size 5
//using namespace GetOpt;
//using namespace vectmath;
int main(int argc, char** argv)
{
	int dimensions;
	int threads;
	int seed;
	GetOpt::GetOpt_pp ops(argc, argv);

	ops.exceptions(std::ios::failbit | std::ios::eofbit);

	try
	{
		ops >> GetOpt::Option('d', "dimensions", dimensions, 4);
		ops >> GetOpt::Option('t', "threads", threads, 2);
		ops >> GetOpt::Option('s', "seed", seed, 0);
	}
	catch(GetOpt::GetOptEx ex)
	{
		std::cerr << "Error in arguments" << std::endl;
		std::cerr << "Usage: "<<argv[0]<<" [options]"<<std::endl;
		std::cerr << "Options:"<<std::endl;
		std::cerr << "-s|--size <NUM>: Set the matrix to be a NUMxNUM matrix."<<std::endl;
		std::cerr << "-t|--threads <NUM>: Set the number of threads that are started for the computation of the multiplication."<<std::endl;
	}

	std::cout<<"d was set to "<<dimensions<<std::endl;
	std::cout<<"t was set to "<<threads<<std::endl;
	std::cout<<"s was set to "<<seed<<std::endl;


	std::srand(ops >> GetOpt::OptionPresent('s', "seed") ? seed : std::time(0));



	{
		vectmath::Vector < double > v0;
		std::cout<<"v0: "<<v0<<"asfdadsf"<<std::endl;
	}
	vectmath::Vector<double> v1(dimensions);
	std::cout<<"v1: "<<v1<<std::endl;

	vectmath::Vector<double> v2 { 10, 9, 8, 7, 5, 4, 3, 2, 1, 0 };
	std::cout<<"v2: "<<v2<<std::endl;
	vectmath::Vector<double> v3(v2);
	std::cout<<"v3: "<<v2<<std::endl;
	v2+=v3;
	std::cout<<"v2: "<<v2<<std::endl;

	vectmath::Vector<double> v4 { 2, 1, 0 };
	std::cout<<"v4: "<<v4<<std::endl;
	vectmath::Vector<double> v5 { 4, 2, 5 };
	std::cout<<"v5: "<<v5<<std::endl;

	std::cout<<"v4*v5 = "<<v4*v5<<std::endl;


	{
		std::cout<<"fooooooooooooooooooooooooooo"<<std::endl;
		std::cout<<"fooooooooooooooooooooooooooo"<<std::endl;
		vectmath::Matrix < double > m0(20, 15);
		std::cout<<"fooooooooooooooooooooooooooo"<<std::endl;
		std::cout<<"fooooooooooooooooooooooooooo"<<std::endl;
		std::cout<<"m0: "<<m0<<std::endl;
		std::cout<<"fooooooooooooooooooooooooooo"<<std::endl;
		std::cout<<"fooooooooooooooooooooooooooo"<<std::endl;
	}

	{
		vectmath::Matrix<double> m1{ {1,2,3}, {4,5,6}, {7,8,9}};
		std::cout<<"m1: "<<m1<<std::endl;
	}

	{
		vectmath::Matrix < double > m2(20, 15);
		vectmath::Matrix < double > m3(m2);

		std::cout<<"m2: "<<m2<<std::endl;
		std::cout<<"m3: "<<m3<<std::endl;

		vectmath::Matrix < double > m4;
		std::cout<<"m4: "<<m4<<std::endl;
		m4=m2;
		//swap(m4,m2);
		std::cout<<"m4: "<<m4<<std::endl;

		std::cout<<"m4[3]: "<<m4[3]<<std::endl;
		std::cout<<"m4[3][4]: "<<m4[3][4]<<std::endl;

		m4+=m2;
		std::cout<<"m4+m2: "<<m4<<std::endl;
		m4-=m2;
		std::cout<<"m4: "<<m4<<std::endl;
	}

	{
		vectmath::Matrix<double> A{ {1,2,3}, {4,5,6}, {7,8,9}, {10,11,12}};
		vectmath::Vector<double> x{ 1, 2, 3};
		std::cout<<"A = "<<A<<", x = "<<x<<", Ax = "<<A*x<<"."<<std::endl;
		if(A*x != vectmath::Vector<double>{14,32,50,68})
			std::cout<<"Mist"<<std::endl;
		else
			std::cout<<"Passed"<<std::endl;





	}









//
//	Matrix<double> m1(size, true);
//	Matrix<double> m2(size, true);
//
//#ifdef OPTIMISE_TRANSPOSE
////#pragma message"Using transposed optimisation."
//	m2.transpose();
//#else
//#pragma message"NOT using transposed optimisation."
//#endif
//
//	clock_t t = clock();
//	Matrix<double> m3(m1*m2);
//	t = clock() - t;
//
//	//std::cout<<"Calculated "<<m1<<"*"<<m2<<"="<<m3<<" in "<<t<<" clicks ("<<static_cast < double > (t)/CLOCKS_PER_SEC<<"s)."<<std::endl;
//	std::cout<<"Calculated in "<<t<<" clicks ("<<static_cast < double > (t)/CLOCKS_PER_SEC<<"s)."<<std::endl;
//

		return 0;
	}
