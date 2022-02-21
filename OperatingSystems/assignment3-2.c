#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main() {
  int fdi[2], fdo[2], nbytes;

  char    string[] = "Hello, World";

  char readbuffer[80];

  pipe(fdi);
  pipe(fdo);

  int pid = fork();

  // child process 
  if (pid == 0) {
    close(fdi[1]); // close output side of pipe 
    close(fdo[0]);

    nbytes = read(fdi[0], readbuffer, sizeof(readbuffer));

    printf("Child recieved message: \"%s\"\n", readbuffer);

    for (int i = 0; i < (int)sizeof(readbuffer); i++) {
      char curchar = readbuffer[i];
      if (curchar < 123 && curchar > 96) {
	readbuffer[i] -= 32;
      }
      else if (curchar < 91 && curchar > 64) {
	readbuffer[i] += 32;
      }
      
    }
    write(fdo[1], readbuffer, (strlen(readbuffer)+1));
  }
  // parent
  else {
    close(fdi[0]); // close output side of pipe 
    close(fdo[1]);
    write(fdi[1], string, (strlen(string)+1));

    nbytes = read(fdo[0], readbuffer, sizeof(readbuffer));

    printf("Parent recieved message: \"%s\"\n", readbuffer);
    
  }
}
