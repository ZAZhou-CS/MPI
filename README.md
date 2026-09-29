修改CMakelists.txt，添加自己要编译的文件
cd ~/Research/Gitcodes/StudyCode/MPI

cmake -S . -B build
cmake --build build

mpirun -np 2 ./build/send
MPI的这个配置我还是有点蒙，
不过我现在算是把工作流的搭建弄清楚了
Project
│
├── src          ← 你写的代码
├── build        ← 编译产生的东西
├── CMakeLists.txt ← 告诉电脑如何编译
├── README.md    ← 项目说明
└── notes        ← 学习笔记


