// #include <stdio.h>
// #include "../common/all.h"
// #include </home/kryo/emsdk/upstream/emscripten/cache/sysroot/include/emscripten/emscripten.h>

// // EMSCRIPTEN_KEEPALIVE int main()
// // {
// //   return 0;
// // }

// #ifdef __cplusplus
// #define EXTERN extern "C"
// #else
// #define EXTERN
// #endif

// EXTERN EMSCRIPTEN_KEEPALIVE void myFunction(int argc, char **argv)
// {
//   printf("MyFunction Called\n");
// }

// EXTERN EMSCRIPTEN_KEEPALIVE int add(int argc, char **argv)
// {
//   if (argc != 4)
//   {
//     perror("Expected params: int a, int b !");
//     return -1;
//   }
//   printf("MyFunction Called\n");
//   return 0;
// }

// // test client

// #define SERVER_ADDR "127.0.0.1"
// #define SERVER_PORT 5550
// #define BUFF_SIZE 1025

// #define FILE_EXISTS "File Already Exists!\n"
// #define FILE_SUCCESS "File Transfer Is Completed…\n"
// #define FILE_NOT_FOUND "Error: File Not Found!\n"
// #define FILE_WRONG_FORMAT "Error: Wrong File Format!\n"

// #define IMG_PREFIX '>'
// #define MSG_PREFIX '<'

// EXTERN EMSCRIPTEN_KEEPALIVE int _test_client(int argc, char **argv)
// {
//   int server_fd;
//   if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
//   { /* calls socket() */
//     perror("\nError: ");
//     exit(-1);
//   }

//   return server_fd;
// }

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
// #include "../common/all.h"
#include <emscripten/emscripten.h>
#include <emscripten/posix_socket.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 5551
#define BUFF_SIZE 8192

char *L_MSG_1 = "Login is successful!\n";
char *L_MSG_2 = "Not found!\n";
char *L_MSG_3 = "Account is blocked or inactive\n";
char *L_MSG_4 = "Password is not correct. Please try again!\n";
char *L_MSG_B = "Account is blocked!\n";
char *L_MSG_GOODBYE = "Goodbye ";

char LOGIN_PREFIX = '>';
char LOGOUT_PREFIX = '<';

/**
 * msg format: <LOGIN_PREFIX><username>:<password>\0
 */
int login_msg(char *buffer, char *username, char *pwd)
{
  long u_len = strlen(username);
  long p_len = strlen(pwd);

  if (u_len + p_len + 1 > BUFF_SIZE)
  {
    printf("Msg too long: %ld+%ld\n", u_len, p_len);
    return -1;
  }

  buffer[0] = LOGIN_PREFIX;
  strncpy(buffer + 1, username, u_len);
  buffer[u_len + 1] = ' ';
  strncpy(buffer + u_len + 2, pwd, p_len);
  return 0;
}

int main(int argc, char *argv[])
{
  // config
  int _port = SERVER_PORT;
  char *_server_addr = SERVER_ADDR;
  if (argc != 3)
  {
    perror("Expected: ./client IPAddress PortNumber (Ví dụ: ./client 127.0.0.1 5500)");
    // exit(0);
  }
  else
  {
    // _port = atoi(argv[2]);
    // _server_addr = argv[1];
  }

  char username[BUFF_SIZE], password[BUFF_SIZE];

  int client_sock;
  char buff[BUFF_SIZE];
  struct sockaddr_in server_addr; /* server's address information */
  int msg_len, bytes_sent, bytes_received;

  // Step 1: Construct socket
  client_sock = socket(AF_INET, SOCK_STREAM, 0);
  // PRINT_INT(client_sock);

  // Step 2: Specify server address
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(_port);
  server_addr.sin_addr.s_addr = inet_addr(_server_addr);
  // PRINT_INT(_port);

  // Step 3: Request to connect server
  if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0)
  {
    printf("\nError!Can not connect to sever! Client exit imediately! \n");
    return 0;
  }

  // Step 4: Communicate with server
  // 4.1 login
  // while (1)
  // {
  //   // send message
  //   printf("Enter username: ");
  //   memset(username, '\0', (strlen(username) + 1));
  //   // fgets(username, BUFF_SIZE, stdin);
  //   strcpy(username, "Hello");
  //   printf("Enter password: ");
  //   memset(password, '\0', (strlen(password) + 1));
  //   // fgets(password, BUFF_SIZE, stdin);
  //   strcpy(password, "Hi");

  //   login_msg(buff, username, password);
  //   PRINT_STR(buff);

  //   msg_len = strlen(buff);
  //   // if (msg_len == 0)
  //   // break;

  //   bytes_sent = send(client_sock, buff, msg_len, 0);
  //   if (bytes_sent <= 0)
  //   {
  //     printf("\nConnection closed!\n");
  //     // break;
  //     return 0;
  //   }

  //   // receive echo reply
  //   bytes_received = recv(client_sock, buff, BUFF_SIZE - 1, 0);
  //   if (bytes_received <= 0)
  //   {
  //     printf("\nError!Cannot receive data from sever!\n");
  //     // break;
  //     return 0;
  //   }

  //   buff[bytes_received] = '\0';
  //   printf("%s", buff);
  //   printf("---\n");

  //   // handle login
  //   if (strcmp(buff, L_MSG_1) == 0)
  //   {
  //     // break;
  //     return 0;
  //   }
  // }

  // // logout
  // while (1)
  // {
  //   char opt[BUFF_SIZE];
  //   printf("Enter 'Bye' to logout, else to quit: ");
  //   scanf("%s", opt);

  //   switch (strcasecmp(opt, "Bye"))
  //   {
  //   case 0:
  //     buff[0] = LOGOUT_PREFIX;
  //     strcpy(buff + 1, username);
  //     msg_len = strlen(buff);
  //     bytes_sent = send(client_sock, buff, msg_len, 0);
  //     if (bytes_sent <= 0)
  //     {
  //       printf("\nConnection closed!\n");
  //       break;
  //     }

  //     bytes_received = recv(client_sock, buff, BUFF_SIZE - 1, 0);
  //     if (bytes_received <= 0)
  //     {
  //       printf("\nError!Cannot receive data from sever!\n");
  //       break;
  //     }
  //     buff[bytes_received] = '\0';
  //     printf("%s", buff);
  //     printf("---\n");
  //     close(client_sock);
  //     return 0;
  //     break;
  //   default:
  //     close(client_sock);
  //     return 0;
  //   }
  // }

  // Step 4: Close socket
  close(client_sock);
  return 0;
}
