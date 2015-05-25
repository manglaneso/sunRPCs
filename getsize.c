/*	Andrés Manglano Cañizares NIA: 100303534
	Fayán Leonardo Pardo Ladino NIA: 100303560
*/



#include "fs_server.h"

void
fs_server_1(char *host, char* remotefile)
{
	CLIENT *clnt;
	enum clnt_stat retval_1;
	int result_open;

	enum clnt_stat retval_2;
	int result_getSize;

	enum clnt_stat retval_3;
	int result_close;

#ifndef	DEBUG
	clnt = clnt_create (host, FS_SERVER, FS_SERVER_V1, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	//We communicate with the server to open the file
	retval_1 = mi_open_1(remotefile, READMODE, &result_open, clnt);
	if (retval_1 != RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	}
	//we communicate with the server to get the size of the file
	retval_2 = mi_get_size_1(result_open, &result_getSize,  clnt);
	if (retval_2 != RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	}
	// Result is printed in the client
	if(result_getSize>=0){
		printf("The size of %s is %i Bytes\n", remotefile, result_getSize);
	}else{
		puts("Error getting size (file may not exist)\n");
	}
	retval_3 = mi_close_1(result_open, &result_close, clnt);
	if (retval_3 != RPC_SUCCESS) {
		clnt_perror (clnt, "Close call failed");
	}
	if(result_close == 0){
		puts("Action completed succesfully");
	}else{
		puts("Error closing files");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}

int
main (int argc, char *argv[])
{
	/* Two strings declared to store the arguments of the delete client which are passed to the fs_server_1 function in which the logic of the client is performed*/
	char *host;
	char *remote;

	if (argc < 2) {
		printf ("usage: %s server_host remote_file\n", argv[0]);
		exit (1);
	}

	host = argv[1];
	remote = argv[2];
	fs_server_1 (host, remote);
	exit (0);
}
