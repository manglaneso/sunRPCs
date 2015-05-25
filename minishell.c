#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

char current [42];

/* Here, the signal handler is implemented. It is called when the signal SIGTSTP (Ctrl -z) is sent to the process*/

static void signalHandler(int sign){
	/*If a command is in execution it will print it, if not, a message saying so will be printed*/
	if(strlen(current) > 0){
		printf("\nExecuting command %s\n", current);
	}else{
		printf("\nNo commands in execution\n");
	}
	

}

/* Here starts the main method */
int main(int argc, char *argv[]){

	/* An int a is initialized in 0 to perform an infinite loop,and an array of characters is declared to scan the entries from the user*/
	int a = 0;
	char command [42];
	/*This signal is implemeted to send the program to the signal handler method when the SIGTSTP is sent to the process */
	signal(SIGTSTP, signalHandler);
	/*Here starts the infinite loop to stop exit the program only when user wants to*/
	while( a == 0){
		/*The program prints "minishell>" and wait for an entry that can be ENTER (That's the reason for the "%[^\r\n]" and the getchar())*/
		printf("minishell> ");
		scanf("%[^\r\n]", command);
		getchar();
		strcpy(current, command);
		/*The program checks if the string entered is "exit", if it is the program exits returning 0*/
		if(strcmp("exit", command) == 0){
			return 0;
			exit(0);
		}
		/*If "command1" is entered, the program makes a fork and executes the command "uptime" with execvp*/
		if (strcmp("command1", command) == 0){
			/*The command inputed by the user is copied to deals with a signal SIGTSTP if one is sent to the process*/
			strcpy(current, command);
			int status;
			pid_t pidchild=fork();

			/*Check if the child is created properly*/
			if(pidchild == -1){
				perror("Fork error\n");
			}
			
			if(pidchild == 0){
				char *args [] = {"uptime", NULL};
				execvp(args[0], args);
			/*This while is executed to prevent a situation where the parent receives the status of a process that is being performed at the same time in background. This way the zombie processes are
			collected at the end of the execution of the program*/
			}while(wait(&status) != pidchild);
			/* Check if the child exits properly*/
			if (status == -1){
				perror("Child exits unsuccessfully\n");
				return -1;
		 	}
		}
		/*If "command2" is entered, the program makes a fork and executes the command "uname -a" with execvp*/
		if(strcmp("command2", command) == 0){
			/*The command inputed by the user is copied to deals with a signal SIGTSTP if one is sent to the process*/
			strcpy(current, command);
			int status;
			pid_t pidchild=fork();

			/*Check if the child is created properly*/
			if(pidchild == -1){
				perror("Fork error\n");
				
			}

			if(pidchild == 0){
				char *args [] = {"uname", "-a", NULL};
				execvp(args[0], args);

			}
			/*This while is executed to prevent a situation where the parent receives the status of a process that is being performed at the same time in background. This way the zombie processes are
			collected at the end of the execution of the program*/
			while(wait(&status) != pidchild);
			/* Check if the child exits properly*/
			if (status == -1){
				perror("Child exits unsuccessfully\n");
				return -1;
		 	}	
		}
		/*If "command3" is entered, the program, asks for a file, makes a fork, opens or creates the file, closes the standard output, dups the file and executes the command "cat/proc/cpuinfo" with execvp*/
		if( strcmp("command3", command) == 0){
			/*The command inputed by the user is copied to deals with a signal SIGTSTP if one is sent to the process*/
			strcpy(current, command);
			int status;
			int fd1;
			char cpuinfofile[42];
			scanf("%[^\r\n]", cpuinfofile);
			getchar();
			pid_t pidchild=fork();

			/*Check if the child is created properly*/
			if(pidchild == -1){
				perror("Fork error\n");
				return -1;				
			}

			if(pidchild == 0){
				close(1);
				/*Check if the file is opened correctly, if it does not exists, it is created*/
				if((fd1 = open(cpuinfofile, O_WRONLY | O_CREAT, 0666)) < 0){
					perror("Error opening the cpuninfo file.");
					return -1;
				}
				dup(fd1);
				char *args[] = {"cat", "/proc/cpuinfo", NULL};
				execvp(args[0],args); 
				
			}/*This while is executed to prevent a situation where the parent receives the status of a process that is being performed at the same time in background. This way the zombie processes are
			collected at the end of the execution of the program*/
			while(wait(&status) != pidchild);
			/* Check if the child exits properly*/
			if (status == -1){
				perror("Child exits unsuccessfully\n");
				return -1;
		 	}	
		}
		/*If "command4" is entered, the program, asks for a file, makes a fork, opens or creates the file, closes the standard input, dups the file and executes the command "grep model name input_file" with execvp*/
		if(strcmp("command4", command) == 0){
			/*The command inputed by the user is copied to deals with a signal SIGTSTP if one is sent to the process*/
			strcpy(current, command);
			int status;
			int fd1;
			char input[42];
			scanf("%[^\r\n]", input);
			getchar();
			pid_t pidchild=fork();

			/*Check if the child is created properly*/
			if(pidchild == -1){
				perror("Fork error\n");
				return -1;				
			}

			if(pidchild == 0){
				close(0);
				/*Check if the file is opened correctly*/
				if((fd1=open(input, O_RDONLY, 0666)) < 0){
					perror("Error opening the cpuinfo file.");
					return -1;
				}

				dup(fd1);
				char *args[] = {"grep", "model name", 0, NULL};
				execvp(args[0],args);

			}
			/*This while is executed to prevent a situation where the parent receives the status of a process that is being performed at the same time in background. This way the zombie processes are
			collected at the end of the execution of the program*/
			while(wait(&status) != pidchild);
			/* Check if the child exits properly*/
			if (status == -1){
				perror("Child exits unsuccessfully\n");
				return -1;
		 	}	
		}
		/*If "command5" is entered, the program will perform the operation of "command3" and "command4" together using a pipe instead of a file. It creates 2 childs*/
		if( strcmp("command5", command) == 0){
			/*The command inputed by the user is copied to deals with a signal SIGTSTP if one is sent to the process*/
			strcpy(current, command);
			int status;
			pid_t pidchild1=fork();

			/*Check if the first child is created properly*/
			if(pidchild1 == -1){
				perror("Fork error\n");
				return -1;
			}
				
			if(pidchild1 == 0){
				/*In the first child the pipe is created*/
				int p[2];
				pipe(p);
				int status1;
				pid_t pidchild2=fork();
				/*Check if the second child is created properly*/
				if(pidchild2 == -1){
					perror("Fork error\n");
					return -1;
				}

				if(pidchild2 == 0){
					/*In the second child the standard output is closed and the pipe's writting position is dupped to be pointed by the standard output, then the output of "cat/proc/cpuinfo" is stored in the pipe*/
					close(1);
					dup(p[1]);
					close(p[1]);
					close(p[0]);

					char *args[] = {"cat", "/proc/cpuinfo", NULL};
					execvp(args[0],args);
					
				}
				/*This while is executed to prevent a situation where the parent receives the status of a process that is being performed at the same time in background. This way the zombie processes are
				collected at the end of the execution of the program*/
				while(wait(&status1) != pidchild2);

				/* Check if the second child exits properly*/
				if (status1 == -1){
					perror("Child exits unsuccessfully\n");
					return -1;
		 		}
				/*In the first child the standard input is closed and the pipe's reading position is dupped to be pointed by the standard input, the pipe is closed, and then the command "grep model name 0"
				 prints on the screen the lines inside the standard input (the output of the first part of the command) which contains the patron "model name".*/
				close(0);  
				dup(p[0]);
				close(p[0]);
				close(p[1]);
				char *args[] = {"grep", "model name", 0, NULL};
				execvp(args[0],args);
						
			}
			/*This while is executed to prevent a situation where the parent receives the status of a process that is being performed at the same time in background. This way the zombie processes are
			collected at the end of the execution of the program*/
			while(wait(&status) != pidchild1);

			/* Check if the first child exits properly*/
			if (status == -1){
				perror("Child exits unsuccessfully\n");
				return -1;
		 	}
			
		}
		/*If "command6" is entered, the program will ask for a file to store the output of the command. To do this two childs will be created, one perform the first part of the command
		("du -ha") and the other one the second part of the command ("grep 4,0K 0")*/
		if( strcmp("command6", command) == 0){
			/*The command inputed by the user is copied to deals with a signal SIGTSTP if one is sent to the process*/
			strcpy(current, command);
			int status;
			char input[42];
			int fd1;

			scanf("%[^\r\n]", input);
			getchar();

			pid_t pidchild1=fork();
			/*Check if the first child is created properly*/
			if(pidchild1 == -1){
				perror("Fork error\n");
				return -1;
			}

			if(pidchild1 == 0){
				/*In the first child the pipe is created*/
				int p[2];
				pipe(p);
				int status1;
				pid_t pidchild2=fork();
				/*Check if the second child is created properly*/
				if(pidchild2 == -1){
					perror("Fork error\n");
					return -1;
				}

				if(pidchild2 == 0){		
					/*In the second child the standard output is closed and the pipe's writting position is dupped to be pointed by the standard output, then the output of "du -ha" is stored in the pipe*/
					close(1);
					dup(p[1]);
					close(p[1]);
					close(p[0]);

					char *args[] = {"du", "-ha",".", NULL};
					execvp(args[0],args);
					
				}/*This while is executed to prevent a situation where the parent receives the status of a process that is being performed at the same time in background. This way the zombie processes are
				collected at the end of the execution of the program*/
				while(wait(&status1) != pidchild2);
				/* Check if the second child exits properly*/
				if (status1 == -1 ){
					perror("Child exits unsuccessfully\n");
					return -1;
		 		}
				/*In the first child the standard input is closed and the pipe's reading position is dupped to be pointed by the standard input, then the standard output is closed, and the output file
				is dupped to be pointed by it and the pipe is closed, then the command "grep 4,0K 0" prints on the file the lines inside the standard input (the output of the first part of the command)
				 which contains the patron "4,0K".*/
				close(0);  
				dup(p[0]);
				close(p[0]);
				close(p[1]);
				/*Check if the file is opened correctly, if it does not exists, it is created*/
				if((fd1=open(input, O_WRONLY | O_CREAT, 0666)) < 0){
					perror("Error opening the input file.");
					return -1;
				}

				close(1);
				dup(fd1);
				close(fd1);

				char *args[] = {"grep", "4,0K", 0, NULL};
				execvp(args[0],args);
						
			}/*This while is executed to prevent a situation where the parent receives the status of a process that is being performed at the same time in background. This way the zombie processes are
			collected at the end of the execution of the program*/
			while(wait(&status) != pidchild1);
			/* Check if the second child exits properly*/		
			if (status == -1 ){
				perror("Child exits unsuccessfully\n");
				return -1;
		 	}
			
		}
		/*If "command7" is entered, the program will ask for two files, one to store the output of the command and the other one to show the errors of the command. To do this three childs will be created,
		 one perform the first part of the command ("du -ha"), the second one to perform the second part of the command ("grep 4,0K 0"), and the other one the second part of the command ("cut -f2")*/
		if( strcmp("command7", command) == 0){
		/*The command inputed by the user is copied to deals with a signal SIGTSTP if one is sent to the process*/
			strcpy(current, command);
			int status;
			char input[42];
			char input2[42];
			int fd1;
			int fd2;

			scanf("%[^\r\n]", input);
			getchar();
			scanf("%[^\r\n]", input2);
			getchar();
			
			pid_t pidchild1=fork();
			/*Check if the first child is created properly*/
			if(pidchild1 == -1){
				perror("Fork error\n");
				return -1;
			}
			/*In the first child, the first pipe is created*/
			if(pidchild1 == 0){
				int p[2];
				pipe(p);
				int status1;

				pid_t pidchild2=fork();
				/*Check if the second child is created properly*/
				if(pidchild2 == -1){
					perror("Fork error\n");
					return -1;
				}
				/*In the second child, the second pipe is created*/
				if(pidchild2 == 0){
					int p2[2];
					pipe(p2);
					int status2;
					pid_t pidchild3=fork();
					/*Check if the third child is created properly*/
					if(pidchild3 == -1){
						perror("Fork error\n");
						return -1;
					}

					if(pidchild3 == 0){
						/*In the third child the standard output is closed and the second pipe's writting position is dupped to be pointed by the standard output, then the output of "du -ha" is stored 
						in the second pipe*/
						close(1);
						dup(p2[1]);
						close(p2[1]);
						close(p2[0]);

						char *args[] = {"du", "-ha", ".", NULL};
						execvp(args[0],args);

					}/*This while is executed to prevent a situation where the parent receives the status of a process that is being performed at the same time in background. This way the zombie processes are
					collected at the end of the execution of the program*/
					while(wait(&status2) != pidchild3);
					/* Check if the third child exits properly*/
					if (status2 == -1 ){
						perror("Child exits unsuccessfully\n");
						return -1;
		 			}
		 			/*In the second child the standard input is closed and the second pipe's reading position is dupped to be pointed by the standard input, and the second pipe is closed. 
		 			Then the standard output is closed, and the first pipe's writting position is dupped to be pointed by it, then the command "grep 4,0K 0" prints on the first pipe the lines
		 			 inside the standard input (the output of the first part of the command) which contains the patron "4,0K".*/
					close(0);  
					dup(p2[0]);
					close(p2[0]);
					close(p2[1]);

					close(1);
					dup(p[1]);
					close(p[1]);
					close(p[0]);
							
					char *args[] = {"grep", "4,0K", 0, NULL};
					execvp(args[0],args);

				}/*This while is executed to prevent a situation where the parent receives the status of a process that is being performed at the same time in background. This way the zombie processes are
				collected at the end of the execution of the program*/
				while(wait(&status1) != pidchild2);
				/* Check if the second child exits properly*/
				if (status1 == -1 ){
					perror("Child exits unsuccessfully\n");
					return -1;
		 		}
		 		/*In the first child the standard input is closed and the first pipe's reading position is dupped to be pointed by the standard input, then the first pipe is closed.
		 		The error file is opened (or created), the standard error is closed, and the error file is dupped to be pointed by it.
				The output file is opened or created, the standard output is closed, and the output file is dupped to be pointed by it, then the command "cut -f2" prints on the file
				only the names of the files printed inside the standard input (the output of the second part of the command).*/
				close(0);
				dup(p[0]);
				close(p[0]);
				close(p[1]);
				/*Check if the error file is opened correctly, if it does not exists, it is created*/
				if((fd2=open(input2, O_WRONLY | O_CREAT, 0666)) < 0){
					perror("Error opening the error file.");
					return -1;
				}
				close(2);
				dup(fd2);
				/*Check if the output file is opened correctly, if it does not exists, it is created*/
				if((fd1=open(input, O_WRONLY | O_CREAT, 0666)) < 0){
					perror("Error opening the output file.");
					return -1;
				}
				close(1);
				dup(fd1);

				char *args[] = {"cut", "-f2", 0, NULL};
				execvp(args[0],args);

			}/*This while is executed to prevent a situation where the parent receives the status of a process that is being performed at the same time in background. This way the zombie processes are
			collected at the end of the execution of the program*/
			while(wait(&status) != pidchild1);	
			/* Check if the first child exits properly*/
			if (status == -1 ){
				perror("Child exits unsuccessfully\n");
				return -1;
		 	}	
			
		}
		/*If "command8" is entered, the program will perform exactly the same operation as "command6" but in background*/
		if( strcmp("command8", command) == 0){
			/*The command inputed by the user is copied to deals with a signal SIGTSTP if one is sent to the process*/
			strcpy(current, command);
			char input[42];
			int fd1;
			scanf("%[^\r\n]", input);
			getchar();
			
			pid_t pidchild1=fork();
			/*Check if the first child is created properly*/
			if(pidchild1 == -1){
				perror("Fork error\n");
				return -1;
			}

			if(pidchild1 == 0){	
				/*In the first child the pipe is created*/
				int p[2];
				pipe(p);
				pid_t pidchild2=fork();
				/*Check if the second child is created properly*/
				if(pidchild2 == -1){
					perror("Fork error\n");
					return -1;
				}

				if(pidchild2 == 0){	
					/*In the second child the standard output is closed and the pipe's writting position is dupped to be pointed by the standard output, then the output of "du -ha" is stored in the pipe*/
					close(1);
					dup(p[1]);
					close(p[1]);
					close(p[0]);

					char *args[] = {"du", "-ha", ".", NULL};
					execvp(args[0],args);
					
				}
				 /*In the first child the standard input is closed and the pipe's reading position is dupped to be pointed by the standard input, then the standard output is closed, and the output file
				is dupped to be pointed by it and the pipe is closed, then the command "grep 4,0K 0" prints on the file the lines inside the standard input (the output of the first part of the command)
				 which contains the patron "4,0K".*/
				close(0);  
				dup(p[0]);
				close(p[0]);
				close(p[1]);
				/*Check if the file is opened correctly, if it does not exists, it is created*/
				if((fd1=open(input, O_WRONLY | O_CREAT, 0666)) < 0){
					perror("Error opening the output file.");
					return -1;
				}
				close(1);
				dup(fd1);
				close(fd1);

				char *args[] = {"grep", "4,0K", 0, NULL};
				execvp(args[0],args);
			}
			/*The father does not wait to the first child to finish executing, neither the first child waits to the second child instead another command could be executed by the minishell instantly.
			 A zombie process will be created and the program prints its pid on the screen */
			printf("[%d]\n", pidchild1);
	
		}
		
		/*This two "memset" system calls will free the memory used by the strings command and current storing the value 0 on them on each iteration*/
		memset(command,'\0',strlen(command));
		memset(current,'\0',strlen(current));	
	
	}
	/*The program returns 0 if success*/
	return 0;
}