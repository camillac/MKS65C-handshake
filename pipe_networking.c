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
  char * myfifo1 = "/tmp/client_to_server";
  // char * myfifo2 = "/tmp/downstream";

  int m = mkfifo(myfifo1, 0644);
  if (m < 0){
    printf("Error: %s\n", strerror(errno));
  }

  fds[0] = open(myfifo1, O_RDONLY);

  char * myfifo2 = malloc(200);
  read(fds[0], myfifo2, 200);
  printf("Recieved downpipe name...\n");
  remove(myfifo1);

  fds[1] = open(myfifo2, O_WRONLY);
  write(fds[1], "hi", 2);

  close(fds[1]);

  *to_client = fds[0];

  char * response = calloc(200, 1);
  read(fds[0], response, 200);
  printf("%s\n", response);
  close(fds[0]);

  return fds[1];
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
  char * myfifo1 = "/tmp/client_to_server";
  char * myfifo2 = "/tmp/server_to_client";

  int m = mkfifo(myfifo2, 0644);
  if (m < 0){
    printf("Error: %s\n", strerror(errno));
  }

  fds[0] = open(myfifo1, O_WRONLY);
  write(fds[0], myfifo2, 200);

  fds[1] = open(myfifo2, O_RDONLY);
  char * hi = calloc(1, 2);
  read(fds[1], hi, 2);
  printf("%s\n", hi);
  remove(myfifo2);
  close(fds[1]);

  char * r = "I'M DONE";
  write(fds[0], r, 200);

  *to_server = fds[0];
  close(fds[0]);


  return fds[1];
}
