
#include <mpi.h>
#include <iostream>
#include <vector>

void print_data(int rank,const char* stage,int step,const std::vector<int>& data)
{
    std::cout << "Rank " << rank<< " | " << stage<< " | step " << step<< " | data: ";
    for (int x : data)
    {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const int N = 8;

    // Check
    if (N % size != 0)  //等大小chunk模型
    {
        if (rank == 0)
        {
            std::cerr<< "N must be divisible by number of processes\n";
        }

        MPI_Finalize();
        return 1;
    }

    // Initial data

    std::vector<int> data(N);

    for (int i = 0; i < N; i++)
    {
        data[i] = rank * 10 + i;
    }

    int chunk_size = N / size;

    // Ring neighbors
    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;

    // Temporary receive buffer
    std::vector<int> recv_buffer(chunk_size);

    // Print initial data
    MPI_Barrier(MPI_COMM_WORLD);
    for (int r = 0; r < size; r++)
    {
        if (rank == r)
        {
            std::cout << "Rank " << rank << " initial: ";
            for (int x : data)
            {
                std::cout << x << " ";
            }
            std::cout << std::endl;
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    // Phase 1: Reduce-Scatter
    /*
    每一轮，每个 Rank 把一个 chunk 往右传，同时从左边收到另一个 chunk；
    收到之后，把两个 chunk 做加法。
    经过 P-1 轮，每个 chunk 都完成了所有 Rank 的求和，并且分散在不同 Rank 上
    
    */

    for (int step = 0; step < size - 1; step++)  //size-1次已经走过所有其他rank了
    {

        int send_chunk =(rank - step + size) % size;  //此轮，应该把哪个chunk发给右边rank
        int recv_chunk =(rank - step - 1 + size) % size;//从左边rank收到哪个chunk，并对这个chunk做reduction
        // Print communication plan
        MPI_Barrier(MPI_COMM_WORLD);

        for (int r = 0; r < size; r++)
        {
            if (rank == r)
            {
                std::cout
                    << "\n[Reduce-Scatter]"
                    << " Rank " << rank
                    << " | step " << step
                    << " | send chunk " << send_chunk
                    << " -> Rank " << next
                    << " | receive chunk " << recv_chunk
                    << " <- Rank " << prev
                    << std::endl;
            }

            MPI_Barrier(MPI_COMM_WORLD);
        }
        // Print before communication
        MPI_Barrier(MPI_COMM_WORLD);

        for (int r = 0; r < size; r++)
        {
            if (rank == r)
            {
                print_data( rank,"before Sendrecv", step,data);
            }

            MPI_Barrier(MPI_COMM_WORLD);
        }
        // Send / Receive  每个rank同时做
        MPI_Sendrecv(data.data() + send_chunk * chunk_size, //找到要发送chunk的起始地址
            chunk_size, MPI_INT,
            next,0,
            recv_buffer.data(),
            chunk_size,MPI_INT,prev,0,
            MPI_COMM_WORLD,MPI_STATUS_IGNORE
        );

        // Print received chunk
        MPI_Barrier(MPI_COMM_WORLD);

        for (int r = 0; r < size; r++)
        {
            if (rank == r)
            {
                std::cout<< "Rank " << rank
                    << " received chunk "
                    << recv_chunk
                    << ": ";
                for (int x : recv_buffer)
                {
                    std::cout << x << " ";
                }

                std::cout << std::endl;
            }

            MPI_Barrier(MPI_COMM_WORLD);
        }

        // 接收到的加在自己原来对应位置的chunk上

        for (int i = 0; i < chunk_size; i++)
        {
            data[recv_chunk * chunk_size + i]
                += recv_buffer[i];
        }
        // Print after reduction
        MPI_Barrier(MPI_COMM_WORLD);

        for (int r = 0; r < size; r++)
        {
            if (rank == r)
            {
                print_data( rank,"after reduction",step, data
                );
            }
            MPI_Barrier(MPI_COMM_WORLD);
        }
    }

    // Phase 2: Allgather
    /*
    Reduce-Scatter 结束后，每个 Rank 只有一个“已经完成全局求和”的 chunk；
    Allgather 的任务就是把这个完整 chunk 沿 Ring 传播，最终每个 Rank 都拿到所有 chunk
    RankN->chunk N+2
    */

    for (int step = 0; step < size - 1; step++)
    {   //C2->R1，C3->R2
        int send_chunk =(rank + 1 - step + size) % size;//把哪个已获得完整的chunk发给右边rank
        int recv_chunk = (rank - step + size) % size;  //收到直接保存
        // Print communication plan
        MPI_Barrier(MPI_COMM_WORLD);
        for (int r = 0; r < size; r++)
        {
            if (rank == r)
            {
                std::cout
                    << "\n[Allgather]"
                    << " Rank " << rank
                    << " | step " << step
                    << " | send chunk " << send_chunk
                    << " -> Rank " << next
                    << " | receive chunk " << recv_chunk
                    << " <- Rank " << prev
                    << std::endl;
            }

            MPI_Barrier(MPI_COMM_WORLD);
        }

   
        // Send / Receive
   

        MPI_Sendrecv(
            data.data() + send_chunk * chunk_size,
            chunk_size,
            MPI_INT,
            next,
            1,

            recv_buffer.data(),
            chunk_size,
            MPI_INT,
            prev,
            1,

            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );

   
        // Store received chunk
   

        for (int i = 0; i < chunk_size; i++)
        {
            data[recv_chunk * chunk_size + i]
                = recv_buffer[i];
        }

   
        // Print after Allgather step
   

        MPI_Barrier(MPI_COMM_WORLD);

        for (int r = 0; r < size; r++)
        {
            if (rank == r)
            {
                print_data(
                    rank,
                    "after allgather",
                    step,
                    data
                );
            }

            MPI_Barrier(MPI_COMM_WORLD);
        }
    }


    // Final result


    MPI_Barrier(MPI_COMM_WORLD);

    for (int r = 0; r < size; r++)
    {
        if (rank == r)
        {
            std::cout
                << "\n========== FINAL =========="
                << std::endl;

            std::cout
                << "Rank " << rank << ": ";

            for (int x : data)
            {
                std::cout << x << " ";
            }

            std::cout << std::endl;
        }

        MPI_Barrier(MPI_COMM_WORLD);
    }


    // Finalize


    MPI_Finalize();

    return 0;
}

