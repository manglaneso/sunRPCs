/*	Andrés Manglano Cañizares NIA: 100303534
	Fayán Leonardo Pardo Ladino NIA: 100303560
*/

#include "fs_server.h"

bool_t
mi_get_size_1_svc(int fd, int *result,  struct svc_req *rqstp)
{
	bool_t retval;

	/*
	 * Returns size of the file that corresponds the the file descriptor fd using lseek.
	 * It returns the size of the file in case of success, or -1 in case
	 * of error.
	 */
	puts("Getting the size of the remote file...");
	int check = lseek(fd, 0, SEEK_END);
	if (check >= 0){	
		*result = check;
	 	retval = TRUE;
	}else{
		*result = -1;
	 	retval = TRUE;
	}

	return retval;
}

bool_t
mi_creat_1_svc(char *name, int modo, int *result,  struct svc_req *rqstp)
{
	bool_t retval;

	/*
	 * Creates a file with the name and the POSIX mode given as arguments.
	 * Returns a file descriptor greater than 0 on success and -1 on error.
	 */
	puts("Creating a remote file...");
	int check = open(name, modo, 0666);
	if (check >= 0){
	 	*result = check;
	 	retval = TRUE;
	}else{
	 	*result = -1;
	 	retval = TRUE;
	}

	return retval;
}

bool_t
mi_open_1_svc(char *name, int flags, int *result,  struct svc_req *rqstp)
{
	bool_t retval;
	/*
	 * Opens the file given its name as parameter with the correspondent flag and 
	 * returns its file descriptor if successful and -1 if not.
	 */
	puts("Opening a remote file...");
	int check;
	if(flags == 0){
		
		check = open(name, O_RDONLY, 0666);
	}else if(flags == 1){
		
		check = open(name, O_WRONLY, 0666);
	}else if(flags == 3){
		
		check = open(name, O_RDWR, 0666);
	}else{
		*result = -1;
		retval = TRUE;
		return retval;
	}

	if (check > 0){
		*result = check;
	 	retval = TRUE;
	}else{
	 	*result = -1;
	 	retval = TRUE;
	}

	return retval;

}

bool_t
mi_close_1_svc(int fd, int *result,  struct svc_req *rqstp)
{
	bool_t retval;

	/*
	 * Close a opened file given its file descriptor passed as argument.
	 * Returns 0 on success and -1 on error.
	 */
	puts("Closing a remote file...");
	int check = close(fd);
	if (check == 0){
	 	*result = check;
	 	retval = TRUE;
	}else{
	 	*result = -1;
	 	retval = TRUE;
	}

	return retval;
}

bool_t
mi_unlink_1_svc(char *name, int *result,  struct svc_req *rqstp)
{
	bool_t retval;

	/*
	 * Unlink a file to reomve it. Returns 0 on success and -1 on error.
	 */
	puts("Unlinking a remote file...");
	int check = unlink(name);

	if (check == 0){
	 	*result = check;
	 	retval = TRUE;
	}else{
	 	*result = -1;
	 	retval = TRUE;
	}

	return retval;
}

bool_t
mi_read_1_svc(int fd, int pos, int size, ReadRes *result,  struct svc_req *rqstp)
{
	bool_t retval;

	/*
	 * Reads a remote file given its file descriptor. Its size and the position from
	 * where to start reading it it is also passed as argument. The data of the file
	 * to be read is returned in the RWData_val of the data element of the ReadRes 
	 * *result. On error it returns -1 and 0 if the end of the file is found.
	 */
	puts("Reading a remote file...");
	char coso[size];
	result->data.RWData_val = malloc(size);
	int check = lseek(fd, pos, 0);
	if (check == -1){	
	 	result->nread = -1;
	 	retval = TRUE;
	 	return retval;
	}

	check = read(fd, coso, size);
	if (check != -1){
		memcpy(result->data.RWData_val, coso, size);
	 	result->data.RWData_len = check;
	 	result->nread = check;
	 	retval = TRUE;
	}else{
	 	result->nread = -1;
	 	retval = TRUE;
	}
	
	return retval;
}

bool_t
mi_write_1_svc(int fd, RWData data, int pos, int size, int *result,  struct svc_req *rqstp)
{
	bool_t retval;

	/*
	 * Receives a file descriptor of a file and some data in a RWData struct and
	 * writes the data into the file. Returns 0 on success, -1 on error.
	 */
	puts("Writing a remote file...");
	int check = lseek(fd, pos, 0);
	if (check == -1){	
	 	retval = TRUE;
	 	return retval;
	}
	
	check = write(fd, data.RWData_val ,size);
	if (check >= 0){
	 	*result = check;
	 	retval = TRUE;
	}else{
	 	*result = -1;
	 	retval = TRUE;
	}

	return retval;
}

int
fs_server_1_freeresult (SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result)
{
	xdr_free (xdr_result, result);
	/*
	 * This function frees the result of the different functions.
	 */
	
	return 1;
}
