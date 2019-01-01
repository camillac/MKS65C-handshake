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

  // pid_t parent = getpid();

  int pid = fork();

  while(1){
    if (pid == -1){
      printf("Error: %s\n", strerror(errno));
      return 0;
    }
    else if (pid == 0){
      char * data = calloc(sizeof(char), 200);
      int status = read(from_client, data, 200);
      if (status == 0){
        exit(0);
      }
      else {
        data = strcat(data, " nya~");
        write(to_client, data, 200);
      }
    }
    else {
      from_client = server_handshake( &to_client );
      pid = fork();
    }
  }


}
