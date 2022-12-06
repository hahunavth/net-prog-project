#include "../../lib/common/all.h"

#include "../base/utils.h"

#define BREAK_IF_NOT_TRANSFER(byte_trans) \
  if (byte_trans <= 0)                    \
  {                                       \
    printf("Connection closed\n");        \
    break;                                \
  }

#define PORT 5500 /* Port that will be opened */
#define BACKLOG 2 /* Number of allowed connections */
#define BUFF_SIZE 8

int main()
{
  // fd
  int listen_sock, conn_sock;
  // msg - encode
  // msg_t *recv_msg = create_str_msg("");
  // buffer - transfer info
  char recv_data[BUFF_SIZE];
  char send_data[BUFF_SIZE];
  int bytes_sent, bytes_received;
  // address information
  struct sockaddr_in server;
  struct sockaddr_in client;
  //
  int sin_size;

  // Step 1: Construct a TCP socket to listen connection request
  listen_sock = socket(AF_INET, SOCK_STREAM, 0);
  CATCH_STDERR;

  // Step 2: Bind address to socket
  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);              /* Remember htons() from "Conversions" section? =) */
  server.sin_addr.s_addr = htonl(INADDR_ANY); /* INADDR_ANY puts your IP address automatically */
  bind(listen_sock, (struct sockaddr *)&server, sizeof(server));
  CATCH_STDERR;

  listen(listen_sock, BACKLOG);
  CATCH_STDERR;

  while (1)
  {
    // NOTE: accept request
    sin_size = sizeof(struct sockaddr_in);
    conn_sock = accept(listen_sock, (struct sockaddr *)&client, &sin_size);
    CATCH_STDERR;

    // NOTE: start convension
    // Step 1: recv
    while (1)
    {
      bytes_received = recv(conn_sock, recv_data, BUFF_SIZE - 1, 0);
      BREAK_IF_NOT_TRANSFER(bytes_received);
      // Step 1.1: if recv str -> end str
      if (recv_data[bytes_received - 1] == '\n')
      {
        recv_data[bytes_received - 1] = '\0';
      }
      else
      {
        recv_data[bytes_received] = '\0';
      }
      PRINT_STR(recv_data);
    }
    // Step 2: send
    // strcpy(send_data, "a");
    // bytes_sent = send(conn_sock, send_data, strlen(send_data), 0);
    // NOTE: Send long msg
    // char *long_msg = "Hello from ????????????? !!!!!!!!!!!!!!!!!!!!!!!! ???";
    // int i = 0;
    // while (i < strlen(long_msg))
    // {
    //   strncpy(send_data, long_msg + i, BUFF_SIZE);
    //   bytes_sent = send(conn_sock, send_data, strlen(send_data), 0);
    //   i += bytes_sent;
    // }

    close(listen_sock);
    //
    break; // connect 1 time and close
    //
  }
  close(listen_sock);
  // perror("");
  return 0;
}