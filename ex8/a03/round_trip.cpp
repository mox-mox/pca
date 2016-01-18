// Include the MPI version 2 C++ bindings:
#include <mpi.h>
#include <iostream>
#include <string.h>
#include "getopt_pp.hpp"


//using namespace std;

int main(int argc, char* argv[])
{
	//{{{ Argument handling

	int size;
	GetOpt::GetOpt_pp ops(argc, argv);

	ops.exceptions(std::ios::failbit|std::ios::eofbit);
	try
	{
		ops>>GetOpt::Option('s', "size", size, 10);
	}
	catch(GetOpt::GetOptEx ex)
	{
		std::cerr<<"Error in arguments"<<std::endl;
		std::cerr<<"Usage: "<<argv[0]<<" [options]"<<std::endl;
		std::cerr<<"Options:"<<std::endl;
		std::cerr<<"-s|--size   <NUM>: Set the message size for the round trip latency test. Use value '0' for a self test."<<std::endl;

		return -1;
	}
	//}}}

	// Initialize the MPI library:
	MPI::Init(argc, argv);


	if( MPI::COMM_WORLD.Get_size() < 2 )
	{
		MPI::Finalize();
		std::cout<<"Need at least two processes to pass a message."<<std::endl;
		return -1;
	}
	char name[MPI_MAX_PROCESSOR_NAME];
	int len;
	memset(name, 0, MPI_MAX_PROCESSOR_NAME);
	MPI::Get_processor_name(name, len);
	memset(name+len, 0, MPI_MAX_PROCESSOR_NAME-len);
	std::cout<<"name: "<<name<<std::endl;


	switch( MPI::COMM_WORLD.Get_rank())
	{
		case 0:
			if( size == 0 )	// Testmode
			{
				double t1, t2;
				t1 = MPI::Wtime();	// Measure start time.
				int monkey = 0xaffe;
				//std::cout<<"monkey: "<<std::hex<<monkey<<std::endl;
				MPI::COMM_WORLD.Send(&monkey, 1, MPI_INT, 1, 123);	// Send a message...
				MPI::COMM_WORLD.Recv(&monkey, 1, MPI_INT, 1, 123);	// ... and receive it.
				//std::cout<<"monkey: "<<std::hex<<monkey<<std::endl;
				t2 = MPI::Wtime();	// Measure end time
				if( monkey != ~(0xaffe))
				{
					std::cout<<"There was a communication error."<<std::endl;
					MPI::Finalize();
					return -2;
				}
				std::cout<<"Commonication works. The half round trip latency for an integer is "<<(t2-t1)/2<<" seconds."<<std::endl;
			}
			else// Normal mode
			{
				std::cout<<"Testing communication delay for message size of "<<size<<" bytes."<<std::endl;
				double t1, t2;
				t1 = MPI::Wtime();	// Measure start time.
				char* message = new char[size];
				MPI::COMM_WORLD.Send(message, size, MPI_CHAR, 1, 123);	// Send a message...
				MPI::COMM_WORLD.Recv(message, size, MPI_CHAR, 1, 123);	// ... and receive it.
				t2 = MPI::Wtime();	// Measure end time
				std::cout<<"Half round trip latency is "<<(t2-t1)/2<<" seconds."<<std::endl;
			}
			break;
		case 1:
			if( size == 0 )	// Testmode
			{
				int ape = 0;
				MPI::COMM_WORLD.Recv(&ape, 1, MPI_INT, 0, 123);
				ape = ~ape;	// Negate the received number, so we can check, that sending and receiving actually worked.
				MPI::COMM_WORLD.Send(&ape, 1, MPI_INT, 0, 123);
			}
			else// Normal benchmark mode.
			{
				char* message = new char[size];
				MPI::COMM_WORLD.Recv(message, size, MPI_CHAR, 0, 123);
				MPI::COMM_WORLD.Send(message, size, MPI_CHAR, 0, 123);
			}
			break;
		default:
			std::cout<<"Other process."<<std::endl;
	}


	// Tell the MPI library to release all resources it is using:
	MPI::Finalize();
	return 0;
}
