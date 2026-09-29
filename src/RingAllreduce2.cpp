
#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>


// Print data


void print_data(
    int rank,
    const char* stage,
    int step,
    const std::vector<int>& data)
{
    std::cout << "Rank " << rank
              << " | " << stage
              << " | step " << step
              << " | data: ";

    for (int x : data)
    {
        std::cout << x << " ";
    }

    std::cout << std::endl;
}



// Main


int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Arbitrary N
    const int N = 10;
    // Ring neighbors
    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;

    std::vector<int> counts(size);
    std::vector<int> displs(size);

    //这里做了区别，确保了不能被整除的情况
    int base = N / size;
    int remainder = N % size;
    for (int i = 0; i < size; i++)
    {
        counts[i] = base + (i < remainder ? 1 : 0);
    }
    displs[0] = 0;
    for (int i = 1; i < size; i++)
    {
        displs[i] =displs[i - 1] + counts[i - 1];
    }
    std::vector<int> data(N);

    for (int i = 0; i < N; i++)
    {
        data[i] = rank * 10 + i;
    }
    MPI_Barrier(MPI_COMM_WORLD);//所有进程必须到达这里，等都到齐了，才往下执行

    for (int r = 0; r < size; r++)
    {
        if (rank == r)
        {
            std::cout << "Rank " << rank<< " initial: ";
            for (int x : data)
            {
                std::cout << x << " ";
            }
            std::cout << std::endl;
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }


    // Print chunk distribution
    if (rank == 0)
    {
        std::cout << "\nChunk distribution:\n";
        for (int i = 0; i < size; i++)
        {
            std::cout
                << "chunk " << i
                << " | count = " << counts[i]
                << " | displacement = " << displs[i]
                << std::endl;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    int max_chunk_size =*std::max_element(counts.begin(),counts.end());
    std::vector<int> recv_buffer(max_chunk_size);
    // Phase 1: Reduce-Scatter
    for (int step = 0; step < size - 1; step++)
    {
        int send_chunk =(rank - step + size) % size;
        int recv_chunk =(rank - step - 1 + size) % size;
        // Print communication plan
        MPI_Barrier(MPI_COMM_WORLD);
        for (int r = 0; r < size; r++)
        {
            if (rank == r)
            {
                std::cout<< "\n[Reduce-Scatter]"
                    << " Rank " << rank
                    << " | step " << step
                    << " | send chunk " << send_chunk
                    << " (" << counts[send_chunk] << " elements)"
                    << " -> Rank " << next
                    << " | receive chunk " << recv_chunk
                    << " (" << counts[recv_chunk] << " elements)"
                    << " <- Rank " << prev
                    << std::endl;
            }

            MPI_Barrier(MPI_COMM_WORLD);
        }

        // Send / Receive
        MPI_Sendrecv(
            data.data() + displs[send_chunk],//要发送的数据
            counts[send_chunk],//发送多少个元素
            MPI_INT,//发送数据类型
            next,//发送给谁
            0,//tag
            recv_buffer.data(),//要接收的数据放哪
            counts[recv_chunk],//接收多少个元素
            MPI_INT,//接收数据类型
            prev,//从谁那接收
            0,//tag

            MPI_COMM_WORLD,//通信域
            MPI_STATUS_IGNORE//接收状态
        );

        // Reduction
        for (int i = 0; i < counts[recv_chunk]; i++)
        {
            data[displs[recv_chunk] + i]
                += recv_buffer[i];
        }

        // Print result after reduction
        MPI_Barrier(MPI_COMM_WORLD);
        for (int r = 0; r < size; r++)
        {
            if (rank == r)
            {
                print_data(rank,"after reduction",step,data);
            }
            MPI_Barrier(MPI_COMM_WORLD);
        }
    }

    // Phase 2: Allgather
    for (int step = 0; step < size - 1; step++)
    {
        int send_chunk =(rank + 1 - step + size) % size;
        int recv_chunk =(rank - step + size) % size;
        // Print communication plan
        MPI_Barrier(MPI_COMM_WORLD);

        for (int r = 0; r < size; r++)
        {
            if (rank == r)
            {
                std::cout<< "\n[Allgather]"
                    << " Rank " << rank << " | step " << step<< " | send chunk " << send_chunk
                    << " (" << counts[send_chunk] << " elements)"<< " -> Rank " << next
                    << " | receive chunk " << recv_chunk
                    << " (" << counts[recv_chunk] << " elements)"
                    << " <- Rank " << prev<< std::endl;
            }
            MPI_Barrier(MPI_COMM_WORLD);
        }
        // Send / Receive
        MPI_Sendrecv(
            data.data() + displs[send_chunk],
            counts[send_chunk],
            MPI_INT,
            next,
            1,

            recv_buffer.data(),
            counts[recv_chunk],
            MPI_INT,
            prev,
            1,

            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );
        // Copy received chunk
        for (int i = 0; i < counts[recv_chunk]; i++)
        {
            data[displs[recv_chunk] + i]= recv_buffer[i];
        }
        // Print after Allgather
        MPI_Barrier(MPI_COMM_WORLD);
        for (int r = 0; r < size; r++)
        {
            if (rank == r)
            {
                print_data(rank,"after allgather",step,data);
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
                << "Rank " << rank
                << ": ";

            for (int x : data)
            {
                std::cout << x << " ";
            }

            std::cout << std::endl;
        }

        MPI_Barrier(MPI_COMM_WORLD);
    }


    // Verification

    bool correct = true;
    for (int i = 0; i < N; i++)
    {
        int expected =
            10 * size * (size - 1) / 2
            + i * size;

        if (data[i] != expected)
        {
            correct = false;
        }
    }
    if (rank == 0)
    {
        if (correct)
        {
            std::cout << "\nVerification: PASS"
                << std::endl;
        }
        else
        {
            std::cout<< "\nVerification: FAIL"<< std::endl;
        }
    }


    // Finalize
    MPI_Finalize();
    return 0;
}
