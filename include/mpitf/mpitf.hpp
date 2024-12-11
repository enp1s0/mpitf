#ifndef __MPITF_HPP__
#define __MPITF_HPP__
#include <cstddef>
#include <cstdint>
#include <memory>
#include <sstream>
#include <mpi.h>
#include <unistd.h>

#ifndef MPITF_HANDLE_ERROR
#define MPITF_HANDLE_ERROR(status) mpitf::error::check(status, __FILE__, __LINE__, __func__)
#endif
#ifndef MPITF_HANDLE_ERROR_M
#define MPITF_HANDLE_ERROR_M(status, message) mpitf::error::check(status, __FILE__, __LINE__, __func__, message)
#endif

namespace mpitf {
namespace error {
inline void check(const int status, const std::string filename, const std::size_t line, const std::string funcname, const std::string message = "") {
	if (status != MPI_SUCCESS) {
		char error_string[MPI_MAX_ERROR_STRING];
		int error_string_length;
		MPI_Error_string(status, error_string, &error_string_length);
	
		std::stringstream ss;
		ss << error_string;
		if(message.length() != 0){
			ss<<" : "<<message;
		}
	    ss<<" ["<<filename<<":"<<line<<" in "<<funcname<<"]";
		throw std::runtime_error(ss.str());
	}
}
} // namespace error
template <class T>
inline MPI_Datatype get_data_type();
template <> inline MPI_Datatype get_data_type<char          >() {return MPI_CHAR          ;};
template <> inline MPI_Datatype get_data_type<signed char   >() {return MPI_SIGNED_CHAR   ;};
template <> inline MPI_Datatype get_data_type<unsigned char >() {return MPI_UNSIGNED_CHAR ;};
template <> inline MPI_Datatype get_data_type<wchar_t       >() {return MPI_WCHAR         ;};
template <> inline MPI_Datatype get_data_type<short         >() {return MPI_SHORT         ;};
template <> inline MPI_Datatype get_data_type<unsigned short>() {return MPI_UNSIGNED_SHORT;};
template <> inline MPI_Datatype get_data_type<int           >() {return MPI_INT           ;};
template <> inline MPI_Datatype get_data_type<unsigned      >() {return MPI_UNSIGNED      ;};
template <> inline MPI_Datatype get_data_type<long          >() {return MPI_LONG          ;};
template <> inline MPI_Datatype get_data_type<unsigned long >() {return MPI_UNSIGNED_LONG ;};
template <> inline MPI_Datatype get_data_type<float         >() {return MPI_FLOAT         ;};
template <> inline MPI_Datatype get_data_type<double        >() {return MPI_DOUBLE        ;};
template <> inline MPI_Datatype get_data_type<long double   >() {return MPI_LONG_DOUBLE   ;};
//template <> inline MPI_Datatype get_data_type<int8_t        >() {return MPI_INT8_T        ;};
//template <> inline MPI_Datatype get_data_type<int16_t       >() {return MPI_INT16_T       ;};
//template <> inline MPI_Datatype get_data_type<int32_t       >() {return MPI_INT32_T       ;};
//template <> inline MPI_Datatype get_data_type<int64_t       >() {return MPI_INT64_T       ;};
//template <> inline MPI_Datatype get_data_type<uint8_t       >() {return MPI_UINT8_T       ;};
//template <> inline MPI_Datatype get_data_type<uint16_t      >() {return MPI_UINT16_T      ;};
//template <> inline MPI_Datatype get_data_type<uint32_t      >() {return MPI_UINT32_T      ;};
//template <> inline MPI_Datatype get_data_type<uint64_t      >() {return MPI_UINT64_T      ;};

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

inline int get_size_within_node(const MPI_Comm mpi_comm) {
	const std::size_t hostname_max_size = 256;
	char hostname[hostname_max_size];

	gethostname(hostname, hostname_max_size);

	std::uint64_t hash = 2999;
	for(std::size_t i = 0; (i < hostname_max_size) && (hostname[i] != '\0'); i++) {
		hash = hash * 16437 + static_cast<std::uint64_t>(hostname[i]);
	}

	const int nprocs = mpitf::get_comm_size(mpi_comm);
	const int rank = mpitf::get_comm_rank(mpi_comm);

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

	const int nprocs = mpitf::get_comm_size(mpi_comm);
	const int rank = mpitf::get_comm_rank(mpi_comm);

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
} // namespace mpitf

#endif /* end of include guard */
