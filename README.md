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


A. Allreduce：自己实现 3 种算法
    1.Ring Allreduce
    2.Tree-based Allreduce
        Binary tree
        K-ary tree
3.Rabenseifner's Allreduce
B. All-to-all：自己实现 2 种算法
    Spread-out All-to-all
    Bruck All-to-all
C. 阅读开源 MPI 实现
    MPICH
    Open MPI

自己用MPI的基础通信操作，把这个算法搭建出来

需要掌握的：
1.算法实现的细节，思路
2.rank和rank之间如何通信
3.每次到底发送什么数据
4.用了什么MPI primitive

需要做的事情是：
先自己实现algorithm
再自己理解algorithm
自己实现
验证
看MPICH/Open MPI
比较我的实现和工业级MPI实现有什么区别





Algorithm
       ↓
自己画 communication pattern
       ↓
自己设计数据结构
       ↓
自己决定 Send / Recv / Isend / Irecv
       ↓
自己写 MPI
       ↓
correctness
       ↓
performance
       ↓
GPU version


CUDA homework 已经完成了它作为基础训练的作用。现在开始进入“我给你一个算法，你自己把它实现出来”的阶段

7：00
7：00-8：00 安顿一天
8：00-12：30学习
12：30-13：00 午睡休息
13：00-17：00 学习
17：00-19：00 跑步机上走路记单词，放弃扇贝，扇贝不适合我，先不要一上来就6/km，先走起来，哪怕只是2km/h
19：00-20：00 练新概念new1*3,new2*1
20:00-21：00 TED练习
21：00-22：00 PTE DI练习
22：00-23：00 洗澡洗衣睡觉
如果早上能起来的话
5：30-7：00 去附近公园转转，或者跳跳拉拉操，都好

重新分配时间
英语学习 4h，一集英文单词看10遍，跟读，87集，每天背一集内容 大约1.5h 21-22.5
            雅思口语课  1h 20-21
            回家 0.5h 19.5-20
            新概念口语练习背诵＋练习 1h 17.5-18.5
            PTE DI题练习  1h  18.5-19.5
技能进阶 10h
重新分配啥时间
PMPP 2h，专门用来做课后题，做那个什么solution 8-10，继续磨脑子
MPI 2h，专门用来学协议  10-12 开脑子，训练脑子，边看边整理记录，边练习
Leetcode 1h，每天1道 7-8 开胃奖励小菜，
导师任务 13-17 4h 

其他：
晚休，0.5h 跳上15分钟操，睡觉
午休1h 12-13，跳上20分钟操，睡觉
早上准备工作1h 6-7
洗澡洗衣0.5h 22:30-23:00
睡眠7h  23-6

4：30起床，洗漱洗脸刷牙吃药
5：00-6：00 边听英语边走一小时路，学一集单词
6：00出发，买早餐出发，2鸡蛋，1个菜夹馍，在小区楼底下晒晒太阳
6：30-7：00 记录所思所想
7：00-8：00 github刷题
8：00-11：00学一章节MPI，边看分析边做笔记记录，在借助GPT的前提下是没问题的
11：00-13：00 做项目
13：00-13：30 午休
13：30-18：30 写代码
18：30-19：30 新概念英语跟读
19：30-20：30 DI口语练习
20：30 回家
21：00-22：00 英语听力练习
22：00-22：30 复习早上的单词
22：30 洗澡洗衣睡觉


