/*	Andrés Manglano Cañizares NIA: 100303534
	Fayán Leonardo Pardo Ladino NIA: 100303560
*/


#include "fs_server.h"

void
fs_server_1(char *host, char* remotefile)
{
	CLIENT *clnt;
	enum clnt_stat retval_1;
	int result_unlink;
	
#ifndef	DEBUG
	clnt = clnt_create (host, FS_SERVER, FS_SERVER_V1, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	//we communicate with the server to delete the remote file
	retval_1 = mi_unlink_1(remotefile, &result_unlink,  clnt);
	if (retval_1 != RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	}
	//Result printed in the client
	if(result_unlink==0){
		puts("File deleted correctly");
	}else{
		puts("Error deleting (file may not exist)");
	}
	
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}

int
main (int argc, char *argv[])
{	
	/* Two strings declared to store the arguments of the delete client which are passed to the fs_server_1
	 function in which the logic of the client is performed*/
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
