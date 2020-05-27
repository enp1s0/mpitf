#include <iostream>
#include <mpi_local_rank.hpp>

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);

	int rank = mtk::get_comm_rank(MPI_COMM_WORLD);
	int nprocs = mtk::get_comm_size(MPI_COMM_WORLD);

	int local_rank = mtk::get_rank_within_node(MPI_COMM_WORLD);
	int local_size = mtk::get_size_within_node(MPI_COMM_WORLD);

	std::printf("%4d / %4d : local rank = %4d / %4d\n", rank, nprocs, local_rank, local_size);

	using data_t = int;
	data_t send = rank;
	data_t recv = 0;
	MPI_Allreduce(&send, &recv, 1, mtk::get_data_type<data_t>(), MPI_SUM, MPI_COMM_WORLD);
	std::cout << "Allreduce sum = " << recv <<std::endl;

	MPI_Finalize();
}
