#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {

  int fds[2];
  char * myfifo0 = "/tmp/client_to_server";

  int fifo_status = mkfifo(myfifo0, 0644);
  if (fifo_status < 0){
    printf("Error: %s\n", strerror(errno));
  }

  printf("Waiting for Client...\n");

  fds[0] = open(myfifo0, O_RDONLY);

  char * myfifo1 = malloc(200);
  read(fds[0], myfifo1, 200);
  printf("Recieved server_to_client Pipe Name...\n");
  remove(myfifo0);

  fds[1] = open(myfifo1, O_WRONLY);
  write(fds[1], "Connection with Server Successful", 200);

  *to_client = fds[1];

  char * response = calloc(200, sizeof(char));
  read(fds[0], response, 200);
  printf("%s\n", response);

  return fds[0];
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  int fds[2];
  char * myfifo0 = "/tmp/client_to_server";
  char * myfifo1 = "/tmp/server_to_client";

  int fifo_status = mkfifo(myfifo1, 0644);
  if (fifo_status < 0){
    printf("Error: %s\n", strerror(errno));
  }

  fds[0] = open(myfifo0, O_WRONLY);
  write(fds[0], myfifo1, 200);

  fds[1] = open(myfifo1, O_RDONLY);
  char * message = calloc(sizeof(char), 200);
  read(fds[1], message, 200);
  printf("%s\n", message);
  remove(myfifo1);

  char * response = "Connection with Client Successful";
  write(fds[0], response, 200);

  *to_server = fds[0];


  return fds[1];
}
