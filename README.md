修改CMakelists.txt，添加自己要编译的文件
cd ~/Research/Gitcodes/StudyCode/MPI

cmake -S . -B build
cmake --build build

mpirun -np 2 ./build/send



