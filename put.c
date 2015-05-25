/*	Andrés Manglano Cañizares NIA: 100303534
	Fayán Leonardo Pardo Ladino NIA: 100303560
*/

#include "fs_server.h"

void
fs_server_1(char *host, char* remotefile, char* localfile)
{
	CLIENT *clnt;

	enum clnt_stat retval_1;
	int result_create;

	enum clnt_stat retval_3;
	int result_open;
	
	enum clnt_stat retval_4;
	int result_close;
	
	enum clnt_stat retval_6;
	int result_write;
	RWData result_read;


#ifndef	DEBUG
	clnt = clnt_create (host, FS_SERVER, FS_SERVER_V1, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	//Remote file of the server is created
	retval_1 = mi_creat_1(remotefile, S_IRWXU, &result_create,  clnt);
	if (retval_1 != RPC_SUCCESS) {
		clnt_perror (clnt, "Create call failed");
	}
	//Remote file of the server is opened for read
	retval_3 = mi_open_1(remotefile, WRITEMODE, &result_open, clnt);
	if (retval_3 != RPC_SUCCESS) {
		clnt_perror (clnt, "Open call failed");
	}
	//Local file is opened for read
	int localfd = open(localfile, O_RDONLY, 0666);
	if (localfd < 0){
		printf("Error opening local file (It may not exist) \n");
		exit(-1);
	}
	//We obtain the size of the local file
	int result_getSize = lseek(localfd, 0, SEEK_END);
	if (result_getSize < 0){
		printf("Error seeking local file \n");
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
                result_read.RWData_val = (char *) realloc(result_read.RWData_val,realsize);
            }else{ //if it is the first and only iteration we allocate the space needed
                result_read.RWData_val = malloc(realsize);
            }

		}else{
			//if it is the first iteration of several we allocate the max space permitted to send
			if(it == result_getSize/NFSMAXDATA2){
				realsize=NFSMAXDATA2;
                result_read.RWData_val = malloc(realsize);
			}
		}
		//The values of the string are resetted and actual position is updated
		memset(result_read.RWData_val, 0, realsize);
		result_read.RWData_len=realsize;
		position = pointer*NFSMAXDATA2;

		//We read the actual part of the local file
		lseek(localfd, position, 0);
		chuck = read(localfd, result_read.RWData_val, realsize);
		if (chuck < 0){
			printf("Error reading local file \n");
		}
		//We write the part in the remote file
		retval_6 = mi_write_1(result_open, result_read, position, realsize, &result_write, clnt);
		if (retval_6 != RPC_SUCCESS) {
			clnt_perror (clnt, "Write call failed");
		}
		
		it--;
        pointer++;
		atleast = 1;
	}	
	
	//Remote file is closed
	retval_4 = mi_close_1(result_open, &result_close, clnt);
	if (retval_4 != RPC_SUCCESS) {
		clnt_perror (clnt, "Close call failed");
	}
	//Local file is closed
	chuck = close(localfd);
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
		printf ("usage: %s local_file server_host remote_file\n",argv[0]);
		exit (1);
	}

	host = argv[2];
	remote = argv[3];
	local = argv[1];
	fs_server_1 (host, remote, local);
	exit (0);
}
