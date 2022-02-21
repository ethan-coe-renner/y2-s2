#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int *fibs; /* this data is shared by the thread(s) */
void *fib(void *param);
int main(int argc, char *argv[])
{
  pthread_t tid; /* the thread identifier */
  pthread_attr_t attr; /* set of attributes for the thread */
  if (argc != 2) {
    fprintf(stderr,"usage: a.out <integer value>\n");
    return -1;
  }
  if (atoi(argv[1]) < 0) {
    fprintf(stderr,"Argument %d must be non-negative\n",atoi(argv[1]));
    return -1;
  }

  int input = atoi(argv[1]);

  fibs = malloc(input * sizeof(int));

  /* get the default attributes */
  pthread_attr_init(&attr);
  /* create the thread */
  pthread_create(&tid,&attr,fib,argv[1]);
  /* now wait for the thread to exit */
  pthread_join(tid,NULL);

  printf("[%d",fibs[0]);
  for (int i = 1; i< input; i++) {
    printf(", %d", fibs[i]);
  }
  printf("]\n");
}

void *fib(void *param) {
  int a = 1, b = 1,i=0, input = atoi(param),c;

  while (i < input) {
    fibs[i] = a;
    c = a + b;
    a = b;
    b = c;
    i++;
  }

  pthread_exit(0);
}
