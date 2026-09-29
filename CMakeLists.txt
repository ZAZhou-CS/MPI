cmake_minimum_required(VERSION 3.16)

project(MPI_Project
        LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(MPI REQUIRED)

#hello_mpi
add_executable(
    hello_mpi
    src/hello_mpi.cpp
)


target_link_libraries(
    hello_mpi
    MPI::MPI_CXX
)

#RingAllreduce 
add_executable(
    RingAllreduce                     #程序名字
    src/RingAllreduce.cpp             #源代码
)


target_link_libraries(
    RingAllreduce 
    MPI::MPI_CXX
)


#RingAllreduce2 
add_executable(
    RingAllreduce2                     #程序名字
    src/RingAllreduce2.cpp             #源代码
)


target_link_libraries(
    RingAllreduce2 
    MPI::MPI_CXX
)

#RingAllreduce3 

