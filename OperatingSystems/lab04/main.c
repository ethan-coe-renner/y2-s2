#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main() {
  // Step 1
  int voters, rounds;
  printf("Enter the number of voters: ");
  scanf("%d", &voters);

  printf("Enter the number of rounds: ");
  scanf("%d", &rounds);

  // Step 2
  int ** pipes = malloc(voters * sizeof(int *));

  for (int i = 0; i<voters; i++) {
    pipes[i] = malloc(2 * sizeof(int));
    pipe(pipes[i]);
  }


  // Step 3
  int pid, voter_id = -1;
  for (int i = 0; i < voters; i++) {
    pid = fork();

    if (pid == 0) {
      voter_id = i;
      close(pipes[i][0]); // close read end of voter
      break;
    }
    else {
      close(pipes[i][1]); // close write end of judge
    }
  }

  // Step 4
  int nbytes;
  for (int round = 1; round <= rounds; round++) {
    if (voter_id == -1) {
      int vote_total = 0;
      printf("\n\n------ Round %d ------\n", round);
      for (int voter = 0; voter < voters; voter++) {
	int vote;
	nbytes = read(pipes[voter][0],&vote,sizeof(int));

	vote_total += vote;
	printf("Round %d: Voter %d votes: \n", round, voter);
	if (vote) {
	  printf("YES\n");
	}
	else {
	  printf("NO\n");
	}
      }
      printf("--- Round %d: The voting result is a ", round);
      if (vote_total >= voters / 2) {
	printf("YES.\n");
      }
      else {
	printf("NO.\n");
      }
    }
    else {
      srand(time(0) * voter_id * round);
      int vote = rand()%2;
      /* printf("JURER: round %d, voter %d voted: %d\n", round, voter_id, vote); */
      write(pipes[voter_id][1], &vote, sizeof(int));
    }
  }
}
