/*	Andrés Manglano Cañizares NIA: 100303534
	Fayán Leonardo Pardo Ladino NIA: 100303560
*/


#include "fs_server.h"

void
fs_server_1(char *host, char* remotefile, char* localfile)
{
	CLIENT *clnt;
	enum clnt_stat retval_1;
	int result_getSize;

	enum clnt_stat retval_3;
	int result_open;
	
	enum clnt_stat retval_4;
	int result_close;
	
	enum clnt_stat retval_6;
	ReadRes result_read;

#ifndef	DEBUG
	clnt = clnt_create (host, FS_SERVER, FS_SERVER_V1, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	//Remote file of the server is opened for read
	retval_3 = mi_open_1(remotefile, READMODE, &result_open, clnt);
	if (retval_3 != RPC_SUCCESS) {
		clnt_perror (clnt, "Open call failed");
	}
	if(result_open < 0){
		puts("Error opening remote file");
		exit(-1);
	}
	//We get the size of the remote file
	retval_1 = mi_get_size_1(result_open, &result_getSize, clnt);
	if (retval_1 != RPC_SUCCESS) {
		clnt_perror (clnt, "getSize call failed");
	}
	//We create/open a local file for writing in it
	int check = open(localfile, O_WRONLY | O_CREAT, 0666);
	if (check < 0){
		printf("Error opening local file \n");
	}

	int it = result_getSize/NFSMAXDATA2; //number of parts we need to send to the server
	int realsize;
	int chuck;
	int atleast = 0; //this variable will indicate if there was more than 1 iteration
	int position = 0;
    int pointer = 0;
    
	while(it >= 0){
		if(it==0){
			realsize = result_getSize - ((result_getSize/NFSMAXDATA2)*NFSMAXDATA2);
			//if it is the last iteration, we reallocate the string space with the size of the last part
			if(atleast==1){
                result_read.data.RWData_val = (char *) realloc(result_read.data.RWData_val,realsize);
            }else{ //if it is the first and only iteration we allocate the space needed
                result_read.data.RWData_val = malloc(realsize);
            }
		
		}else{
			//if it is the first iteration of several we allocate the max space permitted to send
			if(it == result_getSize/NFSMAXDATA2){
				realsize=NFSMAXDATA2;
                result_read.data.RWData_val = malloc(realsize);
			}
		}
		//The values of the string are resetted and actual position is updated
		memset(result_read.data.RWData_val, 0, realsize);
		position = pointer*NFSMAXDATA2;
		//We communicate with the server to read a part of the remote file
		retval_6 = mi_read_1(result_open, position, realsize, &result_read, clnt);
		if (retval_6 != RPC_SUCCESS) {
			clnt_perror (clnt, "Read call failed");
		}
		//The part that was just read is written in the local file
		chuck = write(check, result_read.data.RWData_val, realsize);
		if (chuck < 0){
			printf("Error writing to local file \n");
		}
		it--;
        pointer++;
		atleast = 1;
	}	
	
	//We communicate with the server to close the remote file
	retval_4 = mi_close_1(result_open, &result_close, clnt);
	if (retval_4 != RPC_SUCCESS) {
		clnt_perror (clnt, "Close call failed");
	}
	//We close the local file
	chuck = close(check);
	if (chuck < 0){
		printf("Error closing local file \n");
	}
	if(result_close == 0 && chuck ==0){
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
	/* Three strings declared to store the arguments of the delete client which are passed to the fs_server_1 function in which the logic of the client is performed*/
	char *host;
	char *remote;
	char *local;

	if (argc < 3) {
		printf ("usage: %s server_host remote_file local_file\n", argv[0]);
		exit (1);
	}

	host = argv[1];
	remote = argv[2];
	local = argv[3];
	fs_server_1 (host, remote, local);
	exit (0);
}
