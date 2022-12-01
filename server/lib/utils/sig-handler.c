#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "./sig-handler.h"

void sig_handler(int signum)
{
  printf("\nInside handler function\n");
  signal(SIGINT, SIG_DFL); // Re Register signal handler for default action
}