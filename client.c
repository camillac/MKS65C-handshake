#include "pipe_networking.h"

static void sighandler(int signo){
  if (signo == SIGINT){
    printf("\n");
    exit(0);
  }
}

int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  while(1){
    printf("ENTER INPUT: ");
    char * input = calloc(sizeof(char), 200);
    fgets(input, 200, stdin);
    char * p = strchr(input, '\n');
    if (p) *p = 0;

    write(to_server, input, 200);

    char * response = calloc(sizeof(char), 200);
    read(from_server, response, 200);
    printf("Response: %s\n", response);
  }

  return 0;
}
