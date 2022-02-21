/**
 * This program illustrates the functionality of the ipcs command on POSIX systems.
 *
 * sm.c
 *
 * Usage:
 *      gcc -o sm sm.c
 *
 *      ./sm 
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char *argv[])
{
	int segment_id;  			// identifier of the shared memory segment
	unsigned short mode;		// permissions of the segment
	struct shmid_ds shmbuffer; 	// the shared memory segment 


	// Step 1: Create a new shared memory segment using shmget
	segment_id = shmget(IPC_PRIVATE, 200 *sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
		
		
	// Step 2: Retrieve the information of the segment and store in shmbuffer		
	if (shmctl(segment_id, IPC_STAT, &shmbuffer) == -1) {
	  fprintf(stderr,"Unable to access segment %d\n",segment_id);
	  return -1;
	}

	// Step 3: output information about the segment in the required format

	/** report on the permission */
	mode = shmbuffer.shm_perm.mode;

	/** OWNER */
	if (mode & 0400)
		printf("r");
	else
		printf("-");
	if (mode & 0200)
		printf("w");
	else
		printf("-");
	if (mode & 0100)
		printf("x");
	else
		printf("-");

	/** GROUP */
	if (mode & 0040)
		printf("r");
	else
		printf("-");
	if (mode & 0020)
		printf("w");
	else
		printf("-");
	if (mode & 0010)
		printf("x");
	else
		printf("-");

	/** WORLD */
	if (mode & 0004)
		printf("r");
	else
		printf("-");
	if (mode & 0002)
		printf("w");
	else
		printf("-");
	if (mode & 0001)
		printf("x");
	else
		printf("-");

// Step 4: Create a new process using fork

	int pid = fork();



	// Step 5: The child process sends a message to the parent process via the 
//         shared memory segment created in Step 1 and the parent prints out 
//         the message it received from the child process

	char *shared_memory = (char *)shmat(segment_id, NULL, 0);

	if (pid == 0) { // child process
	  sprintf(shared_memory, "Hello parent process!");
	  shmdt(shared_memory);
	  
	}
	else { // parent process
	  wait(NULL);
	  printf("\nParent recieved message from Child: %s\n",shared_memory);
	  shmdt(shared_memory);
	}

	return 0;
}
