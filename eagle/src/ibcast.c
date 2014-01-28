/*
 * ibcast.c
 *
 * implements a nonblocking broadcast 
 */

#include <stdlib.h>
#include "ibcast.h"

/* internal functions */
static int Ibcast_work(Ibcast_handle *handle);

/* keyval for attribute cacheing */
int ibcast_keyval = MPI_KEYVAL_INVALID;

int Ibcast(void *buf, int count, MPI_Datatype datatype, int root, 
     MPI_Comm comm, Ibcast_handle **handle_out) {
  Ibcast_syshandle  *syshandle;
  Ibcast_handle     *handle;
  int                flag;
  int                mask;
  int                relrank;
  int                no_handles;
  int                retn;
  int                size;
  int                rank;
  int                req_no = 0;

  MPI_Copy_function  Ibcast_copy;
  MPI_Delete_function  Ibcast_delete;
  
  /* get info about the communicator */
  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);

  /* if size is 1, this is trivially finished */
  if (size == 1) {
    (*handle_out) = NULL;
    return(MPI_SUCCESS);
  }
  
  /* first see if this library has ever been called. 
     Generate new key value if not */
  if (ibcast_keyval == MPI_KEYVAL_INVALID) {
    MPI_Keyval_create(Ibcast_copy, Ibcast_delete, &ibcast_keyval, NULL);
  }
  
  /* this communicator might already have used this collective operation,
     so we can use the information cached on it */
  MPI_Attr_get(comm, ibcast_keyval, (void **)&syshandle, &flag);
  if (flag == 0) {
    /* there was no attribute previously cached */
    syshandle = (Ibcast_syshandle *) malloc(sizeof(Ibcast_syshandle));
    
    /* fill in the attribute information */
    syshandle->ordering_tag = 0; /* start with tag zero */
    MPI_Comm_dup(comm, &(syshandle->comm));  /* duplicate comm */

    /* attach information to the communicator */
    MPI_Attr_put(comm, ibcast_keyval, (void *) syshandle);
  }

  /* create a handle for this particular ibcast operation */
  handle = (Ibcast_handle *) malloc(sizeof(Ibcast_handle));
  handle->num_sends = 0;
  handle->num_recvs = 0;
  
  /* count how many send/recv handles we need */
  mask = 0x1;
  relrank = (rank - root + size) % size;
  while ((mask & relrank) == 0 && mask < size) {
    if ((relrank | mask) < size) {
      handle->num_sends++;
    }
    mask <<= 1;
  }
  if (mask < size) {
    handle->num_recvs++;
  }
  
  /* allocate request and status arrays for sends and receives */
  no_handles = handle->num_sends + handle->num_recvs;
  handle->req_array = (MPI_Request *) malloc(no_handles * sizeof(MPI_Request));
  handle->stat_array = (MPI_Status *) malloc(no_handles * sizeof(MPI_Status));
  
  /* create "permanent" send/recv requests */
  mask = 0x1;
  while ((mask & relrank) == 0 && mask < size) {
    if ((relrank | mask) < size) {
      MPI_Send_init(buf, count, datatype, ((relrank | mask) + root)%size,
                    syshandle->ordering_tag, syshandle->comm,
                    &(handle->req_array[req_no++]));
    }
    mask <<= 1;
  }
  if (mask < size) {
    MPI_Recv_init(buf, count, datatype, ((relrank & (~ mask)) + root)%size,
                  syshandle->ordering_tag, syshandle->comm,
                  &(handle->req_array[req_no++]));
  }
  
  /* start send/recv requests */
  retn = Ibcast_work(handle);

  /* update the cached information:
     increase the tag for next ibcast operation to avoid back-masking */
  ++(syshandle->ordering_tag);

  /* return handle and error code */
  (*handle_out) = handle;
  return(retn);
}


static int Ibcast_work(Ibcast_handle *handle) {
  /* if I don't have any recv's, start all my sends -- the root */
  if (handle->num_recvs == 0) {
    MPI_Startall(handle->num_sends, handle->req_array);
  } else {
    /* start all my recv's */
    MPI_Startall(handle->num_recvs, &handle->req_array[handle->num_sends]);
  }
  return(MPI_SUCCESS);
}

int Ibcast_wait(Ibcast_handle **handle_out) {
  Ibcast_handle   *handle = (*handle_out);
  int              retn;
  int              i;

  /* A NULL handle means there's nothing to do */
  if (handle == NULL) {
    return(MPI_SUCCESS);
  }
  
  /* If I wasn't the root, finish my recvs and start my sends */
  if (handle->num_recvs != 0) {
    MPI_Waitall(handle->num_recvs, &handle->req_array[handle->num_sends],
                &handle->stat_array[handle->num_sends]);
    MPI_Startall(handle->num_sends, handle->req_array);
  }
  
  /* wait for all my sends */
  retn = MPI_Waitall(handle->num_sends, handle->req_array, handle->stat_array);

  /* free permanent requests */
  for (i=0; i < (handle->num_sends + handle->num_recvs); i++) {
    MPI_Request_free( &(handle->req_array[i]) );
  }

  /* free handle */
  free(handle->req_array);
  free(handle->stat_array);
  free(handle);

  /* return NULL handle */
  (*handle_out) = NULL;
  return(retn);
}

/* copy function for ibcast cached information */
int Ibcast_copy(MPI_Comm oldcomm, int keyval, void *extra, void *attr_in, 
    void *attr_out, int *flag) {
  Ibcast_syshandle  *syshandle = (Ibcast_syshandle *) attr_in;
  Ibcast_syshandle  *new_syshandle;
  
  /* do we have valid keyval and attr ? */
  if ( (keyval != ibcast_keyval) || (syshandle = NULL) ) {
    return(-1);
  }
  
  /* create a new syshandle for the new communicator */
  new_syshandle = (Ibcast_syshandle *) malloc(sizeof(Ibcast_syshandle));

  /* fill in the attribute information */
  new_syshandle->ordering_tag = 0;  /* start with tag zero */
  MPI_Comm_dup(syshandle->comm, &(new_syshandle->comm));
                                    /* dup the "hidden" communicator */

  /* return new syshandle and set flag */
  (*(void**)attr_out) = (void *) new_syshandle;
  (*flag) = 1;
  return(MPI_SUCCESS);
}


/* destructor for ibcast cached information */
int Ibcast_delete(MPI_Comm comm, int keyval, void *attr_val, void *extra) {
  Ibcast_syshandle *syshandle = (Ibcast_syshandle *) attr_val;
  
  /* do we have valid keyval and attr ? */
  if ( (keyval != ibcast_keyval) || (syshandle == NULL) ) {
    return(-1);
  }

  /* free the "hidden" comm and syshandle */
  MPI_Comm_free(&(syshandle->comm));
  free(syshandle);
  return(MPI_SUCCESS);
}
