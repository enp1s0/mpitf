# Function for getting a local rank within a node

## Requirement
- C++ (11 or later)

## Sample

```cpp
#include <iostream.hpp>
#include <mpi_local_rank.hpp>

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);

	int rank, nprocs;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

	int local_rank = mtk::get_rank_within_node(MPI_COMM_WORLD);

	std::printf("%4d / %4d : local rank = %4d\n", rank, nprocs, local_rank);

	MPI_Finalize();
}
```
