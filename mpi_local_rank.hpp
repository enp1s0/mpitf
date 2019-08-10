#ifndef __MPI_LOCAL_RANK_HPP__
#define __MPI_LOCAL_RANK_HPP__
#include <cstddef>
#include <mpi.h>
#include <unistd.h>

namespace mtk {
int get_rank_within_node(const MPI_Comm mpi_comm) {
	const std::size_t hostname_max_size = 256;
	char hostname[hostname_max_size];

	gethostname(hostname, hostname_max_size);
}
} // namespace mtk

#endif /* end of include guard */
