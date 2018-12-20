#include "pipe_networking.h"

static void sighandler(int signo){
  if (signo == SIGINT){
    printf("Exiting Server...\n");
    remove("/tmp/client_to_server");
    exit(0);
  }
}

int main() {

  signal(SIGINT, sighandler);

  int to_client;
  int from_client;

  from_client = server_handshake( &to_client );

  while(1){
    char * data = calloc(sizeof(char), 200);
    int status = read(from_client, data, 200);
    if (status == 0){
      from_client = server_handshake( &to_client );
    }
    else {
      data = strcat(data, " nya~");
      write(to_client, data, 200);
    }
  }


}
