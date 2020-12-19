/*
 *  header for ibcast library, implementing a nonblocking broadcast
 */

#include <mpi.h>

/* handle for ibcast operations on a communicator */
typedef struct {
  MPI_Comm  comm;
  int       ordering_tag;
} Ibcast_syshandle;

/* handle for a particular ibcast operation */
typedef struct {
  MPI_Request   *req_array;
  MPI_Status    *stat_array;
  int            num_sends;
  int            num_recvs;
} Ibcast_handle;

/* function prototypes */
int Ibcast(void *buf, int count, MPI_Datatype datatype, int root, 
           MPI_Comm comm, Ibcast_handle **handle_out);
int Ibcast_wait(Ibcast_handle **handle_out);
