#include <mpi.h>
#include <cuda_runtime.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>

// CUDA error checking
#define CUDA_CHECK(call)                                                   
do                                                                          
{                                                                           
    cudaError_t err = (call);                                              
    if (err != cudaSuccess)                                                
    {                                                                       
        std::cerr << "CUDA error: " << cudaGetErrorString(err)             
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl;   
        MPI_Abort(MPI_COMM_WORLD, 1);  //终止MPI communicator中的整个MPI程序                                    
    }                                                                       
} while (0)

// GPU reduction kernel  //两个数组对应元素相加的kernel
__global__
void reduce_chunk(int* data,const int* recv_buffer,int offset,int count)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < count)
    {
        data[offset + i] += recv_buffer[i];
    }
}

// GPU copy kernel
__global__
void copy_chunk(int* data, const int* recv_buffer,int offset,int count)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < count)
    {
        data[offset + i] = recv_buffer[i];
    }
}
// Print data

void print_data(int rank,const char* stage,int step, const std::vector<int>& data)
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
    // Problem size
    const int N = 10;
    // Ring neighbors
    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;
    // Uneven chunk distribution
    std::vector<int> counts(size);
    std::vector<int> displs(size);
    int base = N / size;
    int remainder = N % size;
    for (int i = 0; i < size; i++)
    {
        counts[i] = base + (i < remainder ? 1 : 0);
    }
    displs[0] = 0;
    for (int i = 1; i < size; i++)
    {
        displs[i] = displs[i - 1] + counts[i - 1];
    }
    // Host data
    std::vector<int> data(N);
    for (int i = 0; i < N; i++)
    {
        data[i] = rank * 10 + i;
    }
    // Initial data
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
    // Print chunk distribution
    if (rank == 0)
    {
        std::cout << "\nChunk distribution:\n";
        for (int i = 0; i < size; i++)
        {
            std::cout<< "chunk " << i
                << " | count = " << counts[i]
                << " | displacement = " << displs[i]
                << std::endl;
        }
    }
    // Maximum chunk size
    MPI_Barrier(MPI_COMM_WORLD);
    int max_chunk_size =*std::max_element(counts.begin(), counts.end());
    // Host receive buffer
    std::vector<int> recv_buffer(max_chunk_size);
    // CUDA memory
    int* d_data = nullptr;
    int* d_recv_buffer = nullptr;
    CUDA_CHECK(cudaMalloc(&d_data,N * sizeof(int)));
    CUDA_CHECK(cudaMalloc(&d_recv_buffer,max_chunk_size * sizeof(int)));
    // Copy initial data:
    // Host → Device
    CUDA_CHECK(cudaMemcpy(d_data,data.data(),N * sizeof(int),cudaMemcpyHostToDevice));
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
                    << " (" << counts[send_chunk]
                    << " elements)"
                    << " -> Rank " << next

                    << " | receive chunk " << recv_chunk
                    << " (" << counts[recv_chunk]
                    << " elements)"
                    << " <- Rank " << prev

                    << std::endl;
            }
            MPI_Barrier(MPI_COMM_WORLD);
        }
        // MPI Send / Receive
        // IMPORTANT:
        // This first version still uses CPU memory.
        MPI_Sendrecv(
            data.data() + displs[send_chunk],
            counts[send_chunk],
            MPI_INT,
            next,
            0,

            recv_buffer.data(),
            counts[recv_chunk],
            MPI_INT,
            prev,
            0,

            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );

        CUDA_CHECK(cudaMemcpy(d_recv_buffer,recv_buffer.data(),counts[recv_chunk] * sizeof(int),cudaMemcpyHostToDevice));
        int threads = 256;
        int blocks =(counts[recv_chunk] + threads - 1)/ threads;
        reduce_chunk<<<blocks, threads>>>(d_data,displs[recv_chunk],counts[recv_chunk]);
        CUDA_CHECK(cudaGetLastError());
        CUDA_CHECK(cudaDeviceSynchronize());
        // Copy GPU result back:
        // Device → Host
        CUDA_CHECK(cudaMemcpy(data.data(),d_data,N * sizeof(int),cudaMemcpyDeviceToHost));
        // Print result after reduction
        MPI_Barrier(MPI_COMM_WORLD);
        for (int r = 0; r < size; r++)
        {
            if (rank == r)
            {
                print_data(rank,"after GPU reduction",step,data);
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
                    << " Rank " << rank
                    << " | step " << step

                    << " | send chunk " << send_chunk
                    << " (" << counts[send_chunk]
                    << " elements)"
                    << " -> Rank " << next

                    << " | receive chunk " << recv_chunk
                    << " (" << counts[recv_chunk]
                    << " elements)"
                    << " <- Rank " << prev

                    << std::endl;
            }

            MPI_Barrier(MPI_COMM_WORLD);
        }
        // MPI Send / Receive
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
        // Host → Device
        CUDA_CHECK(cudaMemcpy(d_recv_buffer,recv_buffer.data(),counts[recv_chunk] * sizeof(int),cudaMemcpyHostToDevice));
        // GPU copy
        int threads = 256;
        int blocks =(counts[recv_chunk] + threads - 1)/ threads;
        copy_chunk<<<blocks, threads>>>(d_data,d_recv_buffer,displs[recv_chunk],counts[recv_chunk]);
        CUDA_CHECK(cudaGetLastError());
        CUDA_CHECK(cudaDeviceSynchronize());
        // Device → Host
        CUDA_CHECK(cudaMemcpy(data.data(),d_data, N * sizeof(int),cudaMemcpyDeviceToHost));
        // Print after Allgather
        MPI_Barrier(MPI_COMM_WORLD);
        for (int r = 0; r < size; r++)
        {
            if (rank == r)
            {
                print_data( rank,"after GPU allgather",step,data);
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
            std::cout<< "\n========== FINAL =========="
                << std::endl;
            std::cout<< "Rank " << rank
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
        int expected =10 * size * (size - 1) / 2 + i * size;
        if (data[i] != expected)
        {
            correct = false;
        }
    }
    if (rank == 0)
    {
        if (correct)
        {
            std::cout<< "\nVerification: PASS"<< std::endl;
        }
        else
        {
            std::cout<< "\nVerification: FAIL"<< std::endl;
        }
    }
    // Free CUDA memory
    CUDA_CHECK(cudaFree(d_data));
    CUDA_CHECK(cudaFree(d_recv_buffer));
    // Finalize MPI
    MPI_Finalize();
    return 0;
}