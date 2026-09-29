// #include <mpi.h>
// #include <iostream>

// int main(int argc, char* argv[])
// {
//     MPI_Init(&argc, &argv);

//     int rank;
//     int size;

//     MPI_Comm_rank(
//         MPI_COMM_WORLD,
//         &rank
//     );

//     MPI_Comm_size(
//         MPI_COMM_WORLD,
//         &size
//     );


//     std::cout 
//         << "Hello from process "
//         << rank
//         << " of "
//         << size
//         << std::endl;


//     MPI_Finalize();

//     return 0;
// }

#include<iostream>
#include<mpi.h>

int main(int argc, char** argv){
    MPI_Init(&argc,&argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    int data[5] = {10,20,30,40,50};
    if(rank == 0){
        MPI_Send(
            data,5,MPI_INT,1,0,MPI_COMM_WORLD
        );

    }
}