#ifndef __MPI_LOCAL_RANK_HPP__
#define __MPI_LOCAL_RANK_HPP__
#include <cstddef>
#include <cstdint>
#include <memory>
#include <mpi.h>
#include <unistd.h>

namespace mtk {
inline int get_size_within_node(const MPI_Comm mpi_comm) {
	const std::size_t hostname_max_size = 256;
	char hostname[hostname_max_size];

	gethostname(hostname, hostname_max_size);

	std::uint64_t hash = 2999;
	for(std::size_t i = 0; (i < hostname_max_size) && (hostname[i] != '\0'); i++) {
		hash = hash * 16437 + static_cast<std::uint64_t>(hostname[i]);
	}

	int nprocs, rank;
	MPI_Comm_size(mpi_comm, &nprocs);
	MPI_Comm_rank(mpi_comm, &rank);

	std::unique_ptr<std::uint64_t[]> hash_table(new std::uint64_t [nprocs]);

	MPI_Allgather(&hash, 1, MPI_LONG_LONG, hash_table.get(), 1, MPI_LONG_LONG, mpi_comm);

	int local_size = 0;
	for(std::size_t i = 0; i < nprocs; i++) {
		if(hash_table.get()[i] == hash) {
			local_size++;
		}
	}

	return local_size;
}

inline int get_rank_within_node(const MPI_Comm mpi_comm) {
	const std::size_t hostname_max_size = 256;
	char hostname[hostname_max_size];

	gethostname(hostname, hostname_max_size);

	std::uint64_t hash = 2999;
	for(std::size_t i = 0; (i < hostname_max_size) && (hostname[i] != '\0'); i++) {
		hash = hash * 16437 + static_cast<std::uint64_t>(hostname[i]);
	}

	int nprocs, rank;
	MPI_Comm_size(mpi_comm, &nprocs);
	MPI_Comm_rank(mpi_comm, &rank);

	std::unique_ptr<std::uint64_t[]> hash_table(new std::uint64_t [nprocs]);

	MPI_Allgather(&hash, 1, MPI_LONG_LONG, hash_table.get(), 1, MPI_LONG_LONG, mpi_comm);

	int local_rank = 0;
	for(std::size_t i = 0; i < rank; i++) {
		if(hash_table.get()[i] == hash) {
			local_rank++;
		}
	}

	return local_rank;
}

inline int get_comm_rank(const MPI_Comm mpi_comm) {
	int rank;
	MPI_Comm_rank(mpi_comm, &rank);
	return rank;
}

inline int get_comm_size(const MPI_Comm mpi_comm) {
	int size;
	MPI_Comm_size(mpi_comm, &size);
	return size;
}
} // namespace mtk

#endif /* end of include guard */
