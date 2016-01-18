// SOURCE: Second example from http://www.umbc.edu/hpcf/hpc-archived/5245461.php

// Include the MPI version 2 C++ bindings:
#include <mpi.h>
#include <iostream>
#include <string.h>

//using namespace std;

int main(int argc, char* argv[])
{
	// Initialize the MPI library:
	MPI::Init(argc, argv);


	if( MPI::COMM_WORLD.Get_size() < 2 )
	{
		MPI::Finalize();
		std::cout<<"Need at least two processes to pass a message."<<std::endl;
		return -1;
	}


	switch( MPI::COMM_WORLD.Get_rank())
	{
		case 0:
		{
			double t1, t2;
			t1 = MPI::Wtime();	// Measure start time.
			int monkey = 0xaffe;
			//std::cout<<"monkey: "<<std::hex<<monkey<<std::endl;
			MPI::COMM_WORLD.Send(&monkey, 1, MPI_INT, 1, 123);		// Send a message...
			MPI::COMM_WORLD.Recv(&monkey, 1, MPI_INT, 1, 123);		// ... and receive it.
			//std::cout<<"monkey: "<<std::hex<<monkey<<std::endl;
			t2 = MPI::Wtime();	// Measure end time
			if( monkey != ~(0xaffe) )
			{
				std::cout<<"There was a communication error."<<std::endl;
				MPI::Finalize();
				return -2;
			}
			std::cout<<"Half round trip latency is "<<(t2-t1)/2<<" seconds."<<std::endl;
		}
		break;
		case 1:
		{
			int ape = 0;
			MPI::COMM_WORLD.Recv(&ape, 1, MPI_INT, 0, 123);
			ape = ~ape;		// Negate the received number, so we can check, that sending and receiving actually worked.
			MPI::COMM_WORLD.Send(&ape, 1, MPI_INT, 0, 123);
		}
		break;
		default:
			std::cout<<"Other process."<<std::endl;
	}


	// Tell the MPI library to release all resources it is using:
	MPI::Finalize();
	return 0;
}
