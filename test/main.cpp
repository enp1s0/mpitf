#include <iostream>
#include <mpitf/mpitf.hpp>

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);

	int rank = mpitf::get_comm_rank(MPI_COMM_WORLD);
	int nprocs = mpitf::get_comm_size(MPI_COMM_WORLD);

	int local_rank = mpitf::get_rank_within_node(MPI_COMM_WORLD);
	int local_size = mpitf::get_size_within_node(MPI_COMM_WORLD);

	std::printf("%4d / %4d : local rank = %4d / %4d\n", rank, nprocs, local_rank, local_size);

	using data_t = int;
	data_t send = rank;
	data_t recv = 0;
	MPITF_HANDLE_ERROR(MPI_Allreduce(&send, &recv, 1, mpitf::get_data_type<data_t>(), MPI_SUM, MPI_COMM_WORLD));
	std::cout << "Allreduce sum = " << recv <<std::endl;

	MPI_Finalize();
}
