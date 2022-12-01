#include "./common/all.h"
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

#define SEM_WAIT \
  sem_wait(&mutex);
#define SEM_POST \
  sem_post(&mutex);

typedef struct
{
  char *name;
} thread_test_args_t;

sem_t mutex;

void *thread_exec(void *_args)
{
  SEM_WAIT;
  thread_test_args_t *args = (thread_test_args_t *)_args;

  PRINT_STR(args->name);
  sleep(4);

  PRINT_STR("EXIT 1");

  SEM_POST;
  return NULL;
}

void *thread_exec2(void *_args)
{
  SEM_WAIT;

  thread_test_args_t *args = (thread_test_args_t *)_args;
  PRINT_STR(args->name);
  sleep(2);

  PRINT_STR("EXIT 2");

  SEM_POST;
  return NULL;
}

int main(int argc, char *argv)
{
  signal(SIGINT, sig_handler);
  sem_init(&mutex, 0, 1);

  pthread_t t1, t2;
  thread_test_args_t args, args2;

  args.name = "Hello 1";
  int thread = pthread_create(&t1, NULL, thread_exec, (void *)&args);

  args2.name = "Hello 2";
  int thread2 = pthread_create(&t2, NULL, thread_exec2, (void *)&args2);

  PRINT_STR("Hello from main");
  // PRINT_STR(NULL);
  PRINT_INT(5);

  // pthread_join(t1, NULL); // wait until thread end
  // pthread_join(t2, NULL);
  sem_destroy(&mutex);
  return 0;
}