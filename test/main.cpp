#include <iostream>
#include <mpi_local_rank.hpp>

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);

	int rank, nprocs;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

	int local_rank = mtk::get_rank_within_node(MPI_COMM_WORLD);
	int local_size = mtk::get_size_within_node(MPI_COMM_WORLD);

	std::printf("%4d / %4d : local rank = %4d / %4d\n", rank, nprocs, local_rank, local_size);

	MPI_Finalize();
}
