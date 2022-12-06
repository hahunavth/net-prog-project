#include "../../lib/common/all.h"

#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

#define PORT 5500

#define MAX_CLIENT_COUNT 10
#define INVALID_SOCKET -1
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
int g_dfd[MAX_CLIENT_COUNT] = {0};
int g_count = 0;

//
sem_t mutex;

#define SEM_WAIT \
  sem_wait(&mutex);
#define SEM_POST \
  sem_post(&mutex);

void *thr_client_exec(void *args)
{
  int cli_fd = *((int *)args);
  char buffer[1024];

  PRINT_INT(cli_fd);

  int bytes_received = recv(cli_fd, buffer, 1024, 0);

  PRINT_STR("Receved: ");
  PRINT_STR(buffer);

  close(cli_fd);
}

void at_exit_close_conn()
{
  printf("Exit!");
}

int main()
{
  atexit(at_exit_close_conn);
  sem_init(&mutex, 0, 1);
  pthread_t t[MAX_CLIENT_COUNT];
  int t_ids[MAX_CLIENT_COUNT];

  // fd_set readfds;
  // struct timeval tv;

  SOCKADDR_IN server, client;

  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  CATCH_STDERR;

  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  bind(socket_fd, (struct sockaddr *)&server, sizeof(server));
  CATCH_STDERR;

  listen(socket_fd, MAX_CLIENT_COUNT);
  CATCH_STDERR;

  while (g_count < MAX_CLIENT_COUNT - 5)
  {
    int sin_size = sizeof(SOCKADDR_IN);
    int cli_fd = accept(socket_fd, (struct sockaddr *)&client, &sin_size);
    CATCH_STDERR;

    PRINT_STR("Accept server: ");
    PRINT_INT(g_count);
    // PRINT_INT(select(MAX_CLIENT_COUNT, cli_fd, readfds, ))

    t_ids[g_count] = pthread_create(&t[g_count], NULL, thr_client_exec, (void *)&cli_fd);
    PRINT_INT(t_ids[g_count]);

    g_count++;
  }

  for (int i = 0; i < g_count; i++)
    // if (t[i] > 0)
    pthread_join(t[i], NULL);
  close(socket_fd);
  sem_destroy(&mutex);
  return 0;
}